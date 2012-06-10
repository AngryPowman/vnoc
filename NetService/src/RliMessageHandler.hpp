#pragma once
#include "VnocProtocol.hpp"

class RliMessageHandler : public IMessageHandler
{
public:
    RliMessageHandler(VnocProtocol *protocol):protocol_(protocol)
    {
        protocol_->RegisterMessageHandler(this);
    }
    virtual MSGTYPE getMessageType() const
    {
        return MSG_RLI_TYPE;
    }
    virtual int operator()(const CMessage *msg, MessageContext *ctx)
    {
        MSG_ALI aliMessage;
        aliMessage.SetLoginResult(0);
        protocol_->SendVnocMessage(&aliMessage, ctx);
        return 1;
    }
private:
    VnocProtocol *protocol_;
};
