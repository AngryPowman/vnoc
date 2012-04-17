#ifndef SOCKETHANDLER_H_
#define SOCKETHANDLER_H_
#include "AsioTcpConnection.hpp"
class SocketHandler
{
public:
	virtual void start()=0;
};
class SocketHandlerFactory
{
public:
	virtual SocketHandler* CreateHandler(AsioTcpConnection *connection)=0;
};

#endif /* ECHOTESTHANDLER_H_ */
