local udp_client = nil
package.path = package.path .. ";.\\LuaSocket\\?.lua"
package.cpath = package.cpath .. ";.\\LuaSocket\\?.dll"
local socket = require("socket")
local callbacks = {}

function l(msg)
  log.write("DCSMENUACTIONSAI", log.INFO, msg)
end

function sendMessage(message, messageType)
    local json_format_result = "{'" .. messageType .. "' :" .. message .. "}"
    udp_client:send(json_format_result)
end

function callbacks.onSimulationStart()
    l("onSimulationStart")
    udp_client = socket.try(socket.udp())
    socket.try(udp_client:settimeout(.001))
    socket.try(udp_client:setpeername("127.0.0.1", 55337))
    socket.try(udp_client:send("------ START UDP DCSMenuActionAI ------"))

    local current_mission = DCS.getCurrentMission()

    if current_mission and current_mission.mission and current_mission.mission.theatre then
        local tempdir = lfs.tempdir()
        local filepath = tempdir .. "/terrain_name.txt"
        local f = io.open(filepath, "w")
        l(current_mission.mission.theatre)

        if f then
           f:write(current_mission.mission.theatre)
           f:close()
        end
    end
end

function callbacks.onRadioMessage(message, duration)
  sendMessage(message, "radio")
end

DCS.setUserCallbacks(callbacks)

l("Callbacks installed")
