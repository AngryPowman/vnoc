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

    virtual MSGTYPE getMessageType() const
    {
        return MSG_RVC_TYPE;
    }

    virtual int operator()(const CMessage *msg, MessageContext *ctx)
    {
        MSG_AVC avcMessage;
        avcMessage.SetCaptchaType(0);
        byte captcha[] = {0};
        avcMessage.SetCaptcha(captcha,sizeof(captcha));
        avcMessage.SetLoginTag(1);
        protocol_->SendVnocMessage(&avcMessage, ctx);
		EZLOGGERVLSTREAM(axter::log_often)<<"Server has sended Rvc to client."<<endl;
        return 1;
    }

private:
    VnocProtocol *protocol_;
};