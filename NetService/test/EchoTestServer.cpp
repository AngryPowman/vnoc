//============================================================================
// Name        : epoll_server.cpp
// Author      : AngryPowman
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "../src/AsioTcpServer.hpp"
#include <iostream>
#include "../src/test/EchoTestHandler.hpp"
using namespace std;

int main()
{
	EchoTestHandlerFactory factory;
	AsioTcpServer server(&factory);
	server.start(12345);

	return 0;
}
