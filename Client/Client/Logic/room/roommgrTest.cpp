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
    std::vector<int> roomlist;

    CRoomMgrTest() : CTestBase()
    {
        for(int i = 0; i < 10; i++)
        {
            roomlist.push_back(i);
        }
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

        MSG_ACL netMsg;
        netMsg.SetRoomList(roomlist);
        pNetCenter->MockReceive(&netMsg);
    }

    VOID OnGetRoomListResult(XMessage_GetRoomList_Result *pmsg)
    {
        CPPUNIT_ASSERT(roomlist == pmsg->roomID);
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(CRoomMgrTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(CRoomMgrTest, "testRoomMgr");

#endif