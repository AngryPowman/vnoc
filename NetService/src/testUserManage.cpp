#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "UserManage.hpp"


class CUserManageTest : public CppUnit::TestFixture
{
private:
	CUserManage um;
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
		
		string strUser = "user00";
		char strPassword[20] = "0000000000000000000"; 
		userinfo stInfo = {};
		CPPUNIT_ASSERT(um.Authenticate(strUser, (char*)strPassword, &stInfo));
        
    }

};
