#ifndef VNOCMESSAGEHANDLER_HPP
#define VNOCMESSAGEHANDLER_HPP
#include "AsioTcpConnection.hpp"
#include "SocketHandler.hpp"

#include <ezlogger_headers.hpp>
template <typename ConnectionT>
class VnocMessageHandler : public SocketHandler
{
public:
	VnocMessageHandler(ConnectionT *connection):connection_(connection){}
	virtual void start()
	{
        connection_->recv(headerData_, sizeof(headerData_)-1, 
			std::bind(&VnocMessageHandler::ReadHandler, this,
          std::placeholders::_1,
          std::placeholders::_2));
	}

private:
	void ReadHandler(const asio::error_code& error, size_t bytes_transferred)
	{

	}
	char headerData_[64];
	ConnectionT *connection_;
};
class VnocMessageHandlerFactory: public SocketHandlerFactory
{
public:
	virtual SocketHandler* CreateHandler(AsioTcpConnection *connection)
	{
		return new VnocMessageHandler<AsioTcpConnection>(connection);
	}
};
#endif //VNOCMESSAGEHANDLER_HPP