#pragma once

#include <sstream>
#include <stdexcept>
#include <winsock2.h>

#include <SocketTools/Common.h>

namespace socket_tools
{
	class SOCKET_TOOLS_API SocketManager
	{
	public:
		SocketManager();
		~SocketManager();

	private:
		WSADATA wsaData;
	};
}