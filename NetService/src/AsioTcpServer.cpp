#include "AsioTcpServer.hpp"
#include "AsioTcpConnection.hpp"
#include <functional>
#include <thread>
#include <iostream>
#include "asio.hpp"
#include "test/EchoTestHandler.hpp"
using namespace asio;
using namespace asio::ip;
using namespace std;

static asio::io_service g_io_service;

AsioTcpServer::AsioTcpServer(SocketHandlerFactory* handlerFactory):
    io_service_(g_io_service),
    acceptor_(g_io_service),
	handlerFactory_(handlerFactory)
{}

AsioTcpServer::~AsioTcpServer()
{
}
void AsioTcpServer::worker()
{
    cout<<"start worker"<<endl;
    g_io_service.run();
}
void AsioTcpServer::AcceptHandler( AsioTcpConnection* conn, const asio::error_code& error)
{
	if (!error)
	{
		SocketHandler *handler=handlerFactory_->CreateHandler(conn);
		handler->start();
		
		AsioTcpConnection* new_connection(new AsioTcpConnection(io_service_));
		acceptor_.async_accept(new_connection->socket(),
			std::bind(&AsioTcpServer::AcceptHandler, this ,new_connection,
				std::placeholders::_1));
	} else 
	{
		std::cout<<error.message()<<std::endl;
	}
}

bool AsioTcpServer::start(unsigned int port)
{
    tcp::endpoint endpoint(tcp::v4(), port);
    acceptor_.open(tcp::v4());
    acceptor_.bind(tcp::endpoint(tcp::v4(), port));
	acceptor_.listen();
	AsioTcpConnection* new_connection(new AsioTcpConnection(io_service_));
    acceptor_.async_accept(new_connection->socket(),
        std::bind(&AsioTcpServer::AcceptHandler, this ,new_connection,
			std::placeholders::_1));
    std::thread t(&AsioTcpServer::worker, this);
    t.join();
    return true;
}
