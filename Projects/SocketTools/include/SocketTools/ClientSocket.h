#pragma once

#include <cstdint>

#include <SocketTools/Socket.h>

namespace socket_tools
{
	class SOCKET_TOOLS_API ClientSocket :
		public Socket
	{
	public:
		using Socket::Socket;

		ClientSocket(
			SocketMode mode,
			Protocol protocol,
			AddressFamily family,
			const char* address,
			const char* port);

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
	};
}