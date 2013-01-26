#include "stdafx.h"
#include "TestClassHeader.h"

class CUserSessionTest : public CTestBase<TestUnit_UserSession>
{
	CPPUNIT_TEST_SUITE( CUserSessionTest );
	CPPUNIT_TEST( TestLoginRequest );
	CPPUNIT_TEST_SUITE_END();
	
public:
	UINT	m_testCount;
	BOOL	m_receivedResult;
	CUserSessionTest():CTestBase(), m_testCount(0),m_receivedResult(FALSE)
	{}

	Begin_XMessage(CUserSessionTest)
		OnXMessage(XMessage_LoginError,OnLoginError);
		OnXMessage(XMessage_Login_Result,OnLoginResult);
	End_XMessage();

	VOID TestLoginRequest()
	{
		INetCenter* pNetCenter=NULL;
		Global->GetINetCenter(&pNetCenter);
		CPPUNIT_ASSERT(pNetCenter);
		if (!pNetCenter)
		{
			return;
		}

		XMessage_Login msg;
		msg.username = _T("");
		msg.pwd = _T("");
		SendXMessage(&msg);

		msg.username = _T("123");
		SendXMessage(&msg);

		msg.username = _T("sparta");
		SendXMessage(&msg);

		MSG_ALI netMsg;
		netMsg.SetLoginResult(0);
		pNetCenter->MockReceive(&netMsg);

		CPPUNIT_ASSERT(m_testCount == 3);
		CPPUNIT_ASSERT(m_receivedResult);
	}

	VOID OnLoginError( XMessage_LoginError* msg )
	{
		if (ResultFailed(msg->result))
		{
			++m_testCount;
		}
		CPPUNIT_ASSERT(ResultFailed(msg->result));
	}

	VOID OnLoginResult( XMessage_Login_Result* msg)
	{
		CPPUNIT_ASSERT(msg->success);
		m_receivedResult = TRUE;
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION ( CUserSessionTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(CUserSessionTest, "testUserSession");
