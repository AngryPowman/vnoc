#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../../NMessage/NBaseMessage.h"
#include "../../NMessage/MsgDataValue/MsgDataValue.h"
#include "../../NMessage/MsgDataValue/StringData.h"
#include "../../NMessage/MsgDataValue/UInt16Data.h"
#include "../../NMessage/ParaserMessageXML.h"
#include "../../NMessage/XMLObject.h"
#include <string.h>
class testNMessage : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( testNMessage );
    CPPUNIT_TEST( TestNMessage );
    CPPUNIT_TEST( TestNMessageXML );
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
        VNOC::Message::UInt16Data Data(8);
        VNOC::Message::MsgDataValue* pReadData = NULL;
        VNOC::Message::BaseMessage BaseTest;
        BaseTest.Write("10",Data);
        BaseTest.Read("10",pReadData);
        VNOC::Message::uint16 num = 0;
        pReadData->ToUInt16(num);
        CPPUNIT_ASSERT(num == 8);

        VNOC::Message::StringData strData(std::string("hello"));
        BaseTest.Write("11", strData);
        BaseTest.Read("11", pReadData);
        std::string str = "";
        pReadData->ToStr(str);
        CPPUNIT_ASSERT(str == "hello");
    }
    void TestNMessageXML()
    {
        VNOC::Message::XMLObject* test = NULL;
        VNOC::Message::ParserMessageXML xml;
        xml.LoadFile("../test/msgdef.xml");
        test = xml.GetOjbect("MSG_ALI");
        CPPUNIT_ASSERT(test->GetName() == "MSG_ALI");
        CPPUNIT_ASSERT(test->GetId() == 33);
        CPPUNIT_ASSERT(test->GetItem("LoginResult")->GetName() == "LoginResult");
        CPPUNIT_ASSERT(test->GetItem("LoginResult")->GetMType() == "data");
        CPPUNIT_ASSERT(test->GetItem("LoginResult")->GetType() == "byte");
    }
};
CPPUNIT_TEST_SUITE_REGISTRATION ( testNMessage );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(testNMessage, "testNMessage");