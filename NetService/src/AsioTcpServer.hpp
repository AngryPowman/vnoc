#ifndef ASIOTCPSERVER_H_
#define ASIOTCPSERVER_H_
#include <memory>
#include "asio.hpp"
#include "SocketServer.hpp"
#include "AsioTcpConnection.hpp"
#include "SocketHandler.hpp"

class AsioTcpServer :public SocketServer
{
public:
    AsioTcpServer(SocketHandlerFactory* handlerFactory);
    ~AsioTcpServer();
    virtual bool start(unsigned int port);
private:
    asio::io_service& io_service_;
    asio::ip::tcp::acceptor acceptor_;
	SocketHandlerFactory* handlerFactory_;
	AsioTcpConnection *new_connection_;
    void worker();
	void AcceptHandler( AsioTcpConnection *conn, const asio::error_code& error);
};


#endif /* ASIOTCPSERVER_H_ */