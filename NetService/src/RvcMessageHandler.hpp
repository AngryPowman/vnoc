#pragma once
#include "VnocProtocol.hpp"
#include <ezlogger_headers.hpp>

class RvcMessageHandler: public IMessageHandler
{
public:
    RvcMessageHandler(VnocProtocol *protocol):protocol_(protocol)
    {
        protocol_->RegisterMessageHandler(this);
    }

    virtual VMsg getMessageType() const
    {
        return MSG_RequestVerificationCode_Id;
    }

    virtual int operator()(const CMessage *msg, MessageContext *ctx)
    {
        MSG_AnswerVerificationCode avcMessage;
        avcMessage.SetCaptchaType(0);
        std::string strCaptcha;
        avcMessage.SetCaptcha(strCaptcha);
        avcMessage.SetLoginTag(1);
        protocol_->SendVnocMessage(&avcMessage, ctx);
        EZLOGGERVLSTREAM(axter::log_often)<<"Server had geted RVC and had sended AVC to client. (RVC)"<<endl;
        return 1;
    }

private:
    VnocProtocol *protocol_;
};