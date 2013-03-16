
#include "../../NMessage/Message2Parser.h"
#include "../../NMessage/Message2Pack.h"
#include "../../NMessage/BufferMessage.h"
#include <ezlogger_headers.hpp>
#include "VnocProtocol.hpp"
const size_t MAX_PACKAGE_LEN = 1024*1024U;
using namespace std;
using namespace VNOC::Message;
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
    CUserManage::GetInstance()->deleteOnlineUser(ctx_->userName.c_str());
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
    if (bytes_transferred != MSG_HEAD_LEN) {
        EZLOGGERVLSTREAM(axter::log_often)<<"header lenth miss match"<<endl;
        readHeader();
        return;
    }
    if (headerData_[0] != 'V'){
        EZLOGGERVLSTREAM(axter::log_often)<<"This is not VNOC header."<<endl;
        readHeader();
        return;
    }
    size_t package_len = *(int *)(headerData_ + 2);
    if (package_len <= MSG_HEAD_LEN) {
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
    memcpy(messageBuffer, headerData_, MSG_HEAD_LEN);
    EZLOGGERVLSTREAM(axter::log_often)<<"get correct header, lenth:"<<package_len<<endl;
    connection_->recv(&messageBuffer[MSG_HEAD_LEN], package_len - MSG_HEAD_LEN, 
        std::bind(&VnocMessageSocketHandler::ReadBodyHandler, this, messageBuffer,
        std::placeholders::_1,
        std::placeholders::_2));
}

template <typename ConnectionT>
void VnocMessageSocketHandler<ConnectionT>::ReadBodyHandler(char *messageBuffer, const asio::error_code& error, size_t bytes_transferred){
    EZLOGGERVLSTREAM(axter::log_often)<<"received: "<<bytes_transferred<<endl;
    CBufferMessage safe_buf;
    safe_buf.Copy(messageBuffer, bytes_transferred + MSG_HEAD_LEN);
    if (error) {
        delete this;
        return;
    }
    //process the message here
    EZLOGGERVLSTREAM(axter::log_often)<<"receve message body"<<endl;
    CMessage2Parser m2Parser;
    CMessage msg(CMessage2Parser::GetMsgType(safe_buf));
    m2Parser.Parser(&msg, safe_buf);
    int ret=0;
    if (msg.IsValid()){
        int type = msg.MsgId();
        EZLOGGERVLSTREAM(axter::log_often)<<"message type: "<<type<<endl;
        auto handlers = protocol_->getHandler((VMsg)type);
        for(auto message_handler = handlers.begin(); message_handler != handlers.end(); ++message_handler){
            ret = (**message_handler)(&msg, this->ctx_) || ret;
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
    CMessage2Pack m2Pack;
    CBufferMessage PackBuff;
    m2Pack.PackMessage(msg, PackBuff);
    size_t length = PackBuff.GetSize();
    assert(length != 0);
    assert(PackBuff.GetBuffer() != NULL);
    smart_buf pack(new char[length]);
    memcpy((char*)pack, PackBuff.GetBuffer(), length);
    char *buf = pack;
    connection_->send(buf, length,
        std::bind(&VnocMessageSocketHandler::SendHandler, this, pack,
            std::placeholders::_1,
            std::placeholders::_2));
    EZLOGGERVLSTREAM(axter::log_often)<<"send vnoc message type:"<< msg->MsgId()<<" lenght: "<<length<<endl;
}
