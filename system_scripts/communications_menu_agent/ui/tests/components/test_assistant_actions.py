import pytest
from ui.components.gemini_assistant_actions import check_menu_key_most_likely
from ui.config.default_communications.EN.en_parsed import DEFAULT_EN_MENU

@pytest.mark.vcr()
def test_gemini():
    assert check_menu_key_most_likely(
        DEFAULT_EN_MENU,
        "Gr0und A1s Suppli"
    ).content == "Ground Air Supply..."