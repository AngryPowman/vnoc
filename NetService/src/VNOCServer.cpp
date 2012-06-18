//============================================================================
// Name        : epoll_server.cpp
// Author      : AngryPowman
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================



#include <iostream>
#include "NetService.h"
#include "Config.hpp"

using namespace std;

int main()
{
	NetService net;
	net.start(2508);

	return 0;
}

#ifdef WIN32
#include "../../VisualLeakDetector/include/vld.h"
#endif