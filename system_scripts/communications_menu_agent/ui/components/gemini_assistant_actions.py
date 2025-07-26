from enum import Enum
from typing import Any
from langchain_google_genai import ChatGoogleGenerativeAI

class MenuItem(Enum):
    LABEL = "label"
    KEY = "hotkey"

def check_menu_item_most_likely(menu: dict[str, Any], key: str, type_: MenuItem = MenuItem.LABEL):
    """
    Determines the most likely menu item based on a provided JSON menu structure,
    a key to identify the target within the menu, and a type to indicate the value
    of interest (either the label or the hotkey associated with the key).

    This function uses a large language model to infer and extract the most
    likely key or label match based on the provided inputs, following specific
    interpretation rules regarding the type. For `type_` set as "label", the name
    of the item is returned; for "hotkey", the hotkey string (e.g., "F1", "F2")
    is returned. It also logs the received key alongside the inferred corrected
    key for tracking.

    :param menu: A dictionary mapping hotkeys or other identifiers to menu item
        definitions. Represents the structure of the interactive menu where each
        item may have a "label" and/or "hotkey".
    :type menu: dict[str, Any]
    :param key: A string representing the user-provided key used to search for
        the most likely match in the menu.
    :type key: str
    :param type_: Enum `MenuItem` specifying the type of information to extract
        (label vs hotkey) from the identified menu item.
    :type type_: MenuItem
    :return: A string determined by the type (`type_`), representing either the
        label or hotkey most closely associated with the provided key.
    :rtype: str
    """
    llm = ChatGoogleGenerativeAI(
        model="gemini-2.5-flash-lite-preview-06-17",
        temperature=0,
        max_tokens=None,
        timeout=None,
        max_retries=2,
    )
    messages = [
        (
            "system",
            "The user will enter a json and a key, you must find which is "
            " the most likely key inside the json of the key given,"
            " the structure you will receive: json: {json} - key: {key} - type: {label|hotkey}"
            "if the type is hotkey you will return the hotkey, F5, F2, etc nothing more,"
            " if its label you return the name of the item but no the hotkey,"
            "for example if you have something like this"
            "{ 'F3': { 'label': 'Request Repair', 'have_sub_menu': false, 'submenu': {}}"
            "you will return 'Request Repair' and you have type 'label' you will return 'Request Repair'"
            " if its 'hotkey' you will return 'F3'"
        ),
        ("human", f"json: {menu} - key: {key} - type: {type_.value}"),
    ]
    response = llm.invoke(messages)
    corrected_key = response.content
    print(f"key received: {key} - corrected key: {corrected_key}")
    return corrected_key