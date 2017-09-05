#pragma once

#ifdef SOCKET_TOOLS_DYNAMIC_LIBRARY
	#define SOCKET_TOOLS_API __declspec(dllexport)
#else
	#define SOCKET_TOOLS_API __declspec(dllimport)
#endif