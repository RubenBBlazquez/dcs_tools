import mss
import tkinter as tk

from ui.config.settings_management import Settings


class ScreenCaptureTool:
    """
    Class in charge to establish the settings information
    of the area where we are going to take the screenshots
    to do the menu discovering

    Parameters
    ----------
    settings: Settings
        Object with all configurations of the ui app
    """
    _settings: Settings

    def __init__(self, settings: Settings):
        self._settings = settings

        with mss.mss() as sct:
            self.monitor = sct.monitors[settings.monitor_index]

        self.offset_x = self.monitor["left"]
        self.offset_y = self.monitor["top"]
        self.width = self.monitor["width"]
        self.height = self.monitor["height"]

        self.start_x = self.start_y = 0
        self.rect = None

        self.root = tk.Tk()
        self.root.overrideredirect(True)
        self.root.geometry(f"{self.width}x{self.height}+{self.offset_x}+{self.offset_y}")
        self.root.attributes("-alpha", 0.3)
        self.root.attributes("-topmost", True)
        self.root.configure(bg='black')

        self.canvas = tk.Canvas(self.root, cursor="cross", bg='grey11')
        self.canvas.pack(fill=tk.BOTH, expand=True)

        self.canvas.bind("<ButtonPress-1>", self.on_mouse_down)
        self.canvas.bind("<B1-Motion>", self.on_mouse_drag)
        self.canvas.bind("<ButtonRelease-1>", self.on_mouse_up)

        self.root.mainloop()

    def on_mouse_down(self, event):
        self.start_x = self.canvas.canvasx(event.x)
        self.start_y = self.canvas.canvasy(event.y)
        self.rect = self.canvas.create_rectangle(
            self.start_x, self.start_y, self.start_x, self.start_y,
            outline='red', width=2
        )

    def on_mouse_drag(self, event):
        cur_x = self.canvas.canvasx(event.x)
        cur_y = self.canvas.canvasy(event.y)
        self.canvas.coords(self.rect, self.start_x, self.start_y, cur_x, cur_y)

    def on_mouse_up(self, event):
        end_x = self.canvas.canvasx(event.x)
        end_y = self.canvas.canvasy(event.y)
        self.root.destroy()

        x1 = int(min(self.start_x, end_x) + self.offset_x)
        y1 = int(min(self.start_y, end_y) + self.offset_y)
        x2 = int(max(self.start_x, end_x) + self.offset_x)
        y2 = int(max(self.start_y, end_y) + self.offset_y)

        self._settings.last_capture_coordinates = {
            "x1": x1,
            "y1": y1,
            "x2": x2,
            "y2": y2
        }
        print(f"Coordinates saved: ({x1}, {y1}, {x2}, {y2})")