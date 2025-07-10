import json
import os

en_json_path = os.path.join(os.path.dirname(__file__), "en.json")
DEFAULT_EN_MENU = json.load(open(en_json_path, "r"))

test = {"menu": {"atc": {"have_sub_menu": true, "hotkey": "F5", "submenu": {
    "[na]kutaisi(4.413441263mhz)": {"have_sub_menu": true, "hotkey": "F1", "submenu": {
        "requeststart-up": {"have_sub_menu": false, "hotkey": "F3", "submenu": {}},
        "previousmenu": {"have_sub_menu": false, "hotkey": "F11", "submenu": {}},
        "exit": {"have_sub_menu": false, "hotkey": "F12", "submenu": {}}}},
    "[na]senaki-kolkhi(4.313240.6261mhz)(neutral)": {"have_sub_menu": true, "hotkey": "F2", "submenu": {
        "requeststart-up": {"have_sub_menu": false, "hotkey": "F3", "submenu": {}},
        "previousmenu": {"have_sub_menu": false, "hotkey": "F11", "submenu": {}},
        "exit": {"have_sub_menu": false, "hotkey": "F12", "submenu": {}}}},
    "[na]kobuleti(4.3513340.8262mhz)(neutral)": {"have_sub_menu": true, "hotkey": "F3", "submenu": {
        "requeststart-up": {"have_sub_menu": false, "hotkey": "F3", "submenu": {}},
        "previousmenu": {"have_sub_menu": false, "hotkey": "F11", "submenu": {}},
        "exit": {"have_sub_menu": false, "hotkey": "F12", "submenu": {}}}},
    "[na]batumi(4.2513140.4260mhz)(neutral)": {"have_sub_menu": true, "hotkey": "F4", "submenu": {
        "requeststart-up": {"have_sub_menu": false, "hotkey": "F3", "submenu": {}},
        "previousmenu": {"have_sub_menu": false, "hotkey": "Fil", "submenu": {}},
        "exit": {"have_sub_menu": false, "hotkey": "F12", "submenu": {}}}},
    "[na]sukhumi-babushara(4.1512940258mhz)..": {"have_sub_menu": false, "hotkey": "F5", "submenu": {}},
    "[na]nalchik(4.513641.4265mhz)(neutral)": {"have_sub_menu": true, "hotkey": "F6", "submenu": {
        "requeststart-up": {"have_sub_menu": false, "hotkey": "F3", "submenu": {}},
        "previousmend": {"have_sub_menu": false, "hotkey": "F11", "submenu": {}},
        "exit": {"have_sub_menu": false, "hotkey": "F12", "submenu": {}}}},
    "[na]gudauta(4.213040.2259mhz)(neutral)": {"have_sub_menu": true, "hotkey": "F7", "submenu": {
        "requeststart-up": {"have_sub_menu": false, "hotkey": "F3", "submenu": {}},
        "previousmenu": {"have_sub_menu": false, "hotkey": "F11", "submenu": {}},
        "exit": {"have_sub_menu": false, "hotkey": "F12", "submenu": {}}}},
    "[na]beslan(4.7514142.4270mhz)(neutral)": {"have_sub_menu": true, "hotkey": "F8", "submenu": {
        "requeststart-up": {"have_sub_menu": false, "hotkey": "F3", "submenu": {}},
        "previousmenu": {"have_sub_menu": false, "hotkey": "F11", "submenu": {}},
        "exit": {"have_sub_menu": false, "hotkey": "F12", "submenu": {}}}},
    "[na]tbilisi-lochini(4.613841.8267mhz)(neutral)": {"have_sub_menu": true, "hotkey": "F9", "submenu": {
        "requeststart-up": {"have_sub_menu": false, "hotkey": "F3", "submenu": {}},
        "previousmenu": {"have_sub_menu": false, "hotkey": "F11", "submenu": {}},
        "exit": {"have_sub_menu": false, "hotkey": "F12", "submenu": {}}}},
    "[na]soganlug(4.6513942268mhz)(neutral)..": {"have_sub_menu": false, "hotkey": "F10", "submenu": {}},
    "previousmenu": {"have_sub_menu": false, "hotkey": "F11,", "submenu": {}},
    "exit": {"have_sub_menu": false, "hotkey": "F12,", "submenu": {}}}},
                 "groundcrew": {"have_sub_menu": true, "hotkey": "F8",
                                "submenu": {"rearm&refuel": {"have_sub_menu": false, "hotkey": "F1", "submenu": {}},
                                            "groundelectricpower": {"have_sub_menu": true, "hotkey": "F2", "submenu": {
                                                "on": {"have_sub_menu": false, "hotkey": "F1", "submenu": {}},
                                                "off": {"have_sub_menu": false, "hotkey": "F2", "submenu": {}},
                                                "previousmenu": {"have_sub_menu": false, "hotkey": "F11",
                                                                 "submenu": {}},
                                                "exit": {"have_sub_menu": false, "hotkey": "F12,", "submenu": {}}}},
                                            "requestrepair": {"have_sub_menu": false, "hotkey": "F3", "submenu": {}},
                                            "wheelchocks5": {"have_sub_menu": true, "hotkey": "F4", "submenu": {
                                                "place": {"have_sub_menu": false, "hotkey": "F1", "submenu": {}},
                                                "remove": {"have_sub_menu": false, "hotkey": "F2", "submenu": {}},
                                                "previousmenu": {"have_sub_menu": false, "hotkey": "F11",
                                                                 "submenu": {}},
                                                "exit": {"have_sub_menu": false, "hotkey": "F12", "submenu": {}}}},
                                            "salute!": {"have_sub_menu": false, "hotkey": "F6", "submenu": {}},
                                            "requestlaunch": {"have_sub_menu": false, "hotkey": "F7", "submenu": {}},
                                            "changehelmet-mounteddevice": {"have_sub_menu": true, "hotkey": "F8",
                                                                           "submenu": {
                                                                               "setupjhmcs": {"have_sub_menu": false,
                                                                                              "hotkey": "FI",
                                                                                              "submenu": {}},
                                                                               "setupnvg": {"have_sub_menu": false,
                                                                                            "hotkey": "F2",
                                                                                            "submenu": {}},
                                                                               "previousmenu": {"have_sub_menu": false,
                                                                                                "hotkey": "F11",
                                                                                                "submenu": {}},
                                                                               "exit": {"have_sub_menu": false,
                                                                                        "hotkey": "F12,",
                                                                                        "submenu": {}}}},
                                            "dtcinterface": {"have_sub_menu": true, "hotkey": "F9", "submenu": {
                                                "show": {"have_sub_menu": false, "hotkey": "F1", "submenu": {}},
                                                "hide": {"have_sub_menu": false, "hotkey": "F2", "submenu": {}},
                                                "previousmenu": {"have_sub_menu": false, "hotkey": "F11",
                                                                 "submenu": {}},
                                                "exit": {"have_sub_menu": false, "hotkey": "F12,", "submenu": {}}}},
                                            "previousmenu": {"have_sub_menu": false, "hotkey": "F11", "submenu": {}},
                                            "exit": {"have_sub_menu": false, "hotkey": "F12", "submenu": {}}}},
                 "exit": {"have_sub_menu": false, "hotkey": "F12", "submenu": {}}}}
