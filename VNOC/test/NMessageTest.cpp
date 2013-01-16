#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../../NMessage/CMessage.h"
#include "../../NMessage/MsgDataValue/MsgDataValue.h"
#include "../../NMessage/MsgDataValue/StringData.h"
#include "../../NMessage/MsgDataValue/UInt16Data.h"
#include "../../NMessage/ParaserMessageXML.h"
#include "../../NMessage/XMLObject.h"
#include <Windows.h>
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
        VNOC::Message::CMessage BaseTest;
        VNOC::Message::ParserMessageXML xml;
        char szPath[MAX_PATH + 1] = {0};
        ::GetModuleFileNameA(NULL,szPath,MAX_PATH);
        std::string vstrPath = szPath;
        std::string vstrDisposePath;
        int Pos = vstrPath.find("vnoc");
        vstrDisposePath.resize(Pos + 4);
        std::copy(vstrPath.begin(),vstrPath.begin()+(Pos + 4),vstrDisposePath.begin());
        OutputDebugStringA(szPath);
        std::cout<<szPath<<std::endl;
        vstrDisposePath += "/VNOC/test/msgdef.xml";
        CPPUNIT_ASSERT(xml.LoadFile("../../test/msgdef.xml") == VNOC::Message::MsgStatus_Ok);
        BaseTest.SetMessage("MSG_ALI",xml);
        BaseTest.Write("LoginResult",Data);
        BaseTest.Read("LoginResult",pReadData);
        VNOC::Message::uint16 num = 0;
        pReadData->ToUInt16(num);
        CPPUNIT_ASSERT(num == 8);

        VNOC::Message::StringData strData(std::string("hello"));
        BaseTest.Write("ATLGUID", strData);
        BaseTest.Read("ATLGUID", pReadData);
        std::string str = "";
        pReadData->ToStr(str);
        CPPUNIT_ASSERT(str == "hello");
    }
    void TestNMessageXML()
    {
        VNOC::Message::XMLObject* test = NULL;
        VNOC::Message::ParserMessageXML xml;
        char szPath[MAX_PATH + 1] = {0};
        ::GetModuleFileNameA(NULL,szPath,MAX_PATH);
        std::string vstrPath = szPath;
        std::string vstrDisposePath;
        int Pos = vstrPath.find("vnoc");
        vstrDisposePath.resize(Pos + 4);
        std::copy(vstrPath.begin(),vstrPath.begin()+(Pos + 4),vstrDisposePath.begin());
        vstrDisposePath += "/VNOC/test/msgdef.xml";
        OutputDebugStringA(szPath);
        std::cout<<szPath<<std::endl;
        CPPUNIT_ASSERT(xml.LoadFile("../../test/msgdef.xml") == VNOC::Message::MsgStatus_Ok);
        test = xml.GetMsgObject("MSG_ALI");

        CPPUNIT_ASSERT(test->GetName() == "MSG_ALI");
        CPPUNIT_ASSERT(test->GetId() == 23);
        CPPUNIT_ASSERT(test->GetItem("LoginResult")->GetName() == "LoginResult");
        CPPUNIT_ASSERT(test->GetItem("LoginResult")->GetMType() == VNOC::Message::MsgDataMType_Data);
        CPPUNIT_ASSERT(test->GetItem("LoginResult")->GetType() == VNOC::Message::MsgDataType_Byte);
    }
};
CPPUNIT_TEST_SUITE_REGISTRATION ( testNMessage );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(testNMessage, "testNMessage");