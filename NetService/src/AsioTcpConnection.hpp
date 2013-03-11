#ifndef ASIOTCPCONNECTION_H_
#define ASIOTCPCONNECTION_H_
#include "asio.hpp"
#include "SocketServer.hpp"

class AsioTcpConnection: public Connection
{
public:
    AsioTcpConnection(asio::io_service& io_service):socket_(io_service){}
    asio::ip::tcp::socket& socket(){return socket_;} 
    template <typename Handler> int recv(char * buf, size_t len, Handler handler)
    {
        async_read(socket_, asio::buffer(buf, len), handler);
        return 0;
    }
    template <typename Handler> int read_some(char * buf, size_t len, Handler handler)
    {
        socket_.async_read_some(asio::buffer(buf, len), handler);
        return 0;
    }
    template <typename Handler> int send(char * buf, size_t len, Handler handler)
    {
        socket_.async_send(asio::buffer(buf, len), handler);
        return 0;
    }
private:
    asio::ip::tcp::socket socket_;
};

#endif /* ASIOTCPCONNECTION_H_ */