#pragma once
#include "IFrameWork.h"

class CFrameBase : public IFrameAdapter
{
public:
	CFrameBase()
	{
		IFrameWork* pFrameWork;
		Global->GetIFrameModule(&pFrameWork);
		Global->PtrAssert(pFrameWork);
		pFrameWork->AddActor(this);
	}
	virtual ~CFrameBase()
	{
		IFrameWork* pFrameWork;
		Global->GetIFrameModule(&pFrameWork);
		if (pFrameWork)
		{
			pFrameWork->RemoveActor(this);
		}
	}
	VOID SendXMessage(XMessage* msg)
	{
		ATLASSERT(msg);
		IFrameWork* pFrameWork;
		Global->GetIFrameModule(&pFrameWork);
		pFrameWork->SendXMessage(msg);
		pFrameWork->Release();
	}
};

#define Begin_XMessage(_class)	BOOL _class::ProcessXMessage(XMessage* pmsg){
#define OnXMessage(_msg,_proc)	if(pmsg->msgID==_msg) \
{ \
	if(_proc(pmsg)) \
		return TRUE; \
}else
#define OnUIXMessage(_uiid,_msg,_proc)	if(_uiid==pmsg->uiID && pmsg->msgID==_msg) \
{ \
	if(_proc(pmsg)) \
	return TRUE; \
}else
#define Chain_XMessage(_class)	{}{BOOL bRet = _class::ProcessXMessage(pmsg);if(bRet)return bRet;}
#define End_XMessage()			{}return FALSE;}

//////////////////////////////////////////////////////////////////////////
// Messages

#define XMessageID_Login	1024
struct XMessage_Login : public XMessage
{
	XMessage_Login():XMessage(XMessageID_Login){}
	CString username;
	CString pwd;
};
