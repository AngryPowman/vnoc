
#include "../../Message/MessageParser.h"
#include "../../Message/PackMessage.h"
#include <ezlogger_headers.hpp>
#include "VnocProtocol.hpp"
const size_t MAX_PACKAGE_LEN = 1024*1024U;
using namespace std;
template <typename ConnectionT>
VnocMessageSocketHandler<ConnectionT>::VnocMessageSocketHandler(ConnectionT *connection):connection_(connection)
{
    ctx_ = new MessageContext();
    ctx_->connection = (IVnocMessageProtocolHandler*)this;
}
template <typename ConnectionT>
VnocMessageSocketHandler<ConnectionT>::~VnocMessageSocketHandler()
{
    EZLOGGERVLSTREAM(axter::log_often)<<"delete connection Handler."<<endl;
	delete ctx_;
	delete connection_;
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
    EZLOGGERVLSTREAM(axter::log_often)<<"New connection start."<<endl;
    readHeader();
}

template <typename ConnectionT>
void VnocMessageSocketHandler<ConnectionT>::ReadHeaderHandler(const asio::error_code& error, size_t bytes_transferred)
{
    EZLOGGERVLSTREAM(axter::log_often)<<"ReadHeaderHandler, received: "<<bytes_transferred<<endl;
    if (error) {
        delete this;
        return;
    }
    if (bytes_transferred != HEADER_LEN) {
        EZLOGGERVLSTREAM(axter::log_often)<<"header lenth miss match"<<endl;
        readHeader();
        return;
    }
	if (headerData_[0] != 'V'){
	    EZLOGGERVLSTREAM(axter::log_often)<<"This is not VNOC header."<<endl;
        readHeader();
        return;
	}
    size_t package_len = htonl(*(int *)(headerData_+4));
    if (package_len <= HEADER_LEN) {
        EZLOGGERVLSTREAM(axter::log_often)<<"package_len <= HEAD_LEN"<<endl;
        readHeader();
        return;
    }
	if (package_len > MAX_PACKAGE_LEN) {
        EZLOGGERVLSTREAM(axter::log_often)<<"package_len > MAX_PACKAGE_LEN"<<endl;
        readHeader();
        return;
    } 
    char *messageBuffer ( new char[package_len]);
    memcpy(messageBuffer, headerData_, HEADER_LEN);
    EZLOGGERVLSTREAM(axter::log_often)<<"get correct header, lenth:"<<package_len<<endl;
    connection_->recv(&messageBuffer[HEADER_LEN], package_len - HEADER_LEN, 
        std::bind(&VnocMessageSocketHandler::ReadBodyHandler, this, messageBuffer,
            std::placeholders::_1,
            std::placeholders::_2));
}

template <typename ConnectionT>
void VnocMessageSocketHandler<ConnectionT>::ReadBodyHandler(char *messageBuffer, const asio::error_code& error, size_t bytes_transferred){
    EZLOGGERVLSTREAM(axter::log_often)<<"received: "<<bytes_transferred<<endl;
    std::unique_ptr<char[]> safe_buf(messageBuffer);
    if (error) {
        delete this;
        return;
    }
    //process the message here
    EZLOGGERVLSTREAM(axter::log_often)<<"receve message body"<<endl;
    CMessageParser parser;
    std::unique_ptr<CMessage> msg(parser.Parse((byte*)safe_buf.get(), bytes_transferred + HEADER_LEN));
    int ret=0;
    if (msg){
        int type = msg->GetMessageType();
        EZLOGGERVLSTREAM(axter::log_often)<<"message type: "<<type<<endl;
        auto handlers = protocol_->getHandler((MSGTYPE)type);
        for(auto i = handlers.begin(); i != handlers.end(); ++i){
            ret = (**i)(msg.get(), this->ctx_) || ret;
        }
    }
    //wait for next message
    readHeader();

}

template <typename ConnectionT>
void VnocMessageSocketHandler<ConnectionT>::SendHandler(smart_buf buffer, const asio::error_code& error, size_t bytes_transferred)
{

}

 template <typename ConnectionT>
void VnocMessageSocketHandler<ConnectionT>::SendVnocMessage(const CMessage *msg)
{
    PackMessage packer;
    size_t length = packer.GetMessageLen(msg);
    assert(length != 0);
    smart_buf pack(new char[length]);
    packer.Pack(msg, (byte*)pack, length);
    char *buf = pack;
    connection_->send(buf, length,
        std::bind(&VnocMessageSocketHandler::SendHandler, this, pack,
            std::placeholders::_1,
            std::placeholders::_2));
    EZLOGGERVLSTREAM(axter::log_often)<<"send vnoc message type:"<< msg->GetMessageType()<<" lenght: "<<length<<endl;
}
