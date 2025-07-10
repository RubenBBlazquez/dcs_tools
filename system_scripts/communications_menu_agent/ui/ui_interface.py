import tkinter as tk
from tkinter import ttk, messagebox
import keyboard
import threading
import socket
from ui.components.json_menu_composer import DCSMenuParser
from ui.config.settings_management import Settings
from ui.components.hotkey_selector import HotkeySelector
from ui.components.monitor_dropdown import MonitorDropdown
from ui.components.screen_capture_tool import ScreenCaptureTool


class DcsCommunicationsConfigView:
    def __init__(self):
        self.settings = Settings.create_settings_from_file()
        self.dcs_menu_parser = DCSMenuParser(self.settings)
        self.root = tk.Tk()
        self.root.title("Communications Menu Agent Configuration")
        self.root.geometry("435x330")
        self.root.resizable(False, False)

        self.monitor_dropdown = MonitorDropdown(
            parent=self.root,
            monitor_index=self.settings.monitor_index,
            on_change=self.settings.set_monitor_index
        )

        HotkeySelector(
            parent=self.root,
            label="Keyboard Key to start menu discovering",
            initial_value=self.settings.start_menu_discovery_hot_key,
            on_hotkey_change=self.settings.set_start_menu_discovery_hotkey
        )
        HotkeySelector(
            parent=self.root,
            label="Keyboard Key to recorder voice command",
            initial_value=self.settings.voice_recorder_hot_key,
            on_hotkey_change=self.settings.set_voice_recorder_hotkey
        )

        btn_frame = tk.Frame(self.root)
        btn_frame.pack(pady=15)
        ttk.Button(btn_frame, text="Retake Coordinates", command=self.retake_coords).pack(side=tk.LEFT, padx=5)
        ttk.Button(btn_frame, text="Start App", command=self.start_app).pack(side=tk.LEFT, padx=5)

        self.root.protocol("WM_DELETE_WINDOW", self.on_close)
        self.root.mainloop()

    def retake_coords(self):
        ScreenCaptureTool(self.settings)

    def stop_listening(self):
        if hasattr(self, 'hotkey_ref'):
            keyboard.remove_hotkey(self.hotkey_ref)
            print("Hotkey listener stopped.")

        if hasattr(self, '_udp_stop'):
            self._udp_stop.set()

        if self.modal:
            self.modal.destroy()
            self.modal = None

    def listen_udp_loop(self):
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sock.bind(("127.0.0.1", 55337))
        sock.settimeout(0.5)

        while not self._udp_stop.is_set():
            try:
                data, addr = sock.recvfrom(1024)
                msg = data.decode()
                print(f"UDP received from {addr}: {msg}")
            except socket.timeout:
                continue
            except Exception as e:
                print(f"UDP error: {e}")
                break

        sock.close()
        print("UDP listener stopped.")

    def start_app(self):
        coords = self.settings.last_capture_coordinates

        if not coords:
            messagebox.showerror("No Coordinates", "You must select a capture area first.")
            return

        self.modal = tk.Toplevel(self.root)
        self.modal.title("Listening for Hotkey & UDP")
        self.modal.geometry("400x180")
        self.modal.grab_set()

        tk.Label(self.modal, text=f"Listening for hotkey: {self.settings.start_menu_discovery_hot_key}",
                 font=("Arial", 12)).pack(pady=10)
        self.udp_status_label = tk.Label(self.modal, text="Waiting for UDP messages...", font=("Arial", 10))
        self.udp_status_label.pack(pady=10)

        ttk.Button(self.modal, text="Cancel Listening", command=self.stop_listening).pack(pady=10)
        self.modal.protocol("WM_DELETE_WINDOW", self.stop_listening)

        try:
            self.hotkey_ref = keyboard.add_hotkey(
                self.settings.start_menu_discovery_hot_key,
                self.dcs_menu_parser.discovery_scan_dcs_menu_json
            )
        except Exception as e:
            messagebox.showerror("Hotkey Error", f"Failed to register hotkey: {e}")
            self.modal.destroy()
            return

        self._udp_stop = threading.Event()
        self.udp_thread = threading.Thread(target=self.listen_udp_loop, daemon=True)
        self.udp_thread.start()

    def on_close(self):
        self.settings.save_settings_in_file()
        self.root.destroy()

    def __del__(self):
        self.settings.save_settings_in_file()

if __name__ == "__main__":
    DcsCommunicationsConfigView()
