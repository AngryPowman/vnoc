#ifndef VNOCMESSAGEHANDLER_HPP
#define VNOCMESSAGEHANDLER_HPP
#include <stdint.h>
#include <memory>

#include "AsioTcpConnection.hpp"
#include "SocketHandler.hpp"
#include "../../Message/MsgDef.h"
#include <ezlogger_headers.hpp>
template <typename ConnectionT>
class VnocMessageHandler : public SocketHandler
{
public:
    VnocMessageHandler(ConnectionT *connection):connection_(connection){}
    //start handler the message on the connection
    virtual void start();

    //post a read operation to wait for message header.
    void readHeader();

private:
    void ReadHeaderHandler(const asio::error_code& error, size_t bytes_transferred);
    void ReadBodyHandler(char *messageBuffer, const asio::error_code& error, size_t bytes_transferred);
    void SendHandler(char* buffer, const asio::error_code& error, size_t bytes_transferred);

    int HandleRVCMessage(MSG_RVC *rvcMessage);
    int HandleRLIMessage(MSG_RLI *rliMessage);

    const static size_t HEADER_LEN = 30;
    char headerData_[HEADER_LEN];
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
#include "VnocMessageHandler.inl"
#endif //VNOCMESSAGEHANDLER_HPP
