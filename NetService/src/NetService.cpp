/*
 * NetService.cpp
 *
 *  Created on: 2011-10-1
 *      Author: AngryPowman
 */

#include "NetService.h"

using namespace std;

NetService::NetService()
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


