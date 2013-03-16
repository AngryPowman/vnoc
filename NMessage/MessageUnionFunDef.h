#ifndef VNOC_D_MESSSAGE_UNIONFUN_DEF
#define VNOC_D_MESSSAGE_UNIONFUN_DEF

#include "CMessage.h"
#include "MessageUnion.h"

namespace VNOC
{
namespace Message
{

void RegistrePort(CMessage* pMsg, int nId)
{
    CMessage* pMsgConver = NULL;
    switch(nId)
    {
    case MSG_RequestRegister_Id:
        pMsgConver = new MSG_RequestRegister;
        pMsg->CopyPort(*pMsgConver);
        break;
    case MSG_AnswerRegister_Id:
        pMsgConver = new MSG_AnswerRegister;
        pMsg->CopyPort(*pMsgConver);
        break;
    case MSG_RequestVerificationCode_Id:
        pMsgConver = new MSG_RequestVerificationCode;
        pMsg->CopyPort(*pMsgConver);
        break;
    case MSG_AnswerVerificationCode_Id:
        pMsgConver = new MSG_AnswerVerificationCode;
        pMsg->CopyPort(*pMsgConver);
        break;
    case MSG_RequestLogin_Id:
        pMsgConver = new MSG_RequestLogin;
        pMsg->CopyPort(*pMsgConver);
        break;
    case MSG_AnswerLogin_Id:
        pMsgConver = new MSG_AnswerLogin;
        pMsg->CopyPort(*pMsgConver);
        break;
    case MSG_RequestProfileSync_Id:
        pMsgConver = new MSG_RequestProfileSync;
        pMsg->CopyPort(*pMsgConver);
        break;
    case MSG_AnswerProfileSync_Id:
        pMsgConver = new MSG_AnswerProfileSync;
        pMsg->CopyPort(*pMsgConver);
        break;
    case MSG_RequestClassList_Id:
        pMsgConver = new MSG_RequestClassList;
        pMsg->CopyPort(*pMsgConver);
        break;
    case MSG_AnswerClassList_Id:
        pMsgConver = new MSG_AnswerClassList;
        pMsg->CopyPort(*pMsgConver);
        break;
    case MSG_RequestClassInfo_Id:
        pMsgConver = new MSG_RequestClassInfo;
        pMsg->CopyPort(*pMsgConver);
        break;
    case MSG_AnswerClassInfo_Id:
        pMsgConver = new MSG_AnswerClassInfo;
        pMsg->CopyPort(*pMsgConver);
        break;
    case MSG_RequestEnterClassroom_Id:
        pMsgConver = new MSG_RequestEnterClassroom;
        pMsg->CopyPort(*pMsgConver);
        break;
    case MSG_AnswerEnterClassroom_Id:
        pMsgConver = new MSG_AnswerEnterClassroom;
        pMsg->CopyPort(*pMsgConver);
        break;
    }
    if (pMsg != NULL)
    {
        delete pMsgConver;
    }
}

}
}

#endif