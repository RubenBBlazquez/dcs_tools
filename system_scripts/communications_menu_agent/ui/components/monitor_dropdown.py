import tkinter as tk
from tkinter import ttk


class MonitorDropdown:
    def __init__(self, parent, monitor_index=0, on_change=None):
        import mss

        self.on_change = on_change

        with mss.mss() as sct:
            self.monitors = sct.monitors[1:]

        ttk.Label(parent, text="Choose monitor:").pack(pady=(10, 2))

        self.monitor_var = tk.StringVar()
        self.dropdown = ttk.Combobox(
            parent,
            values=[f"Monitor {i + 1} ({m['width']}x{m['height']})" for i, m in enumerate(self.monitors)],
            textvariable=self.monitor_var,
            state="readonly"
        )
        self.dropdown.pack(padx=10, pady=5)
        self.dropdown.current(monitor_index)

        self.dropdown.bind("<<ComboboxSelected>>", self._on_selection_change)

    def _on_selection_change(self, event):
        if self.on_change:
            self.on_change(self.get_selected_index())

    def get_selected_index(self) -> int:
        return self.dropdown.current()