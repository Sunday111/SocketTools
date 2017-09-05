#pragma once

#include <SocketTools/ClientSocket.h>
#include <SocketTools/Socket.h>

namespace socket_tools
{
	class SOCKET_TOOLS_API ServerSocket :
		public Socket
	{
	public:
		ServerSocket(
			SocketMode mode,
			Protocol protocol,
			AddressFamily family,
			const char* port);

		void Listen();
		ClientSocket Accept();
	};
}