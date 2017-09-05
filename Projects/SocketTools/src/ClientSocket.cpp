#include <ws2tcpip.h>
#include <cassert>
#include <sstream>
#include <SocketTools/ClientSocket.h>

#include "WSA_Helpers.h"
#include "Address.h"

namespace socket_tools
{
	ClientSocket::ClientSocket(
		SocketMode mode,
		Protocol protocol,
		AddressFamily family,
		const char* address,
		const char* port) :
		Socket(INVALID_SOCKET)
	{
		Address addr(mode, protocol, family, address, port);

		// Attempt to connect to an address until one succeeds
		for (auto info = addr.info; info != nullptr; info = info->ai_next)
		{
			// Create a SOCKET for connecting to server
			m_socket = socket(info->ai_family, info->ai_socktype, info->ai_protocol);

			if (!SocketIsValid())
			{
				ThrowLastError("socket failed with error: ");
			}

			// Connect to server.
			auto error = connect(m_socket, info->ai_addr, (int)info->ai_addrlen);

			if (error != SOCKET_ERROR)
			{
				return;
			}

			closesocket(m_socket);
			m_socket = INVALID_SOCKET;
		}

		throw std::runtime_error("Unable to connect to server!");
	}

	void ClientSocket::Send(const uint8_t* data, size_t size)
	{
		CheckSocket();

		auto error = send(m_socket, (const char*)data, static_cast<int>(size), 0);

		if (error != SOCKET_ERROR)
		{
			return;
		}

		ThrowLastError("send failed with error: ");
	}

	void ClientSocket::Receive(uint8_t* data, size_t size)
	{
		CheckSocket();

		auto res = recv(m_socket, (char*)data, static_cast<int>(size), 0);

		if (res > 0)
		{
			return;
		}

		if (res == 0)
		{
			throw std::runtime_error("Connection closed");
		}

		ThrowLastError("recv failed with error: ");
	}
}