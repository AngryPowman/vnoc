#include "stdafx.h"
#include "TestClassHeader.h"

VOID CUserSessionTest::TestLoginRequest()
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

VOID CUserSessionTest::OnLoginError( XMessage_LoginError* msg )
{
	if (ResultFailed(msg->result))
	{
		++m_testCount;
	}
	CPPUNIT_ASSERT(ResultFailed(msg->result));
}

CPPUNIT_TEST_SUITE_REGISTRATION ( CUserSessionTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(CUserSessionTest, "testUserSession");
