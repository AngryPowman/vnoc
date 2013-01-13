#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../../NMessage/NBaseMessage.h"
#include "../../NMessage/MsgDataValue/MsgDataValue.h"
#include "../../NMessage/MsgDataValue/UInt16Data.h"
#include <string.h>
class testNMessage : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( testNMessage );
    CPPUNIT_TEST( TestNMessage );
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp()
    {

    }
    void tearDown()
    {

    }
public:
    void TestNMessage()
    {
        VNOC::Message::UInt16Data Data;
        VNOC::Message::UInt16Data* pReadData = NULL;
        VNOC::Message::BaseMessage BaseTest;
        BaseTest.Write("10",Data);
        BaseTest.Read("10",(VNOC::Message::MsgDataValue *&)pReadData);
        CPPUNIT_ASSERT(pReadData != NULL);
    }
};
CPPUNIT_TEST_SUITE_REGISTRATION ( testNMessage );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(testNMessage, "testNMessage");