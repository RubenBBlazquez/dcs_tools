from typing import Any
from langchain_google_genai import ChatGoogleGenerativeAI

import os


def check_menu_key_most_likely(menu: dict[str, Any], key: str, type_: str = "label"):
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
        ("human", f"json: {menu} - key: {key} - type: {type_}"),
    ]
    response = llm.invoke(messages)
    corrected_key = response.content
    print(f"key received: {key} - corrected key: {corrected_key}")
    return corrected_key