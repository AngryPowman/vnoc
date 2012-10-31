#ifndef VNOCMESSAGEHANDLER_HPP
#define VNOCMESSAGEHANDLER_HPP
#include <stdint.h>
#include <memory>
#include "AsioTcpConnection.hpp"
#include "SocketHandler.hpp"
#include "../../Message/MsgDef.h"
#include "VnocProtocol.hpp"
#include "util.hpp"
#include <ezlogger_headers.hpp>

template <typename ConnectionT>
class VnocMessageSocketHandler : public SocketHandler, IVnocMessageProtocolHandler
{
public:
    VnocMessageSocketHandler(ConnectionT *connection);
    ~VnocMessageSocketHandler();

    //start handler the message on the connection
    virtual void start();
    void setProtocol(VnocProtocol* protocol){protocol_= protocol;}
    virtual void SendVnocMessage(const CMessage *msg);

private:
    void ReadHeaderHandler(const asio::error_code& error, size_t bytes_transferred);
    void ReadBodyHandler(char *messageBuffer, const asio::error_code& error, size_t bytes_transferred);
    void SendHandler(smart_buf buffer, const asio::error_code& error, size_t bytes_transferred);

    //post a read operation to wait for message header.
    void readHeader();

    const static size_t HEADER_LEN = 30;
    char headerData_[HEADER_LEN];
    ConnectionT* connection_;
    MessageContext *ctx_;
    VnocProtocol* protocol_;

};
class VnocMessageHandlerFactory: public SocketHandlerFactory
{
public:
    VnocMessageHandlerFactory(VnocProtocol *protocol){

        protocol->RegisterSocketHandlerFactory(this);
        protocol_ = protocol;
    }
    virtual SocketHandler* CreateHandler(AsioTcpConnection *connection)
    {
        auto handler = new VnocMessageSocketHandler<AsioTcpConnection>(connection);
        handler->setProtocol(protocol_);
        return handler;
    }
private:
        VnocProtocol *protocol_;
};
#include "VnocMessageSocketHandler.inl"
#endif //VNOCMESSAGEHANDLER_HPP
