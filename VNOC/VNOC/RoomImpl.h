#pragma once
#include "IVNOCFrame.h"
#include "IRoom.h"
#include "RoomDlg.h"

class CRoomImpl : public IRoom
{
public:
	CRoomImpl(void);
	~CRoomImpl(void);
public:
	STDMETHOD( Initialize(IModule* UpperFrame=NULL) );
	STDMETHOD( UnInitialize() );
	STDMETHOD( Run() );
	STDMETHOD( Terminate() );
	STDMETHOD( Show(BOOL bShow=TRUE) );
private:
	IVNOCFrame* m_frame;
};

