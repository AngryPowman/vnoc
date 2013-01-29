#pragma once
#include "IFrameWork.h"
#include "IRefCountImpl.h"
#include <list>
#include <vector>

struct XMessage : public IRefCountImpl
{
	virtual VOID GetID(CString& id) = 0;

	XMessage()
		: destModule(module_Any)
		, srcModule(module_Any)
		, result(Result_NotSet)
	{
	}

	VOID MarkProcessed(ResultCode result)
	{
		ATLASSERT(result != Result_NotSet);
		this->result = result;
	}
	BOOL IsProcessed()
	{
		return result != Result_NotSet;
	}

	FrameModule	destModule;
	FrameModule srcModule;

	ResultCode	result;
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
	ResultCode SendXMessage(XMessage* msg)
	{
		ATLASSERT(msg);
		IFrameWork* pFrameWork;
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
	virtual ResultCode Initialize(IModule* UpperFrame=NULL){return Result_Success;};
	virtual ResultCode UnInitialize(){return Result_Success;};
	virtual ResultCode Run(){return Result_Success;};
	virtual ResultCode Terminate(){return Result_Success;};
protected:
	FrameModule	m_moduleType;
};

#define CarelessXMessage(_class) \
	virtual VOID _class::ProcessXMessage(XMessage* pmsg,XMessage* pMsgGetList=NULL) \
	{}

#define Begin_XMessage(_class)	\
	virtual VOID _class::ProcessXMessage(XMessage* pmsg,XMessage* pMsgGetList=NULL) \
	{ \
		CString msgID; \
		XMessage_GetListenList* pListMsg=NULL; \
		if (!pMsgGetList) \
		{ \
			if (!pmsg) \
				return; \
			pmsg->GetID(msgID); \
		} \
		else \
		{ \
			pListMsg = dynamic_cast<XMessage_GetListenList*>(pMsgGetList); \
			if (!pListMsg) \
			{ \
				return; \
			} \
		} \

#define OnXMessage(_msg,_proc)	\
		if (pListMsg) \
		{ \
			CString id; \
			_msg::Static_GetID(id); \
			pListMsg->msgIDList.push_back(id); \
		} \
		if ( pmsg && _msg::Static_IsTypeOf(msgID) ) \
		{ \
			_proc((_msg*)pmsg); \
		}

#define Chain_XMessage(_class)	\
		{} \
		{ \
			_class::ProcessXMessage(pmsg); \
		}

#define End_XMessage()	 \
		{} \
	}

#define ImTheMessageOf(_msg)	\
public: \
	virtual VOID GetID(CString& id) \
	{ \
		Static_GetID(id); \
	} \
	static VOID Static_GetID(CString& id) \
	{ \
		id = #_msg; \
	} \
	static BOOL Static_IsTypeOf(LPCTSTR id) \
	{ \
		CString tempID; \
		Static_GetID(tempID); \
		return (tempID.CompareNoCase(id)==0)? TRUE: FALSE; \
	}
