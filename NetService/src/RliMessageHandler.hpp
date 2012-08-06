#pragma once
#include "VnocProtocol.hpp"
#include "UserManage.hpp"

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
		const MSG_RLI * rliMessage = dynamic_cast<const MSG_RLI *>(msg);
		userinfo UserInfo = {0};
		if (CUserManage::GetInstance()->Authenticate((char*)rliMessage->GetAccountNumber(), (char*)rliMessage->GetPassword(), &UserInfo) == LOGIN_OK)
		{
			aliMessage.SetLoginResult(0); //µÇÂ½³É¹¦
		}
		else
		{
			aliMessage.SetLoginResult(1);//Ê§°Ü
		}
        protocol_->SendVnocMessage(&aliMessage, ctx);
        return 1;
    }
private:
    VnocProtocol *protocol_;
};
