import io
import json
import os
import re
from json import JSONDecodeError
from typing import Any

import keyboard
import mss
import attr
from PIL import Image
from google.cloud import vision
from google.oauth2 import service_account
from pywinauto import Desktop
import time
from ui.components.gemini_assistant_actions import check_menu_item_most_likely, MenuItem
from ui.config.default_communications.EN.en_parsed import DEFAULT_EN_MENU
from ui.config.settings_management import Settings

# When a dcs mission starts, can happens that we dont have already
# the terrain, so, until we have it, is Unknown
DEFAULT_TERRAIN = "UNKNOWN"

class TerrainNotDefinedError(Exception):
    def __init__(self):
        self.msg = "Terrain Not Found, Check if you set the files correctly inside DCS Game Folder"

@attr.s(auto_attribs=True)
class DCSMenuParser:
    """
    Class in charge to establish the settings information
    of the area where we are going to take the screenshots
    to do the menu discovering

    Attributes
    ----------
    settings: Settings
        Object with all configurations of the ui app
    terrain: str
        Terrain of the dcs mision we are currently on
    """
    _settings: Settings
    _terrain: str = attr.ib(default=DEFAULT_TERRAIN)
    _vision_client: vision.ImageAnnotatorClient = attr.ib(init=False)
    _parsed_dcs_menu: dict[str, Any] = attr.ib(init=False, factory=dict)

    def __attrs_post_init__(self):
        key_path = os.path.join(os.path.dirname(__file__), "..", "config", "cloud_vision_service_account.json")
        creds = service_account.Credentials.from_service_account_file(key_path)
        self._vision_client = vision.ImageAnnotatorClient(credentials=creds)
        self._parsed_dcs_menu.setdefault("menu", {})

    def _get_dcs_menu_screenshot(self) -> bytes:
        """
        Capture a screenshot of a specific region of the screen and return it as a PNG image in bytes.

        This method captures a screenshot based on previously defined coordinates, converts the
        captured image into a PNG format, and returns the binary content of the image. The captured
        region corresponds to a rectangular area defined in the saved settings.

        :return: PNG representation of the captured screenshot as bytes
        :rtype: bytes

        """
        coords = self._settings.last_capture_coordinates
        screen_rectangle = {
            "left": coords["x1"],
            "top": coords["y1"],
            "width": coords["x2"] - coords["x1"],
            "height": coords["y2"] - coords["y1"]
        }

        with mss.mss() as sct:
            time.sleep(0.3)
            sct_img = sct.grab(screen_rectangle)

        img = Image.frombytes("RGB", sct_img.size, sct_img.rgb)
        img_bytes = io.BytesIO()
        img.save(img_bytes, format="PNG")
        img_bytes.seek(0)

        return img_bytes.read()

    def _parse_image_with_ocr(self, image_bytes: bytes) -> tuple[list[str], list[str]]:
        """
        Parses an image using OCR (Optical Character Recognition) to extract text.

        The function leverages a vision client to process the provided image bytes
        for text detection. It splits detected text into a menu's title items and
        options.

        :param image_bytes: The image data in bytes format to be processed.
        :type image_bytes: bytes
        :return: A tuple containing a list of menu title items and a list of menu
            options extracted from the image text.
        :rtype: tuple[list[str], list[str]]
        :raises Exception: If an error occurs during the OCR process, with details
            from the response error message.
        """
        image = vision.Image(content=image_bytes)
        response = self._vision_client.text_detection(image=image)

        if response.error.message:
            raise Exception(response.error.message)

        text_obtained = response.text_annotations[0].description

        menu_items_with_hotkeys = []
        menu_options = re.findall(r'(F[\w.,-]+\s.*?)(?=\nF[\w.,-]+\s|\Z)', text_obtained, re.DOTALL)

        for option in menu_options:
            try:
                split_option = option.split('. ')
                hotkey, menu_item = (
                    split_option[0].strip(),
                    split_option[1]
                    .replace(' ', '')
                    .replace("\n", "")
                    .replace("%", "")
                    .replace("/", "")
                    .lower()
                )
            except Exception as e:
                split_element = option.split(' ')
                menu_item = (
                    ' '.join(split_element[1:])
                    .replace(' ', '')
                    .replace(' ', '')
                    .replace("\n", "")
                    .replace("%", "")
                    .replace("/", "")
                    .lower()
                )
                hotkey = split_element[0]

            hotkey.replace("i", "1").replace("L", "1")
            menu_items_with_hotkeys.append((hotkey, menu_item))

        menu_title_items = (
            re.split(
                r'^F[\w.,-]+\s',
                response.text_annotations[0].description,
                maxsplit=1,
                flags=re.MULTILINE
            )[0]
            .replace("\n", "")
            .replace("AUTO", "")
            .replace("/", "")
            .replace("\\", "")
            .split('. ')
        )
        menu_title_items = [
            menu_title.replace(' ','').lower()
            for menu_title in menu_title_items
        ]

        return menu_title_items, menu_items_with_hotkeys

    def _edit_specific_deep_key_submenu_in_parsed_dcs_menu(self, keys: list[str], value: dict[str, Any]):
        element_to_edit = self._parsed_dcs_menu["menu"]

        for index in range(len(keys)):
            key = keys[index]

            try:
                element_to_edit = element_to_edit[key]
            except KeyError:
                try:
                    element_to_edit = element_to_edit["submenu"][key]
                except KeyError:
                    correct_key = check_menu_item_most_likely(
                        element_to_edit["submenu"],
                        key,
                        MenuItem.LABEL
                    )
                    element_to_edit = element_to_edit["submenu"][correct_key]

            if index == len(keys) - 1:
                element_to_edit["submenu"].update(value)

    def _focus_dcs_screen(self):
        for w in Desktop(backend="uia").windows():
            if "digital combat simulator" in w.window_text().lower().strip():
                print(f"Found: {w.window_text()}")
                w.set_focus()



    def discovery_scan_dcs_menu_json(self):
        self.iterate_and_parse_dcs_menu()
        self._save_parsed_menu()

    def _save_parsed_menu(self):
        parsed_menus_dcs_file = os.path.join(os.path.dirname(__file__), "test_config", "parsed_menus_dcs.json")
        try:
            parsed_menus_dcs = json.load(open(parsed_menus_dcs_file, 'a+'))
        except JSONDecodeError:
            parsed_menus_dcs = {}

        retries = 20
        while retries > 0:
            if self._terrain != DEFAULT_TERRAIN:
                break

            time.sleep(0.5)
            retries-=1

        if self._terrain == DEFAULT_TERRAIN:
            raise TerrainNotDefinedError()

        parsed_menus_dcs.setdefault(self._terrain, {})
        parsed_menus_dcs[self._terrain].update(self._parsed_dcs_menu)
        json.dump(parsed_menus_dcs, open(parsed_menus_dcs_file, 'w'), indent=4)

    def iterate_and_parse_dcs_menu(self):
        dcs_screenshot = self._get_dcs_menu_screenshot()
        menu_title_items, parsed_menu_items = self._parse_image_with_ocr(dcs_screenshot)
        is_submenu = len(menu_title_items) > 1 and menu_title_items[0].isnumeric()
        hotkeys_elements_with_submenu = []

        for hotkey, menu_item in parsed_menu_items:
            have_sub_menu = '...' in menu_item
            menu_item = menu_item.replace('...', '')

            is_element_submenu_in_default_configs = menu_item in DEFAULT_EN_MENU
            print(f"Menu Item: {menu_item}, Hotkey: {hotkey}, Submenu: {have_sub_menu}")

            menu_item_dict = {
                menu_item: {
                    'have_sub_menu': have_sub_menu,
                    'hotkey': hotkey,
                    'submenu': (
                        DEFAULT_EN_MENU[menu_item]
                        if is_element_submenu_in_default_configs
                        else {}
                    )
                }
            }

            if have_sub_menu and not is_element_submenu_in_default_configs:
                hotkeys_elements_with_submenu.append(hotkey.strip())

            if not is_submenu:
                self._parsed_dcs_menu["menu"].update(menu_item_dict)
                continue

            # Each Menu has at the start [Number. Main. Submenu. SubMenu]
            # we take only the submenus keys
            menu_title_keys = menu_title_items[2:]
            self._edit_specific_deep_key_submenu_in_parsed_dcs_menu(menu_title_keys, menu_item_dict)

        self._focus_dcs_screen()
        time.sleep(0.2)

        for hotkey in hotkeys_elements_with_submenu:
            keyboard.send(hotkey)
            time.sleep(0.2)
            print(f"hotkey pressed {hotkey}")
            self.iterate_and_parse_dcs_menu()

        keyboard.send("F11")
