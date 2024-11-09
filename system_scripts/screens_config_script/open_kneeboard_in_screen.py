import subprocess
import time
import pygetwindow as gw

if __name__ == '__main__':

    # Path to the executable
    exe_path = r"C:\Program Files\OpenKneeboard\bin\OpenKneeboardApp.exe"

    # Start the application
    subprocess.Popen(exe_path)

    # Wait for a few seconds to ensure the application window has opened
    time.sleep(2)

    # Find the window by title (you may need to adjust the title based on what OpenKneeboard's window is named)
    windows = gw.getWindowsWithTitle('OpenKneeboard')  # Replace 'OpenKneeboard' with the actual window title
    if windows:
        window = windows[0]

        # Move the window to the desired coordinates and resize it
        window.moveTo(1920, 20)
        window.resizeTo(600, 1024)
    else:
        print("Window not found.")
