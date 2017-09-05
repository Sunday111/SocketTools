#include "WSA_Helpers.h"

namespace socket_tools
{
	void ThrowError(const char* msg, int error)
	{
		std::stringstream msgStream;
		msgStream << msg << error;
		throw std::runtime_error(msgStream.str());
	}

	void ThrowLastError(const char* msg)
	{
		ThrowError(msg, WSAGetLastError());
	}
}