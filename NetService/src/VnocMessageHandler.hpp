#ifndef VNOCMESSAGEHANDLER_HPP
#define VNOCMESSAGEHANDLER_HPP
#include <stdint.h>
#include <memory>

#include "AsioTcpConnection.hpp"
#include "SocketHandler.hpp"
#include "message/MessageParser.h"
#include "message/PackMessage.h"
#include <ezlogger_headers.hpp>
template <typename ConnectionT>
class VnocMessageHandler : public SocketHandler
{
public:
    VnocMessageHandler(ConnectionT *connection):connection_(connection){}
    virtual void start()
    {
        connection_->recv(headerData_, sizeof(headerData_), 
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
            connection_->recv(headerData_, sizeof(headerData_), 
                std::bind(&VnocMessageHandler::ReadHeadHandler, this,
                std::placeholders::_1,
                std::placeholders::_2));
            return;
        }
        size_t package_len = htonl(*(int *)(headerData_+4));
        char *messageBuffer ( new char[package_len]);
        memcpy(messageBuffer, headerData_, HEAD_LEN);
        connection_->recv(&messageBuffer[HEAD_LEN], package_len - HEAD_LEN, 
                std::bind(&VnocMessageHandler::ReadBodyHandler, this, messageBuffer,
                std::placeholders::_1,
                std::placeholders::_2));

    }

	void ReadBodyHandler(char *messageBuffer, const asio::error_code& error, size_t bytes_transferred){
        std::unique_ptr<char[]> safe_buf(messageBuffer);
        if (error) {
            delete this;
            return;
        }
        //process the message here
        EZLOGGERVLSTREAM(axter::log_rarely)<<"receve message body\n";
        CMessageParser parser;
        std::unique_ptr<CMessage> msg(parser.Parse((byte*)safe_buf.get(), bytes_transferred + HEAD_LEN));
        int ret=0;
        if (msg != NULL){
            int type = msg->GetMessageType();

            switch (type){
            case MSG_RVC_TYPE:
                ret = HandlerRVCMessage(dynamic_cast<MSG_RVC*>(msg.get()));
                break;
            default:
                ret = 0;
            };
        }

        if (ret == 0){
            //message is not handled, we need post a read opeartion
            connection_->recv(headerData_, sizeof(headerData_), 
                std::bind(&VnocMessageHandler::ReadHeadHandler, this,
                std::placeholders::_1,
                std::placeholders::_2));
        }
	}

    void SendHandler(char* buffer, const asio::error_code& error, size_t bytes_transferred)
    {
        delete []buffer;
        if (!error){
            connection_->recv(headerData_, sizeof(headerData_), 
                std::bind(&VnocMessageHandler::ReadHeadHandler, this,
                std::placeholders::_1,
                std::placeholders::_2));
        }
    }

    int HandlerRVCMessage(MSG_RVC *rvcMessage)
    {
        MSG_AVC avcMessage;
        avcMessage.SetCaptchaType(0);
        byte captcha[] = {0};
        avcMessage.SetCaptcha(captcha,sizeof(captcha));
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
    const static size_t HEAD_LEN = 30;
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
