#include "stdafx.h"
#include "TestClassHeader.h"

class CUserSessionTest : public CTestBase<TestUnit_UserSession>
{
	CPPUNIT_TEST_SUITE( CUserSessionTest );
	CPPUNIT_TEST( TestLoginRequest );
	CPPUNIT_TEST_SUITE_END();
	
public:
	UINT	m_testCount;
	CUserSessionTest():CTestBase(), m_testCount(0)
	{}

	Begin_XMessage(CUserSessionTest)
		OnXMessage(XMessage_LoginError,OnLoginError);
	End_XMessage();

	VOID TestLoginRequest()
	{
		XMessage_Login msg;
		msg.username = _T("");
		msg.pwd = _T("");
		SendXMessage(&msg);

		msg.username = _T("123");
		SendXMessage(&msg);

		msg.username = _T("sparta");
		SendXMessage(&msg);

		CPPUNIT_ASSERT(m_testCount == 3);
	}

	VOID OnLoginError( XMessage_LoginError* msg )
	{
		if (ResultFailed(msg->result))
		{
			++m_testCount;
		}
		CPPUNIT_ASSERT(ResultFailed(msg->result));
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION ( CUserSessionTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(CUserSessionTest, "testUserSession");
