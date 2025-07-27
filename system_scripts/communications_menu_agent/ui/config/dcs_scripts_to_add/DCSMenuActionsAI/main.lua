local last_message_send = nil
local terrain = "UNKNOWN"
local last_real_time = 0

ai_menu_actions = {
  Start = function(self)
    package.path = package.path .. ";.\\LuaSocket\\?.lua"
    package.cpath = package.cpath .. ";.\\LuaSocket\\?.dll"
    socket = require("socket")
    local connect_init =
      socket.protect(
      function()
        last_message_send = nil
        udp_client = socket.try(socket.udp())
        socket.try(udp_client:settimeout(.001))
        socket.try(udp_client:setpeername("127.0.0.1", 55337))
        socket.try(udp_client:send("------ START UDP DCSMenuActionAI ------"))    
        lastTime = 0
        -- log.write('DCSMENUACTIONSAI.EXPORT.LUA',log.ERROR, "DEBUG")
      end
    )
    connect_init()
  end,
  AfterNextFrame = function(self)
	local data_send =
	socket.protect(
		function()
			local mech = LoGetMechInfo()

      local gear_value = -1.0

      if mech and mech.gear then
          gear_value = mech.gear.value
      end

      local tempdir = lfs.tempdir()
      local filepath = tempdir .. "/terrain_name.txt"
      local f = io.open(filepath, "r")

      if f then
          terrain = f:read("*l") or terrain
          f:close()
          os.remove(filepath)
      end

      local message = string.format("'landing_gear_state': %.2f , 'terrain': '%s'", gear_value, terrain)
      local json_format_result = "{" .. message .. "}"
      log.write('DCSMENUACTIONSAI.EXPORT.LUA', log.ERROR, json_format_result)

      local must_send_message = false
      local realTime =  os.clock()

      if json_format_result ~= last_message_send then
          must_send_message = true
      elseif (realTime - last_real_time) >= 10.0 then
          must_send_message = true
      end

      if udp_client and must_send_message then
          udp_client:send(json_format_result)
          last_message_send = json_format_result
          last_real_time = realTime
      end
  	end
	)
    data_send()
  end,
  Stop = function(self)
    local connection_close =
      socket.protect(
      function()
        last_message_send = nil
        if udp_client then
          socket.try(udp_client:send("------ END UDP DCSMenuActionAI ------"))
          udp_client:close()
        end
      end
    )
    connection_close()
  end
}

----------------------------------------------------------------------------------------------------
--http://forums.eagle.ru/showpost.php?p=2431726&postcount=5
-- Works before mission start
do
  local SimLuaExportStart = LuaExportStart
  LuaExportStart = function()
    ai_menu_actions:Start()
    if SimLuaExportStart then
      SimLuaExportStart()
    end
  end
end

-- Works after every simulation frame
do
  local SimLuaExportAfterNextFrame = LuaExportAfterNextFrame
  LuaExportAfterNextFrame = function()
    ai_menu_actions:AfterNextFrame()
    if SimLuaExportAfterNextFrame then
      SimLuaExportAfterNextFrame()
    end
  end
end

-- Works after mission stop
do
  local SimLuaExportStop = LuaExportStop
  LuaExportStop = function()
    ai_menu_actions:Stop()
    if SimLuaExportStop then
      SimLuaExportStop()
    end
  end
end
