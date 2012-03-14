#include "AsioTcpServer.hpp"
#include "AsioTcpConnection.hpp"
#include <functional>
#include <thread>
#include <iostream>
#include "asio.hpp"
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
    acceptor_.bind(endpoint);
    AsioTcpConnetion* new_connetion(new AsioTcpConnetion(io_service_));
    acceptor_.async_accept(new_connetion->socket(),accept_handler);
    std::thread t(&AsioTcpServer::worker, this);
    t.join();
    return true;
}
