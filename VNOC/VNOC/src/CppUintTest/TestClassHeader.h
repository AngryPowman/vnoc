#include "../FrameWork/FrameBase.hpp"

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

enum TestUnitID
{
	TestUnit_Begin = module_CppTest_PrivateModule_Begin,
	TestUnit_UserSession,
	TestUnit_End = module_CppTest_PrivateModule_End
};

template<typename TestUnitID TID>
class CTestBase : public CppUnit::TestFixture ,public CFrameBase
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
	CTestBase():CFrameBase((FrameModule)(UINT)TID){};
};

class CUserSessionTest :public CTestBase<TestUnit_UserSession>
{
public:
	VOID	TestLoginRequest();
	VOID	OnLoginError(XMessage_LoginError* msg);

	Begin_XMessage(CUserSessionTest)
		OnXMessage(XMessage_LoginError,OnLoginError);
	End_XMessage();

	UINT	m_testCount;
	CUserSessionTest():CTestBase(), m_testCount(0)
	{}

	CPPUNIT_TEST_SUITE( CUserSessionTest );
	CPPUNIT_TEST( TestLoginRequest );
	CPPUNIT_TEST_SUITE_END();
};
