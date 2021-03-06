#include <iostream>

#include <SocketTools/ClientSocket.h>
#include <SocketTools/ServerSocket.h>
#include <SocketTools/SocketManager.h>

#include "RunServer.h"

namespace test_server
{
	void RunServer(const char* port)
	{
		using namespace socket_tools;

		SocketManager socketManager;

		ServerSocket serverSocket(
			SocketMode::Stream,
			Protocol::TCP,
			AddressFamily::Internet_v4,
			port);

		serverSocket.Listen();

		while (true)
		{
			// Accept a client socket
			auto clientSocket = serverSocket.Accept();

			// Receive until the peer shuts down the connection
			while (true)
			{
				try
				{
					constexpr auto defaultBuflen = 512;
					char recvbuf[defaultBuflen];

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
	}
}