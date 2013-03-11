#ifdef CPPTEST

#include "stdafx.h"
#include <vector>
#include "TestClassHeader.h"

class CRoomMgrTest : public CTestBase<TestUnit_RoomMgr>
{
    CPPUNIT_TEST_SUITE(CRoomMgrTest);
    CPPUNIT_TEST(TestGetRoomList);
    CPPUNIT_TEST_SUITE_END();

public:
    std::vector<uint32> roomIdList;
    std::vector<std::string> roomNameList;
    std::vector<uint32> roomState;

    CRoomMgrTest() : CTestBase()
    {
    }

    Begin_XMessage(CRoomMgrTest)
        OnXMessage(XMessage_GetRoomList_Result, OnGetRoomListResult);
    End_XMessage();

    VOID TestGetRoomList()
    {
        INetCenter* pNetCenter=NULL;
        Global->GetINetCenter(&pNetCenter);
        CPPUNIT_ASSERT(pNetCenter);
        if(!pNetCenter)
        {
            return;
        }

        MSG_AnswerClassList netMsg;
        roomIdList.push_back(1);
        roomNameList.push_back("a");
        roomState.push_back(0);

        roomIdList.push_back(2);
        roomNameList.push_back("b");
        roomState.push_back(1);

        netMsg.SetRoomIdList(roomIdList);
        netMsg.SetRoomNameList(roomNameList);
        netMsg.SetRoomStateList(roomState);

        pNetCenter->MockReceive(&netMsg);
    }

    VOID OnGetRoomListResult(XMessage_GetRoomList_Result *pmsg)
    {
        CPPUNIT_ASSERT(roomIdList == pmsg->roomIdList);
        CPPUNIT_ASSERT(roomNameList == pmsg->roomNameList);
        CPPUNIT_ASSERT(roomState == pmsg->roomStateList);
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(CRoomMgrTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(CRoomMgrTest, "testRoomMgr");

#endif