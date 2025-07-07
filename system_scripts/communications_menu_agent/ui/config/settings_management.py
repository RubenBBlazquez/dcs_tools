import json
import os
import attr
from typing import Dict

SETTINGS_FILE = "config/settings.json"


@attr.s(auto_attribs=True)
class Settings:
    """
    Save all the information necessary for the UI.

    Attributes
    ----------
    monitor_index : int
        Index of your monitor in your OS
    last_capture_coordinates : dict[str, int]
        Coordinates of the menu in DCS to take the screenshots
    start_menu_discovery_hot_key : str
        Hotkey to activate menu discovery
    voice_recorder_hot_key : str
        Hotkey to activate voice command recording

    """
    monitor_index: int = 0
    last_capture_coordinates: Dict[str, int] = attr.Factory(dict)
    start_menu_discovery_hot_key: str = ""
    voice_recorder_hot_key: str = ""

    @classmethod
    def create_settings_from_file(cls):
        if os.path.exists(SETTINGS_FILE):
            with open(SETTINGS_FILE, "r") as f:
                data = json.load(f)

            return cls(**data)

        return cls()

    def save_settings_in_file(self):
        os.makedirs(os.path.dirname(SETTINGS_FILE), exist_ok=True)

        with open(SETTINGS_FILE, "w") as f:
            json.dump(attr.asdict(self), f, indent=4)

    def set_monitor_index(self, index: int):
        self.monitor_index = index

    def set_start_menu_discovery_hotkey(self, value: str):
        self.start_menu_discovery_hot_key = value

    def set_voice_recorder_hotkey(self, value: str):
        self.voice_recorder_hot_key = value
