import pytest
from unittest.mock import patch, MagicMock

from ui.components.json_menu_composer import DCSMenuParser


@pytest.fixture
def parser(tmp_path):
    class DummySettings:
        last_capture_coordinates = {"x1": 0, "y1": 0, "x2": 10, "y2": 10}

    with patch.object(DCSMenuParser, '__attrs_post_init__', return_value=None):
        yield DCSMenuParser(DummySettings())

def test_iterate_and_parse_dcs_menu_double_run(parser):
    first_ocr = ["Main", "ATC..."]
    second_ocr = [
        '2. Main. ATC',
        'F1. [N/A] Kutaisi (4.4 / 134 / 41 / 263 MHz)...',
        'F2. [N/A] Senaki-Kolkhi (4.3 / 132 / 40.6 / 261 MHz) (Neutral)...',
        'F3. [N/A] Kobuleti (4.35 / 133 / 40.8/ 262 MHz) (Neutral)..',
        'F4. [N/A] Batumi (4.25 / 131 / 40.4 / 260 MHz) (Neutral)...',
        'F5 [N/A] Sukhumi-Babushara (4.15 12940258 MHz)..',
        'F6. [N/A] Nalchik (4.5/136 / 41.4/ 265 MHz) (Neutral)...',
        'F7. [N/A] Gudauta (4.2/ 130 / 40.2 / 259 MHz) (Neutral)...',
        'F8. [N/A] Beslan (4,75 / 141/42.4/ 270 MHz) (Neutral)...',
        'F9. [N/A] Tbilisi-Lochini (4.6 / 138/ 41.8 / 267 MHz) (Neutral)...',
        'F10. [N/A] Soganlug (4.65/139/42/ 268 MHz) (Neutral).',
        'F11, Previous Menu',
        'F12, Exit'
    ]

    with patch.object(parser, "_parse_image_with_ocr", side_effect=[first_ocr, second_ocr]):
        with patch.object(parser, "_get_dcs_menu_screenshot", return_value=b''):
            parser._edit_specific_deep_key = MagicMock()
            parser.iterate_and_parse_dcs_menu()
            parser.iterate_and_parse_dcs_menu()
            breakpoint()
            calls = parser._edit_specific_deep_key.call_args_list
            assert calls  # There should be calls to _edit_specific_deep_key
            # Optionally check the arguments for expected menu parsing
            first_call_args = calls[0][0][0]  # menu_title passed to edit method on first call
            assert first_call_args == ["menu"] or first_call_args == first_ocr[0]
            # Print or further check deeper call structure if desired
