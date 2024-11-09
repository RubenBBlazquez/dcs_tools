
# Start the first PowerShell script in a new terminal
Start-Process powershell.exe -ArgumentList "-File", "C:\Users\rbblazquez\Desktop\dcs_tools\system_scripts\screens_config_script\configure_screens.ps1" -NoNewWindow

# Start the second PowerShell script in another new terminal
Start-Process powershell.exe -ArgumentList "-File", "C:\Users\rbblazquez\Desktop\dcs_tools\system_scripts\dcs_components_script\dcs_init_components.ps1"
