#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>

#include <SocketTools/Common.h>

namespace socket_tools
{
	enum class Protocol
	{
		TCP
	};

	enum class SocketMode
	{
		Stream,
		Datagram
	};

	enum class AddressFamily
	{
		Unspecified,
		Internet_v4
	};

	class SOCKET_TOOLS_API Socket
	{
	public:
		Socket(Socket&&);
		Socket(const Socket&) = delete;
		Socket(SOCKET socket);
		~Socket();

		void Shutdown();
		bool SocketIsValid() const;

		Socket& operator=(Socket&& that);
		Socket& operator=(const Socket&) = delete;

	protected:
		void CheckSocket() const;
		void MoveFrom(Socket& that);
		void Close();

		SOCKET m_socket;
	};
}