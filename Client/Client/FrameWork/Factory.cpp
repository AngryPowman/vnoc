#include "stdafx.h"
#include "Factory.h"

#include "../Logic/room/Room.h"
#include "../Logic/room/RoomMgr.h"
#include "../Logic/UserSession/LoginImpl.h"

#include "../GUI/LoginWin/BKWinLogin.h"
#include "../GUI/RoomListWin/BKWinRoomList.h"
#include "../gui/RoomWin/BKWinClassroom.h"

#ifdef CPPTEST
#include "../CppUnitTest/testRunner.h"
#endif

ResultCode CFrameModuleFactory::CreateFrameModule( FrameModule module,IFrameModule** ppModule )
{
	if (!ppModule)
	{
		return Result_InvalidParam;
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
	case module_ClassroomWinData:
		*ppModule = new CRoomBase;
		break;
	case module_ClassroomWin:
		*ppModule = new CClassroomWnd;
		break;
#ifdef CPPTEST
	case module_CppTestMain:
		*ppModule = new CTestRunner;
		break;
#endif
	default:
		ATLASSERT(FALSE && "invalid module");
		return Result_NotFound;
	}
	return Result_Success;
}
