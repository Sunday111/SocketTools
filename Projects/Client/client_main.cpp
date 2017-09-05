#include <iostream>
#include <SocketTools/ClientSocket.h>
#include <SocketTools/SocketManager.h>

int __cdecl main(int argc, char **argv)
{
	constexpr auto defaultBuflen = 512;
	constexpr auto defaultPort = "27015";

	char sendbuf[] = "This is a test";
	char recvbuf[defaultBuflen];

	if (argc != 2)
	{
		std::cout << "usage: " << argv[0] << " server-name" << std::endl;
		return 1;
	}

	using namespace socket_tools;

	SocketManager socketManager;

	ClientSocket socket(
		SocketMode::Stream,
		Protocol::TCP,
		AddressFamily::Unspecified,
		argv[1],
		defaultPort);

	socket.Send(sendbuf);
	socket.Shutdown();
	socket.Receive(recvbuf);

	return 0;
}