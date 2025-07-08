import json
import os

en_json_path = os.path.join(os.path.dirname(__file__), "en.json")
DEFAULT_EN_MENU = json.load(open(en_json_path, "r"))