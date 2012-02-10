//============================================================================
// Name        : epoll_server.cpp
// Author      : AngryPowman
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "src/NetService.h"
#include <iostream>
using namespace std;

int main()
{
	NetService net;
	net.start(12345);

	return 0;
}
