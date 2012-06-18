/*
 * NetService.h
 *
 *  Created on: 2011-9-30
 *      Author: AngryPowman
 */

#ifndef NETSERVICE_H_
#define NETSERVICE_H_

#include "AsioTcpServer.hpp"
#include "VnocProtocol.hpp"

class NetService
{
public:
    NetService();
    virtual ~NetService();

public:
    bool start(unsigned int u_port);
private:
    AsioTcpServer *server_;
    VnocProtocol *protocol_;
};

#endif /* NETSERVICE_H_ */
