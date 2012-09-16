#pragma once
#include "IFrameWork.h"
#include <atlcomcli.h>

class CFrameBase : public IFrameAdapter
{
public:
	CFrameBase()
	{
		CComPtr<IFrameWork> pFrameWork;
		Global->GetIFrameModule(&pFrameWork);
		Global->PtrAssert(pFrameWork);
		pFrameWork->AddActor(this);
	}
	virtual ~CFrameBase()
	{
		CComPtr<IFrameWork> pFrameWork;
		Global->GetIFrameModule(&pFrameWork);
		if (pFrameWork)
		{
			pFrameWork->RemoveActor(this);
		}
	}
	VOID SendXMessage(XMessage* msg)
	{
		ATLASSERT(msg);
		CComPtr<IFrameWork> pFrameWork;
		Global->GetIFrameModule(&pFrameWork);
		pFrameWork->SendXMessage(msg);
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

#define XMessageID_Login	_T("Login")
struct XMessage_Login : public XMessage
{
	XMessage_Login():XMessage(XMessageID_Login){}
	CString username;
	CString pwd;
};

#define XMessageID_Login_Result	_T("LoginResult")
struct XMessage_Login_Result : public XMessage
{
	XMessage_Login_Result():XMessage(XMessageID_Login_Result){}
	BOOL success;
	DWORD	userToken;
	BYTE	guid[16];
};
