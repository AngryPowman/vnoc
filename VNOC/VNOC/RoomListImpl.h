#pragma once
#include "IRoomList.h"
#include "IVNOCFrame.h"
#include "RoomListDlg.h"

class CRoomListImpl
	: public IRoomList
{
public:
	CRoomListImpl(void);
	~CRoomListImpl(void);
public:
	STDMETHOD( Initialize(IModule* UpperFrame=NULL) );
	STDMETHOD( UnInitialize() );
	STDMETHOD( Run() );
	STDMETHOD( Terminate() );
	STDMETHOD( Show(BOOL bShow=TRUE) );
private:
	IVNOCFrame* m_frame;
};

