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
#include <string>
#include "SQLUserStorage.h"
#include "UserManage.hpp"
using namespace std;


int main()
{
    Config::getInstance()->Initialize("vnoc.conf");
    cout<<"port:"<<Config::getInstance()->getValue("port")<<endl;
    sUserStorage us;
    CUserManage::GetInstance()->initial(&us);

    NetService net;
    net.start(Config::getInstance()->getValue("port"));

    return 0;
}

#ifdef WIN32
#include "../../VisualLeakDetector/include/vld.h"
#endif