#include "AsioTcpServer.hpp"
#include "AsioTcpConnection.hpp"
#include <functional>
#include <thread>
#include <iostream>
#include "asio.hpp"
#include <ezlogger_headers.hpp>
#include <signal.h>
using namespace asio;
using namespace asio::ip;
using namespace std;

static asio::io_service g_io_service;
void TermHandler(int para)
{
    cout<<"term server"<<endl;
    g_io_service.stop();
}
AsioTcpServer::AsioTcpServer(SocketHandlerFactory* handlerFactory):
    io_service_(g_io_service),
    acceptor_(g_io_service),
	handlerFactory_(handlerFactory)
{}

AsioTcpServer::~AsioTcpServer()
{
	delete new_connection_;
}
void AsioTcpServer::worker()
{
    EZLOGGERSTREAM<<"start worker"<<endl;
    g_io_service.run();
}
void AsioTcpServer::AcceptHandler( AsioTcpConnection *conn, const asio::error_code& error)
{
    if (!error)
    {
        SocketHandler *handler=handlerFactory_->CreateHandler(conn);
        handler->start();
        
        new_connection_ = new AsioTcpConnection(io_service_);
        acceptor_.async_accept(new_connection_->socket(),
            std::bind(&AsioTcpServer::AcceptHandler, this ,new_connection_,
                std::placeholders::_1));
    } else 
    {
        EZLOGGERSTREAM<<error.message()<<std::endl;
    }
}

bool AsioTcpServer::start(unsigned int port)
{
    signal (SIGINT, TermHandler);
    signal (SIGTERM, TermHandler);
    tcp::endpoint endpoint(tcp::v4(), port);
    acceptor_.open(tcp::v4());
    acceptor_.bind(tcp::endpoint(tcp::v4(), port));
	acceptor_.listen();
	new_connection_ = new AsioTcpConnection(io_service_);
    acceptor_.async_accept(new_connection_->socket(),
        std::bind(&AsioTcpServer::AcceptHandler, this ,new_connection_,
			std::placeholders::_1));
    std::thread t(&AsioTcpServer::worker, this);
    t.join();
    return true;
}
