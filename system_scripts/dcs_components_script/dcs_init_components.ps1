function Test-Admin {
    $currentUser = [Security.Principal.WindowsIdentity]::GetCurrent()
    $principal = New-Object Security.Principal.WindowsPrincipal($currentUser)
    return $principal.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)
}

# Relaunch the script with elevated privileges
if (-not (Test-Admin)) {
    $scriptPath = "C:\Users\rbblazquez\Desktop\dcs_tools\system_scripts\dcs_components_script\dcs_init_components.ps1"
    $arguments = "& '$scriptPath'"
    Start-Process powershell -Verb runAs -ArgumentList $arguments
    exit
}

# Define the list of COM ports
$comPorts = @(12,11, 10, 13,  22, 6, 49)

# Loop through each script and corresponding COM port
for ($i = 0; $i -lt 7; $i++) {
    $cmdScript = "C:\Users\rbblazquez\Desktop\dcs-bios-master\Programs\connect-serial-port.cmd"
    $comPort = $comPorts[$i]
    
    # Get the directory of the cmd script
    $scriptDirectory = [System.IO.Path]::GetDirectoryName($cmdScript)
    
    # Start a new terminal and run the .cmd script with the COM port as an argument
    Start-Process -FilePath "cmd.exe" -ArgumentList "/c cd `"$scriptDirectory`" && start cmd.exe /k `"$cmdScript $comPort`""
}

$path = "R:\DCS World\Config\MonitorSetup\wwtMonitor.lua"
$line = "FA_18C_RWR = { x = 1344, y = 1440, width = 720, height = 720 }"

# Check if the file exists
if (Test-Path -Path $path) {
    # Append the line to the file
    Add-Content -Path $path -Value $line
    Write-Output "Line appended successfully."
} else {
    Write-Error "File not found: $path"
}