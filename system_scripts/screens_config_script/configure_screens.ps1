# Ruta al ejecutable de MultiMonitorTool
$multiMonitorToolPath = "C:\Users\rbblazquez\Desktop\dcs_tools\system_scripts\screens_config_script\MultiMonitorTool.exe"

# Ruta al archivo de configuración guardado
$configFilePath = "C:\Users\rbblazquez\Desktop\dcs_tools\system_scripts\screens_config_script\saved_screens_layout.dat"

# Comando para cargar la configuración guardada
Start-Process -FilePath $multiMonitorToolPath -ArgumentList "/LoadConfig $configFilePath"


python "C:\Users\rbblazquez\Desktop\dcs_tools\system_scripts\screens_config_script\open_kneeboard_in_screen.py"
