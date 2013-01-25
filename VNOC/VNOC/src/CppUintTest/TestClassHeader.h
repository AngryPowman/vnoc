#include "../FrameWork/FrameBase.hpp"

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class CTestBase : public CppUnit::TestFixture
{
public:
};

class CUserSessionTest :public CTestBase ,public CFrameBase
{
public:
	void setUp()
	{
		CComPtr<IFrameWork> pFrameWork;
		Global->GetIFrameModule(&pFrameWork);
		pFrameWork->RegisterModule(this);
	}
	void tearDown()
	{
		CComPtr<IFrameWork> pFrameWork;
		Global->GetIFrameModule(&pFrameWork);
		AddRef();	// 防止引用计数为0被销毁
		pFrameWork->RemoveModule(this);
	}
	VOID	TestLoginRequest();
	VOID	OnLoginError(XMessage_LoginError* msg);

	Begin_XMessage(CUserSessionTest)
		OnXMessage(XMessage_LoginError,OnLoginError);
	End_XMessage();

	UINT	m_testCount;
	CUserSessionTest():CFrameBase(module_CppTest_PrivateModule_UserSession), m_testCount(0)
	{}

	CPPUNIT_TEST_SUITE( CUserSessionTest );
	CPPUNIT_TEST( TestLoginRequest );
	CPPUNIT_TEST_SUITE_END();
};
