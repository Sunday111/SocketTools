#include <SocketTools/Socket.h>

#include "WSA_Helpers.h"

namespace socket_tools
{
	Socket::Socket(SOCKET socket) :
		m_socket(socket)
	{}

	Socket::Socket(Socket&& that)
	{
		MoveFrom(that);
	}

	Socket::~Socket()
	{
		if (SocketIsValid())
		{
			closesocket(m_socket);
		}
	}

	void Socket::Shutdown()
	{
		CheckSocket();

		// shutdown the connection since no more data will be sent
		auto error = shutdown(m_socket, SD_SEND);

		if (error == SOCKET_ERROR)
		{
			ThrowLastError("shutdown failed with error: ");
		}
	}

	bool Socket::SocketIsValid() const
	{
		return m_socket != INVALID_SOCKET;
	}

	Socket& Socket::operator=(Socket&& that)
	{
		Close();
		MoveFrom(that);
		return *this;
	}

	void Socket::CheckSocket() const
	{
		if (!SocketIsValid())
		{
			throw std::runtime_error("Invalid socket");
		}
	}

	void Socket::MoveFrom(Socket& that)
	{
		m_socket = that.m_socket;
		that.m_socket = INVALID_SOCKET;
	}

	void Socket::Close()
	{
		if (SocketIsValid())
		{
			closesocket(m_socket);
		}
	}
}