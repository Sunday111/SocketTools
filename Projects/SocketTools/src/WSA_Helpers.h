#pragma once

#include <sstream>
#include <winsock2.h>

namespace socket_tools
{
	void ThrowError(const char* msg, int error);
	void ThrowLastError(const char* msg);
}