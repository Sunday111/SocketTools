#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>

#include <cstdint>

#include <SocketTools/Common.h>

namespace socket_tools
{
	class SOCKET_TOOLS_API ClientSocket
	{
	public:
		ClientSocket(SOCKET socket);
		ClientSocket(const char* address, const char* port);
		~ClientSocket();

		template<typename T, size_t size>
		void Send(const T(&data)[size])
		{
			Send(
				reinterpret_cast<const uint8_t*>(data),
				size * sizeof(T));
		}

		template<typename T, size_t size>
		void Receive(T(&data)[size])
		{
			Receive(
				reinterpret_cast<uint8_t*>(data),
				size * sizeof(T));
		}

		void Send(const uint8_t* data, size_t size);
		void Receive(uint8_t* data, size_t size);

		void Shutdown();

	private:
		SOCKET m_socket;
	};
}