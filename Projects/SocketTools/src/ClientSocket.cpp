#include <ws2tcpip.h>
#include <cassert>
#include <sstream>
#include <SocketTools/ClientSocket.h>

namespace socket_tools
{
	namespace
	{
		void ThrowLastError(const char* msg)
		{
			std::stringstream msgStream;
			msgStream << msg << WSAGetLastError();
			throw std::runtime_error(msgStream.str());
		}

		struct Address
		{
			Address(const char* address, const char* port) :
				info(nullptr)
			{
				ZeroMemory(&hints, sizeof(hints));
				hints.ai_family = AF_UNSPEC;
				hints.ai_socktype = SOCK_STREAM;
				hints.ai_protocol = IPPROTO_TCP;

				// Resolve the server address and port
				auto result = getaddrinfo(address, port, &hints, &info);

				if (result == 0)
				{
					return;
				}

				std::stringstream msgStream;
				msgStream << "getaddrinfo failed with error: " << result;
				throw std::runtime_error(msgStream.str());
			}

			~Address()
			{
				if (info == nullptr)
				{
					return;
				}

				freeaddrinfo(info);
			}


			addrinfo hints;
			addrinfo* info;
		};
	}

	ClientSocket::ClientSocket(SOCKET socket) :
		m_socket(socket)
	{}

	ClientSocket::ClientSocket(const char* address, const char* port) :
		m_socket(INVALID_SOCKET)
	{
		Address addr(address, port);

		// Attempt to connect to an address until one succeeds
		for (auto info = addr.info; info != nullptr; info = info->ai_next)
		{
			// Create a SOCKET for connecting to server
			m_socket = ::socket(info->ai_family, info->ai_socktype, info->ai_protocol);

			if (m_socket == INVALID_SOCKET)
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

	ClientSocket::~ClientSocket()
	{
		if (m_socket == INVALID_SOCKET)
		{
			return;
		}

		closesocket(m_socket);
	}

	void ClientSocket::Send(const uint8_t* data, size_t size)
	{
		assert(m_socket != INVALID_SOCKET);
		auto error = send(m_socket, (const char*)data, static_cast<int>(size), 0);

		if (error != SOCKET_ERROR)
		{
			return;
		}

		ThrowLastError("send failed with error: ");
	}

	void ClientSocket::Receive(uint8_t* data, size_t size)
	{
		assert(m_socket != INVALID_SOCKET);

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

	void ClientSocket::Shutdown()
	{
		// shutdown the connection since no more data will be sent
		auto error = shutdown(m_socket, SD_SEND);

		if (error == SOCKET_ERROR)
		{
			ThrowLastError("shutdown failed with error: ");
		}
	}
}