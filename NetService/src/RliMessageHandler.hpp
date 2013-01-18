#pragma once
#include "VnocProtocol.hpp"
#include "UserManage.hpp"
#include <ezlogger_headers.hpp>

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
		int LoginResult;
        MSG_ALI aliMessage;
		const MSG_RLI * rliMessage = dynamic_cast<const MSG_RLI *>(msg);
		userinfo UserInfo = {0};

		LoginResult = CUserManage::GetInstance()->Authenticate((char*)rliMessage->GetAccountNumber(), (char*)rliMessage->GetPassword(), &UserInfo);
		if (LOGIN_OK == LoginResult)
		{
		aliMessage.SetLoginResult(0); //µÇÂ½³É¹¦
		EZLOGGERVLSTREAM(axter::log_often)<<"User's account login successfully. (RLI)"<<endl; 
		}
		else if(NULLPOINT == LoginResult)
		{
		aliMessage.SetLoginResult(1);//Ê§°Ü
		EZLOGGERVLSTREAM(axter::log_often)<<"Msg is NULL (RLI)"<<endl; 
		}
		else if(TEST_FALSE == LoginResult)
		{
		aliMessage.SetLoginResult(1);//Ê§°Ü
		EZLOGGERVLSTREAM(axter::log_often)<<"User's password is error, "<<
										  "User's account is: "<<
										  ctx->userName<<
										  " (RLI)"<<endl;
		}
		else if(ACCOUNT_NULL == LoginResult)
		{
		aliMessage.SetLoginResult(1);//Ê§°Ü
		EZLOGGERVLSTREAM(axter::log_often)<<"User's Account is NULL (RLI)"<<endl;
		}

        protocol_->SendVnocMessage(&aliMessage, ctx);
		EZLOGGERVLSTREAM(axter::log_often)<<"Server had geted RLI and had sended ALI to client. (RLI)"<<endl;
        return 1;
    }
    
private:
    VnocProtocol *protocol_;
};
