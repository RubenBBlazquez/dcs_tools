from tkinter import ttk
import tkinter as tk
import keyboard


class HotkeySelector:
    def __init__(self, parent, label: str, initial_value: str = "", on_hotkey_change=None):
        self.on_hotkey_change = on_hotkey_change
        self.hotkey_var = tk.StringVar(value=initial_value)

        ttk.Label(parent, text=label).pack(pady=(10, 2))
        self.entry = ttk.Entry(parent, textvariable=self.hotkey_var, state='readonly')
        self.entry.pack(padx=10, pady=5)

        ttk.Button(parent, text="Select Hotkey", command=self.capture_hotkey).pack(pady=5)

    def capture_hotkey(self):
        self.hotkey_var.set("")
        popup = tk.Toplevel()
        popup.title("Select Hotkey")
        popup.geometry("300x100")
        popup.resizable(False, False)
        popup.grab_set()

        ttk.Label(popup, text="Press your key combination...").pack(pady=20)

        pressed_keys = set()
        recorded = []

        def on_event(e):
            nonlocal recorded
            if e.event_type == 'down':
                pressed_keys.add(e.name)
            elif e.event_type == 'up':
                if pressed_keys:
                    combo = '+'.join(sorted(pressed_keys))
                    recorded = combo
                    keyboard.unhook_all()
                    self.hotkey_var.set(recorded)
                    popup.destroy()

                    if self.on_hotkey_change:
                        self.on_hotkey_change(recorded)

        keyboard.hook(on_event)
        popup.mainloop()

    def get_value(self):
        return self.hotkey_var.get()

    def set_value(self, value):
        self.hotkey_var.set(value)