/*
 * NetService.cpp
 *
 *  Created on: 2011-10-1
 *      Author: AngryPowman
 */

#include "NetService.h"
#include "VnocMessageSocketHandler.hpp"
#include "RliMessageHandler.hpp"
#include "RvcMessageHandler.hpp"


using namespace std;

NetService::NetService()
{

}

NetService::~NetService()
{
    delete server_;
    delete protocol_;
}

bool NetService::start(unsigned int u_port)
{
    protocol_ = new VnocProtocol();
    VnocMessageHandlerFactory socketHandlerFactory(protocol_);
    RliMessageHandler rliHandler(protocol_);
    RvcMessageHandler rvcHandler(protocol_);
    server_= new AsioTcpServer(protocol_->getSocketHandlerFactory());
    server_->start(u_port);
    return true;
}


