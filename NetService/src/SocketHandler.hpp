#ifndef SOCKETHANDLER_H_
#define SOCKETHANDLER_H_
#include "AsioTcpConnection.hpp"
class SocketHandler
{
public:
	virtual void start()=0;
    virtual void ReadsomeHandler(const asio::error_code& error, size_t bytes_transferred)=0;    
	virtual void HandleWrite(const asio::error_code& error)=0;
};
class SocketHandlerFactory
{
public:
	virtual SocketHandler* CreateHandler(AsioTcpConnection *connection)=0;
};

#endif /* ECHOTESTHANDLER_H_ */
