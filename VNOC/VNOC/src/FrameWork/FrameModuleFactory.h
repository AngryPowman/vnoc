#pragma once

#include "Dialogs/BKWinLogin.h"
#include "Dialogs/BKWinRoomList.h"
#include "Dialogs\BKWinClassroom.h"
#include "../Logic/room/RoomMgr.h"
#include "../Logic/UserSession/LoginImpl.h"

class CFrameModuleFactory
{
public:
	/*static CFrameModuleFactory& GetInstance()
	{
		static CFrameModuleFactory factory;
		return factory;
	}*/

	static HRESULT CreateFrameModule(FrameModule module,IFrameModule** ppModule)
	{
		if (!ppModule)
		{
			return E_INVALIDARG;
		}
		*ppModule = NULL;
		switch(module)
		{
		case module_LoginData:
			*ppModule = new CLoginImpl;
			break;
		case module_LoginWin:
			*ppModule = new CLoginWnd;
			break;
		case module_RoomListData:
			*ppModule = new CRoomMgr;
			break;
		case module_RoomListWin:
			*ppModule = new CRoomListWnd;
			break;
		case module_ClassroomWin:
			*ppModule = new CClassroomWnd;
			break;
		case module_ClassroomWinData:
			// TODO: 没有找到对应的类, 在此添加相应处理
			//       May like this: *ppModule = new CClassroom???
			break;
		default:
			ATLASSERT(FALSE && "invalid module");
			return E_FAIL;
		}
		return S_OK;
	}
};
