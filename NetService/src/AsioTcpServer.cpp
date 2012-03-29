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
static void accept_handler(const asio::error_code& error)
{
  if (!error)
  {
    // Accept succeeded.
  }
}
AsioTcpServer::AsioTcpServer():
    io_service_(g_io_service),
    acceptor_(g_io_service)
{}

AsioTcpServer::~AsioTcpServer()
{
}
void AsioTcpServer::worker()
{
    cout<<"start worker"<<endl;
    g_io_service.run();
}
bool AsioTcpServer::start(unsigned int port)
{
    tcp::endpoint endpoint(tcp::v4(), port);
    acceptor_.open(tcp::v4());
    acceptor_.bind(tcp::endpoint(tcp::v4(), port));
    AsioTcpConnection* new_connection(new AsioTcpConnection(io_service_));
    auto hanler(new EchoTestHandler(new_connection));
	acceptor_.listen();
    acceptor_.async_accept(new_connection->socket(),
        std::bind(&EchoTestHandler::AcceptHandler, hanler,
          std::placeholders::_1));
    std::thread t(&AsioTcpServer::worker, this);
    t.join();
    return true;
}
