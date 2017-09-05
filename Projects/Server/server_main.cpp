#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include <SocketTools/ClientSocket.h>
#include <SocketTools/SocketManager.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

int __cdecl main(void)
{
	constexpr auto defaultBuflen = 512;
	constexpr auto defaultPort = "27015";

	int iResult;

	SOCKET ListenSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL;
	struct addrinfo hints;

	char recvbuf[defaultBuflen];

	using namespace socket_tools;
	SocketManager socketManager;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, defaultPort, &hints, &result);
	if (iResult != 0)
	{
		printf("getaddrinfo failed with error: %d\n", iResult);
		return 1;
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (ListenSocket == INVALID_SOCKET)
	{
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		return 1;
	}

	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);

	if (iResult == SOCKET_ERROR)
	{
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		return 1;
	}

	freeaddrinfo(result);

	iResult = listen(ListenSocket, SOMAXCONN);

	if (iResult == SOCKET_ERROR)
	{
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		return 1;
	}

	while (true)
	{
		// Accept a client socket
		auto clientSocketHandle = accept(ListenSocket, NULL, NULL);

		if (clientSocketHandle == INVALID_SOCKET)
		{
			printf("accept failed with error: %d\n", WSAGetLastError());
			closesocket(ListenSocket);
			return 1;
		}

		ClientSocket clientSocket(clientSocketHandle);

		// Receive until the peer shuts down the connection
		while (true)
		{
			try
			{
				clientSocket.Receive(recvbuf);
				std::cout << "Message: " << recvbuf << std::endl;
				clientSocket.Send(recvbuf);
			}
			catch (const std::exception& ex)
			{
				std::cout << ex.what() << std::endl;
				break;
			}
		}

		// shutdown the connection since we're done
		clientSocket.Shutdown();
	}

	// No longer need server socket
	closesocket(ListenSocket);

	return 0;
}