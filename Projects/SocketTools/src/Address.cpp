#include <SocketTools/Socket.h>
#include "Address.h"

namespace socket_tools
{
	namespace
	{
		int SocketModeToSocketType(SocketMode mode)
		{
			switch (mode)
			{
				case socket_tools::SocketMode::Stream:
					return SOCK_STREAM;

				case socket_tools::SocketMode::Datagram:
					return SOCK_DGRAM;
			}

			throw std::invalid_argument("Invalid socket type");
		}

		int ProtocolToIPPROTO(Protocol protocol)
		{
			switch (protocol)
			{
				case socket_tools::Protocol::TCP:
					return IPPROTO_TCP;
			}

			throw std::invalid_argument("Invalid protocol");
		}

		int AdressFamilyToAfFamily(AddressFamily family)
		{
			switch (family)
			{
				case socket_tools::AddressFamily::Unspecified:
					return AF_UNSPEC;

				case socket_tools::AddressFamily::Internet_v4:
					return AF_INET;
			}

			throw std::invalid_argument("Invalid address family");
		}
	}

	Address::Address(
		SocketMode mode,
		Protocol protocol,
		AddressFamily family,
		const char* address,
		const char* port,
		int flags) :
		info(nullptr)
	{
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AdressFamilyToAfFamily(family);
		hints.ai_socktype = SocketModeToSocketType(mode);
		hints.ai_protocol = ProtocolToIPPROTO(protocol);
		hints.ai_flags = flags;

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

	Address::~Address()
	{
		if (info == nullptr)
		{
			return;
		}

		freeaddrinfo(info);
	}
}