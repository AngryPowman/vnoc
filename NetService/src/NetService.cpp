/*
 * NetService.cpp
 *
 *  Created on: 2011-10-1
 *      Author: AngryPowman
 */

#include "NetService.h"
#include "VnocMessageHandler.hpp"

using namespace std;
static VnocMessageHandlerFactory handlerFactory_g;
NetService::NetService():server_(&handlerFactory_g)
{

}

NetService::~NetService()
{

}

bool NetService::start(unsigned int u_port)
{
    server_.start(u_port);
    return true;
}


