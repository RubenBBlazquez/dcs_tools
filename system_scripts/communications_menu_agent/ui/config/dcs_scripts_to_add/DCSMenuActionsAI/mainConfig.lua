module("MainConfig", package.seeall)

--- @class TCPConnectionConfig
--- @field address string
--- @field port integer

--- @class UDPConnectionConfig
--- @field send_address string
--- @field send_port integer
--- @field receive_address string
--- @field receive_port integer

--- @class BIOSConfig
--- @field tcp_config TCPConnectionConfig[]
--- @field udp_config UDPConnectionConfig[]
--- @field dev_mode boolean whether dev mode is enabled and json/header files should be written
local BIOSConfig = {
	tcp_config = {
		{
			address = "*",
			port = 7779,
		},
	},
	udp_config = {
		{
			send_address = "127.0.0.1",
			send_port = 5011,
			receive_address = "*",
			receive_port = 7779,
		},
	},
	dev_mode = true,
}

return BIOSConfig
