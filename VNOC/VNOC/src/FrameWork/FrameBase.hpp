#pragma once
#include "IFrameWork.h"
#include <atlcomcli.h>

#define HR_NOTPROCESSED	MAKE_HRESULT(1,0x590c,0x590c)

struct XMessage : public IRefCountImpl
{
	XMessage(LPCTSTR msgID,FrameModule destModule=module_Any,IFrameModule* srcFrameModule=NULL)
		: msgID(msgID),destModule(destModule),result(HR_NOTPROCESSED)
	{
		if (srcFrameModule)
		{
			srcModule = srcFrameModule->GetModuleType();
		}
	}
	VOID MarkProcessed(HRESULT result)
	{
		ATLASSERT(result != HR_NOTPROCESSED);
		this->result = result;
	}
	BOOL IsProcessed()
	{
		return result != HR_NOTPROCESSED;
	}

	CString		msgID;
	FrameModule	destModule;
	FrameModule srcModule;

	HRESULT	result;
};

class CFrameBase : public IFrameModule
{
public:
	CFrameBase(FrameModule module)
		: m_moduleType(module)
	{
	}
	virtual ~CFrameBase()
	{
	}
	VOID SendXMessage(XMessage* msg)
	{
		ATLASSERT(msg);
		CComPtr<IFrameWork> pFrameWork;
		Global->GetIFrameModule(&pFrameWork);
		Global->PtrAssert(pFrameWork);
		pFrameWork->SendXMessage(msg);
	}
	FrameModule GetModuleType()
	{
		return m_moduleType;
	}
	virtual BOOL IsView()
	{
		return GetModuleType()>module_ViewBegin && GetModuleType()<module_ViewEnd;
	}
	STDMETHOD( Initialize(IModule* UpperFrame=NULL){return S_OK;} );
	STDMETHOD( UnInitialize(){return S_OK;} );
	STDMETHOD( Run(){return S_OK;} );
	STDMETHOD( Terminate(){return S_OK;} );
protected:
	FrameModule	m_moduleType;
};

#define Begin_XMessage(_class)	virtual VOID _class::ProcessXMessage(XMessage* pmsg){
#define OnXMessage(_msg,_proc)	if(pmsg->msgID==_msg) \
{ \
	_proc(pmsg); \
	if(pmsg->IsProcessed()) \
		return; \
}else
#define Chain_XMessage(_class)	{}{_class::ProcessXMessage(pmsg);if(pmsg->IsProcessed())return;}
#define End_XMessage()			{}}

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

#define XMessageID_ShowLogin	_T("ShowLogin")
struct XMessage_ShowLogin : public XMessage
{};

#define XMessageID_ShowRoomList	_T("ShowRoomList")
struct XMessage_ShowRoomList : public XMessage
{
	XMessage_ShowRoomList():XMessage(XMessageID_ShowRoomList){}
};
