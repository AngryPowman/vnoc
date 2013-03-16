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

    virtual VMsg getMessageType() const
    {
        return MSG_RequestLogin_Id;
    }

    virtual int operator()(const CMessage *msg, MessageContext *ctx)
    {
        int LoginResult;
        MSG_AnswerLogin aliMessage;
        std::string strAccountNumber;
        std::string strPassword;
        MSG_RequestLogin rliMessage(*msg);
        userinfo UserInfo = {0};
        rliMessage.GetAccountNumber(strAccountNumber);
        rliMessage.GetPassword(strPassword);
        LoginResult = CUserManage::GetInstance()->Authenticate(strAccountNumber.c_str(), strPassword.c_str(), &UserInfo);
        if (LOGIN_OK == LoginResult)
        {
            aliMessage.SetLoginResult(0); //µÇÂ½³É¹¦
            ctx->userName = strAccountNumber;
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
        else if(HAS_LOGINED == LoginResult)
        {
            aliMessage.SetLoginResult(1);//Ê§°Ü
            EZLOGGERVLSTREAM(axter::log_often)<<"This user has logined (RLI)"<<endl;
        }

        protocol_->SendVnocMessage(&aliMessage, ctx);
        EZLOGGERVLSTREAM(axter::log_often)<<"Server had geted RLI and had sended ALI to client. (RLI)"<<endl;
        return 1;
    }

private:
    VnocProtocol *protocol_;
};
