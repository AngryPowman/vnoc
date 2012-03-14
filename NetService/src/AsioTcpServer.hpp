#ifndef ASIOTCPSERVER_H_
#define ASIOTCPSERVER_H_

#include "asio.hpp"
#include "SocketServer.hpp"

class AsioTcpServer :public SocketServer
{
public:
    AsioTcpServer();
    ~AsioTcpServer();
    virtual bool start(unsigned int port);
private:
    asio::io_service& io_service_;
    asio::ip::tcp::acceptor acceptor_;
    void worker();
};


#endif /* ASIOTCPSERVER_H_ */