#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../UserManage.hpp"
#include "../FileUserStorage.h"

class CUserManageTest : public CppUnit::TestFixture
{
private:
    CPPUNIT_TEST_SUITE( CUserManageTest );
    CPPUNIT_TEST( AuthenticateTest );
    CPPUNIT_TEST_SUITE_END();
	
public:
    void setUp()
    {
		
    }
    void tearDown()
    {
        
    }
public:
	void AuthenticateTest()
	{
		fUserStorage us;
		CUserManage::GetInstance()->initial(&us);
		char strUser0[] = "user00"; //用户名不存在
		char strPassword0[40] = "00000000000000000000000000000000000000"; 
		userinfo stInfo0 = {};
		CPPUNIT_ASSERT(CUserManage::GetInstance()->Authenticate( strUser0, (char*)strPassword0, &stInfo0));
		char strUser1[] = "admin"; //成功登陆
		char strPassword1[40] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
		userinfo stInfo1 = {};
		CPPUNIT_ASSERT(CUserManage::GetInstance()->Authenticate( strUser1, (char*)strPassword1, &stInfo1));
		char strUser2[] = "able"; //密码错误
		char strPassword2[40] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
		userinfo stInfo2 = {};
		CPPUNIT_ASSERT(CUserManage::GetInstance()->Authenticate( strUser2, (char*)strPassword2, &stInfo2));

    }

};
CPPUNIT_TEST_SUITE_REGISTRATION ( CUserManageTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(CUserManageTest, "CUserManageTest");