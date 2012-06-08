
#include "../../Message/MessageParser.h"
#include "../../Message/PackMessage.h"
#include <ezlogger_headers.hpp>
const size_t MAX_PACKAGE_LEN = 1024*1024U;
template <typename ConnectionT>
void VnocMessageHandler<ConnectionT>::readHeader(){
    connection_->recv(headerData_, sizeof(headerData_), 
        std::bind(&VnocMessageHandler::ReadHeaderHandler, this,
        std::placeholders::_1,
        std::placeholders::_2));
}

template <typename ConnectionT>
void VnocMessageHandler<ConnectionT>::start()
{
    readHeader();
}

template <typename ConnectionT>
void VnocMessageHandler<ConnectionT>::ReadHeaderHandler(const asio::error_code& error, size_t bytes_transferred)
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
        std::bind(&VnocMessageHandler::ReadBodyHandler, this, messageBuffer,
            std::placeholders::_1,
            std::placeholders::_2));
}

template <typename ConnectionT>
void VnocMessageHandler<ConnectionT>::ReadBodyHandler(char *messageBuffer, const asio::error_code& error, size_t bytes_transferred){
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

        switch (type){
        case MSG_RVC_TYPE:
            ret = HandleRVCMessage(dynamic_cast<MSG_RVC*>(msg.get()));
            break;
        case MSG_RLI_TYPE:
            ret = HandleRLIMessage(dynamic_cast<MSG_RLI*>(msg.get()));
            break;
        default:
            ret = 0;
        };
    }

    if (ret == 0){
        //message is not handled, we need post a read opeartion
        readHeader();
    }
}

template <typename ConnectionT>
void VnocMessageHandler<ConnectionT>::SendHandler(char* buffer, const asio::error_code& error, size_t bytes_transferred)
{
    delete []buffer;
    if (!error){
        readHeader();
    }
}

template <typename ConnectionT>
int VnocMessageHandler<ConnectionT>::HandleRVCMessage(MSG_RVC *rvcMessage)
{
    MSG_AVC avcMessage;
    avcMessage.SetCaptchaType(0);
    byte captcha[] = {0};
    avcMessage.SetCaptcha(captcha,sizeof(captcha));
    avcMessage.SetLoginTag(1);
    PackMessage packer;
    size_t avcLen = packer.GetMessageLen(&avcMessage);
    char *avcPack = new char[avcLen];
    packer.Pack(&avcMessage, (byte*)avcPack, avcLen);
    connection_->send(avcPack, avcLen,
        std::bind(&VnocMessageHandler::SendHandler, this, avcPack,
            std::placeholders::_1,
            std::placeholders::_2));

    return 1;
}

template <typename ConnectionT>
int VnocMessageHandler<ConnectionT>::HandleRLIMessage(MSG_RLI *rliMessage)
{
    MSG_ALI aliMessage;
    aliMessage.SetLoginResult(0);
    PackMessage packer;
    size_t aliLen = packer.GetMessageLen(&aliMessage);
    assert(aliLen != 0);
    char *Pack = new char[aliLen];
    packer.Pack(&aliMessage, (byte*)Pack, aliLen);
    connection_->send(Pack, aliLen,
        std::bind(&VnocMessageHandler::SendHandler, this, Pack,
            std::placeholders::_1,
            std::placeholders::_2));
    return 1;
}