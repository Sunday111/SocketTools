#include <SocketTools/SocketManager.h>

namespace socket_tools
{
	SocketManager::SocketManager()
	{
		// Initialize Winsock
		auto result = WSAStartup(MAKEWORD(2, 2), &wsaData);

		if (result == 0)
		{
			return;
		}

		std::stringstream stream;
		stream << "WSAStartup failed with error: " << result;
		throw std::runtime_error(stream.str());
	}

	SocketManager::~SocketManager()
	{
		WSACleanup();
	}
}