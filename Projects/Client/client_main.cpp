#include <SocketTools/ClientSocket.h>
#include <SocketTools/SocketManager.h>

int __cdecl main(int argc, char **argv)
{
	constexpr auto defaultBuflen = 512;
	constexpr auto defaultPort = "27015";

	char sendbuf[] = "Very very very very very very very very very very long message";
	char recvbuf[defaultBuflen];

	// Validate the parameters
	if (argc != 2)
	{
		printf("usage: %s server-name\n", argv[0]);
		return 1;
	}

	using namespace socket_tools;
	SocketManager socketManager;
	ClientSocket socket(argv[1], defaultPort);

	socket.Send(sendbuf);
	socket.Shutdown();
	socket.Receive(recvbuf);

	return 0;
}