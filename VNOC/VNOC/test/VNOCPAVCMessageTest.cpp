#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
class VnocpAVCMessageTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( VnocpAVCMessageTest );
    CPPUNIT_TEST( simpleTest );
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp()
    {
        
    }
    void tearDown()
    {

    }
public:
    void simpleTest()
    {
        CPPUNIT_ASSERT(true);
    }

};
CPPUNIT_TEST_SUITE_REGISTRATION ( VnocpAVCMessageTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(VnocpAVCMessageTest, "VnocpAVCMessageTest");