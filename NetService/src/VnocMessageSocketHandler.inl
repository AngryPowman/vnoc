
#include "../../Message/MessageParser.h"
#include "../../Message/PackMessage.h"
#include <ezlogger_headers.hpp>
#include "VnocProtocol.hpp"
const size_t MAX_PACKAGE_LEN = 1024*1024U;
template <typename ConnectionT>
VnocMessageSocketHandler<ConnectionT>::VnocMessageSocketHandler(ConnectionT *connection):connection_(connection)
{
    ctx_ = new MessageContext();
    ctx_->connection = (IVnocMessageProtocolHandler*)this;
}
template <typename ConnectionT>
VnocMessageSocketHandler<ConnectionT>::~VnocMessageSocketHandler()
{
    delete ctx_;
}

template <typename ConnectionT>
void VnocMessageSocketHandler<ConnectionT>::readHeader(){
    connection_->recv(headerData_, sizeof(headerData_), 
        std::bind(&VnocMessageSocketHandler::ReadHeaderHandler, this,
        std::placeholders::_1,
        std::placeholders::_2));
}

template <typename ConnectionT>
void VnocMessageSocketHandler<ConnectionT>::start()
{
    readHeader();
}

template <typename ConnectionT>
void VnocMessageSocketHandler<ConnectionT>::ReadHeaderHandler(const asio::error_code& error, size_t bytes_transferred)
{
    if (error) {
        delete this;
        return;
    }
    if (bytes_transferred != HEADER_LEN) {
        EZLOGGERVLSTREAM(axter::log_often)<<"header lenth miss match\n";
        readHeader();
        return;
    }
	if (headerData_[0] != 'V'){
	    EZLOGGERVLSTREAM(axter::log_often)<<"This is not VNOC header.\n";
        readHeader();
        return;
	}
    size_t package_len = htonl(*(int *)(headerData_+4));
    if (package_len <= HEADER_LEN) {
        EZLOGGERVLSTREAM(axter::log_often)<<"package_len <= HEAD_LEN\n";
        readHeader();
        return;
    }
	if (package_len > MAX_PACKAGE_LEN) {
        EZLOGGERVLSTREAM(axter::log_often)<<"package_len > MAX_PACKAGE_LEN\n";
        readHeader();
        return;
    } 
    char *messageBuffer ( new char[package_len]);
    memcpy(messageBuffer, headerData_, HEADER_LEN);
    connection_->recv(&messageBuffer[HEADER_LEN], package_len - HEADER_LEN, 
        std::bind(&VnocMessageSocketHandler::ReadBodyHandler, this, messageBuffer,
            std::placeholders::_1,
            std::placeholders::_2));
}

template <typename ConnectionT>
void VnocMessageSocketHandler<ConnectionT>::ReadBodyHandler(char *messageBuffer, const asio::error_code& error, size_t bytes_transferred){
    std::unique_ptr<char[]> safe_buf(messageBuffer);
    if (error) {
        delete this;
        return;
    }
    //process the message here
    EZLOGGERVLSTREAM(axter::log_rarely)<<"receve message body\n";
    CMessageParser parser;
    std::unique_ptr<CMessage> msg(parser.Parse((byte*)safe_buf.get(), bytes_transferred + HEADER_LEN));
    int ret=0;
    if (msg != NULL){
        int type = msg->GetMessageType();
        auto handlers = protocol_->getHandler((MSGTYPE)type);
        for(auto i = handlers.begin(); i != handlers.end(); ++i){
            ret = (**i)(msg.get(), this->ctx_) || ret;
        }
    }

    if (ret == 0){
        //message is not handled, we need post a read opeartion
        readHeader();
    }
}

template <typename ConnectionT>
void VnocMessageSocketHandler<ConnectionT>::SendHandler(char* buffer, const asio::error_code& error, size_t bytes_transferred)
{
    delete []buffer;
    if (!error){
        readHeader();
    }
}

 template <typename ConnectionT>
void VnocMessageSocketHandler<ConnectionT>::SendVnocMessage(const CMessage *msg)
{
    PackMessage packer;
    size_t length = packer.GetMessageLen(msg);
    assert(length != 0);
    char *pack = new char[length];
    packer.Pack(msg, (byte*)pack, length);
    connection_->send(pack, length,
        std::bind(&VnocMessageSocketHandler::SendHandler, this, pack,
            std::placeholders::_1,
            std::placeholders::_2));
}
