#ifndef VNOCMESSAGEHANDLER_HPP
#define VNOCMESSAGEHANDLER_HPP
#include <stdint.h>
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
            std::bind(&VnocMessageHandler::ReadHeadHandler, this,
          std::placeholders::_1,
          std::placeholders::_2));
    }

private:
    void ReadHeadHandler(const asio::error_code& error, size_t bytes_transferred)
    {
        if (error) {
            delete this;
            return;
        }
        if (bytes_transferred != HEAD_LEN) {
            EZLOGGERVLSTREAM(axter::log_often)<<"head lenth miss match\n";
            connection_->recv(headerData_, sizeof(headerData_)-1, 
                std::bind(&VnocMessageHandler::ReadHeadHandler, this,
                std::placeholders::_1,
                std::placeholders::_2));
            return;
        }
    }
	void ReadBodyHandler(const asio::error_code& error, size_t bytes_transferred){
	}
    const static size_t HEAD_LEN = 29;
    char headerData_[HEAD_LEN];
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