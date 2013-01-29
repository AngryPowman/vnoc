#pragma once

//#include "../Logic/room/Room.h"
//#include "../Logic/room/RoomMgr.h"
//#include "../Logic/UserSession/LoginImpl.h"

#ifdef CPPTEST
#include "../CppUnitTest/testRunner.h"
#endif


class CFrameModuleFactory
{
public:
	/*static CFrameModuleFactory& GetInstance()
	{
		static CFrameModuleFactory factory;
		return factory;
	}*/

	static ResultCode CreateFrameModule(FrameModule module,IFrameModule** ppModule)
	{
		if (!ppModule)
		{
			return Result_InvalidParam;
		}
		*ppModule = NULL;
		switch(module)
		{
		case module_LoginData:
			//*ppModule = new CLoginImpl;
			break;
		case module_LoginWin:
			//*ppModule = new CLoginWnd;
			break;
		case module_RoomListData:
			//*ppModule = new CRoomMgr;
			break;
		case module_RoomListWin:
			//*ppModule = new CRoomListWnd;
			break;
		case module_ClassroomWinData:
			//*ppModule = new CRoomBase;
			break;
		case module_ClassroomWin:
			//*ppModule = new CClassroomWnd;
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
};
