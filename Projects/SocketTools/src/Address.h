#pragma once

#include "WSA_Helpers.h"
#include <ws2tcpip.h>
#include <SocketTools/Socket.h>

namespace socket_tools
{
	class Address
	{
	public:
		Address(
			SocketMode mode,
			Protocol protocol,
			AddressFamily family,
			const char* address,
			const char* port,
			int flags = 0);
		~Address();

		addrinfo hints;
		addrinfo* info;
	};
}