#pragma once
#include "VnocProtocol.hpp"
#include "UserManage.hpp"

class RclMessageHandler : public IMessageHandler
{
public:
    RclMessageHandler(VnocProtocol *protocol):protocol_(protocol)
    {
        protocol_->RegisterMessageHandler(this);
    }
    virtual MSGTYPE getMessageType() const
    {
        return MSG_RCL_TYPE;
    }
    virtual int operator()(const CMessage *msg, MessageContext *ctx)
    {
        MSG_ACL aclMessage;
        const MSG_RCL * rclMessage = dynamic_cast<const MSG_RCL *>(msg);

        protocol_->SendVnocMessage(&aclMessage, ctx);
        return 1;
    }
private:
    VnocProtocol *protocol_;
};