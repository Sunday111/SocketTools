#include <iostream>

#include "RunServer.h"

void main()
{
	try
	{
		constexpr auto defaultPort = "27015";
		test_server::RunServer(defaultPort);
	}
	catch (const std::exception& ex)
	{
		std::cout << "Exception: " << ex.what() << std::endl;
	}
}