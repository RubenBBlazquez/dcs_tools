import json
import os
from unittest import mock

import pytest
from unittest.mock import patch, MagicMock
from ui.components.json_menu_composer import DCSMenuParser, TerrainNotDefinedError


@pytest.fixture
def parser(tmp_path):
    class DummySettings:
        last_capture_coordinates = {"x1": 0, "y1": 0, "x2": 10, "y2": 10}

    with patch.object(DCSMenuParser, '__attrs_post_init__', return_value=None):
        yield DCSMenuParser(DummySettings())


def test_iterate_and_parse_dcs_menu_multiple_run(parser):
    first_ocr = ["Main", "F1. ATC..."]
    second_ocr = [
        '2. Main. ATC',
        'F1. [N/A] Kutaisi (4.4 / 134 / 41 / 263 MHz)...',
        'F2. [N/A] Senaki-Kolkhi (4.3 / 132 / 40.6 / 261 MHz) (Neutral)',
        'F11, Previous Menu',
        'F12, Exit'
    ]
    third_ocr = [
        '2. Main. ATC. [N/A] Kutaisi (4.4 / 134 / 41 / 263 MHz)',
        'F1. Inbound',
    ]
    parse_image_ocr_mock = patch.object(parser, "_parse_image_with_ocr", side_effect=[first_ocr, second_ocr, third_ocr])
    get_dcs_menu_screenshot_mock = patch.object(parser, "_get_dcs_menu_screenshot", return_value=b'')

    with parse_image_ocr_mock, get_dcs_menu_screenshot_mock:
        parser._edit_specific_deep_key = MagicMock()
        parser.iterate_and_parse_dcs_menu()

    expected_menu_parsed = {
        "menu": {
            "ATC": {
                "have_sub_menu": True,
                "hotkey": "F1",
                "submenu": {
                    "[N/A] Kutaisi (4.4 / 134 / 41 / 263 MHz)": {
                        "have_sub_menu": True,
                        "hotkey": "F1",
                        "submenu": {
                            "Inbound": {
                                "have_sub_menu": False,
                                "hotkey": "F1",
                                "submenu": {}
                            },
                        }
                    },
                    "[N/A] Senaki-Kolkhi (4.3 / 132 / 40.6 / 261 MHz) (Neutral)": {
                        "have_sub_menu": False,
                        "hotkey": "F2",
                        "submenu": {}
                    },
                    "Previous Menu": {
                        "have_sub_menu": False,
                        "hotkey": "F11",
                        "submenu": {}
                    },
                    "Exit": {
                        "have_sub_menu": False,
                        "hotkey": "F12",
                        "submenu": {}
                    }
                }
            }
        }
    }

    assert parser._parsed_dcs_menu == expected_menu_parsed

@mock.patch('ui.components.json_menu_composer.time.sleep', return_value=None)
def test_save_parsed_menus_dcs_error(_, parser):
    current_test_path = os.path.join(
        os.path.dirname(__file__), "test_config", "parsed_menus_dcs.json"
    )

    with mock.patch("ui.components.json_menu_composer.os.path.join") as os_path_mock:
        os_path_mock.return_value = current_test_path

        with (pytest.raises(TerrainNotDefinedError)):
            parser._save_parsed_menu()

@mock.patch('ui.components.json_menu_composer.time.sleep', return_value=None)
def test_save_parsed_menus_dcs(_, parser):
    current_test_path = os.path.join(
        os.path.dirname(__file__), "test_config", "parsed_menus_dcs.json"
    )

    with mock.patch("ui.components.json_menu_composer.os.path.join") as os_path_mock:
        os_path_mock.return_value = current_test_path
        parsed_menu_dcs = {"test": [1,2,3,4], "test2": 12}
        terrain = "Caucasus"
        parser._parsed_dcs_menu = parsed_menu_dcs
        parser._terrain = terrain
        parser._save_parsed_menu()

    created_file = json.load(open(current_test_path, 'r'))
    assert created_file == {"Caucasus": parsed_menu_dcs}