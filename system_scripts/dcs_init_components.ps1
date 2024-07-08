# PowerShell script to run .cmd scripts with predefined COM ports

# Define the list of COM ports
$comPorts = @(22, 47, 48, 49)

# Define the paths to your .cmd scripts
$cmdScripts = @("C:\Users\rbblazquez\Desktop\dcs-bios-master\Programs\connect-serial-port.cmd", "C:\Users\rbblazquez\Desktop\dcs-bios-master\Programs\connect-serial-port.cmd", "C:\Users\rbblazquez\Desktop\dcs-bios-master\Programs\connect-serial-port.cmd", "C:\Users\rbblazquez\Desktop\dcs-bios-master\Programs\connect-serial-port.cmd")

# Ensure the number of COM ports and scripts match
if ($comPorts.Count -ne $cmdScripts.Count) {
    Write-Error "Number of COM ports and .cmd scripts must match"
    exit 1
}

# Loop through each script and corresponding COM port
for ($i = 0; $i -lt $cmdScripts.Count; $i++) {
    $cmdScript = $cmdScripts[$i]
    $comPort = $comPorts[$i]
    
    # Get the directory of the cmd script
    $scriptDirectory = [System.IO.Path]::GetDirectoryName($cmdScript)
    
    # Start a new terminal and run the .cmd script with the COM port as an argument
    Start-Process -FilePath "cmd.exe" -ArgumentList "/c cd `"$scriptDirectory`" && start cmd.exe /k `"$cmdScript $comPort`""
}