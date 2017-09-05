#include "WSA_Helpers.h"
#include "Address.h"

#include <SocketTools/ServerSocket.h>

namespace socket_tools
{
	ServerSocket::ServerSocket(
		SocketMode mode,
		Protocol protocol,
		AddressFamily family,
		const char* port) :
		Socket(INVALID_SOCKET)
	{
		Address addr(mode, protocol, family, nullptr, port, AI_PASSIVE);
	
		m_socket = socket(
			addr.info->ai_family,
			addr.info->ai_socktype,
			addr.info->ai_protocol);
	
		if (m_socket == INVALID_SOCKET)
		{
			ThrowLastError("socket failed with error: ");
		}
	
		// Setup the TCP listening socket
		if (bind(m_socket, addr.info->ai_addr, (int)addr.info->ai_addrlen) == SOCKET_ERROR)
		{
			ThrowLastError("bind failed with error: ");
		}
	}

	void ServerSocket::Listen()
	{
		CheckSocket();

		if (listen(m_socket, SOMAXCONN) == SOCKET_ERROR)
		{
			ThrowLastError("listen failed with error: ");
		}
	}

	ClientSocket ServerSocket::Accept()
	{
		CheckSocket();
		return ClientSocket(accept(m_socket, NULL, NULL));
	}
}