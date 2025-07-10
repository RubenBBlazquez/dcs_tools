import io
import math
import os
from typing import Any

import mss
import attr
from PIL import Image
from google.cloud import vision
from google.oauth2 import service_account

from ui.config.default_communications.EN.en_parsed import DEFAULT_EN_MENU
from ui.config.settings_management import Settings


@attr.s(auto_attribs=True)
class DCSMenuParser:
    """
    Class in charge to establish the settings information
    of the area where we are going to take the screenshots
    to do the menu discovering

    Attributes
    ----------
    _settings: Settings
        Object with all configurations of the ui app
    """
    _settings: Settings
    _menu_json: dict[str, Any] = attr.ib(init=False, factory=dict)
    _vision_client: vision.ImageAnnotatorClient = attr.ib(init=False)
    _parsed_dcs_menu: dict[str, Any] = attr.ib(init=False, factory=dict)

    def __attrs_post_init__(self):
        pass
        # key_path = os.path.join(os.path.dirname(__file__), "..", "config", "cloud_vision_service_account.json")
        # creds = service_account.Credentials.from_service_account_file(key_path)
        # self._vision_client = vision.ImageAnnotatorClient(credentials=creds)

    def _get_dcs_menu_screenshot(self) -> bytes:
        coords = self._settings.last_capture_coordinates
        screen_rectangle = {
            "left": coords["x1"],
            "top": coords["y1"],
            "width": coords["x2"] - coords["x1"],
            "height": coords["y2"] - coords["y1"]
        }

        with mss.mss() as sct:
            sct_img = sct.grab(screen_rectangle)

        img = Image.frombytes("RGB", sct_img.size, sct_img.rgb)
        img_bytes = io.BytesIO()
        img.save(img_bytes, format="PNG")
        img_bytes.seek(0)

        return img_bytes.read()

    def _parse_image_with_ocr(self, image_bytes: bytes) -> list[str]:
        image = vision.Image(content=self._get_dcs_menu_screenshot())
        response = self._vision_client.text_detection(image=image)

        if response.error.message:
            raise Exception(response.error.message)

        return response.text_annotations[0].description.split("\n")[1:]

    def _edit_specific_deep_key_submenu_in_parsed_dcs_menu(self, keys: list[str], value: dict[str, Any]):
        element_to_edit = self._parsed_dcs_menu["menu"]

        for index in range(len(keys)):
            key = keys[index].strip()

            try:
                element_to_edit = element_to_edit[key]
            except Exception:
                element_to_edit = element_to_edit["submenu"][key]

            if index == len(keys) - 1:
                element_to_edit["submenu"].update(value)

    def iterate_and_parse_dcs_menu(self):
        dcs_screenshot = self._get_dcs_menu_screenshot()
        parsed_menu_items = self._parse_image_with_ocr(dcs_screenshot)
        menu_title_items = parsed_menu_items.pop(0)
        split_menu_title = menu_title_items.split('. ')
        is_submenu = len(split_menu_title) > 1 and split_menu_title[0].isnumeric()
        hotkeys_elements_with_submenu = []

        for element in parsed_menu_items:
            have_sub_menu = '...' in element
            element = element.replace('...', '').replace(',', '.')
            split_element = element.split('. ')
            hotkey, menu_item = split_element
            is_element_submenu_in_default_configs = element in DEFAULT_EN_MENU

            menu_item_dict = {
                menu_item: {
                    'have_sub_menu': have_sub_menu,
                    'hotkey': hotkey.strip(),
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
                self._parsed_dcs_menu.setdefault("menu", {}).update(menu_item_dict)
                continue

            # Each Menu has at the start [Number. Main. Submenu. SubMenu]
            # we take only the submenus keys
            menu_title_items = split_menu_title[2:]
            self._edit_specific_deep_key_submenu_in_parsed_dcs_menu(menu_title_items, menu_item_dict)


