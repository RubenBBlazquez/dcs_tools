# Start monitoring for the specified COM port
$comPort = $args[0]
if (-not $comPort) {
    Write-Host "COM port not specified."
    exit 1
}


# Function to turn RTS signal off
function TurnRtsOff($portName) {
    try {
        $port = New-Object System.IO.Ports.SerialPort $portName, 115200

        # Open the serial port
        $port.Open()

        # Turn off RTS signal
        $port.RtsEnable = $false

        # Close the serial port
        $port.Close()
    } catch {
        Write-Host "Error turning off RTS signal: $_.Exception.Message"
    }
}

function IsCom29Connected($portName) {
    $comPorts = [System.IO.Ports.SerialPort]::GetPortNames()
	foreach ($port in $comPorts) {
		Write-Host "COM Port: $($port)"
	}
    return $comPorts -contains $portName
}

# Function to continuously check for connection of COM29
function MonitorCom29Connection($comPort) {
    while ($true) {
        try {
            $com29Connected = IsCom29Connected $comPort
            if ($com29Connected) {
                Write-Host "COM connected. Turning off RTS signal..."
                TurnRtsOff $comPort
                Write-Host "RTS signal turned off."
                break 
            }
        } catch {
            Write-Host "Error checking COM29 connection: $_.Exception.Message"
        }
		
		Write-Host "port not found"
        Start-Sleep -Seconds 5  # Check every 5 seconds
    }
}

Write-Host "COM$COMPORT"
$comPort = "COM$COMPORT"
# Start monitoring
MonitorCom29Connection $comPort