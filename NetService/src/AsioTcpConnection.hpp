#ifndef ASIOTCPCONNECTION_H_
#define ASIOTCPCONNECTION_H_
#include "asio.hpp"
#include "SocketServer.hpp"

class AsioTcpConnetion: public Connection
{
public:
    AsioTcpConnetion(asio::io_service& io_service):socket_(io_service){}
    asio::ip::tcp::socket& socket(){return socket_;} 
    bool start(){return true;}
    template <typename Handler> int recv(char * buf, size_t len, Handler handler);
    template <typename Handler> int recv_some(char * buf, size_t len, Handler handler);
    template <typename Handler> int send(char * buf, size_t len, Handler handler);
private:
    asio::ip::tcp::socket socket_;
};

#endif /* ASIOTCPCONNECTION_H_ */