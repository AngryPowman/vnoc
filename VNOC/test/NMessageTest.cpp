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
#include <shlwapi.h>
#include <atlstr.h>

bool DisposePath(
    IN const wchar_t* strPath,
    OUT std::string& strConvertDir
    )
{
    if (!strPath)
    {
        return false;
    }
    TCHAR szFile[MAX_PATH] = {0};
    ::GetModuleFileName(NULL, szFile, MAX_PATH - 1);
    ::PathRemoveFileSpec(szFile);
    ::PathAppend(szFile, strPath);
    strConvertDir = CW2A(szFile);
    return !strConvertDir.empty();
}

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

        std::string strPath;
        CPPUNIT_ASSERT(DisposePath(L"../test/msgdef.xml", strPath) == true);
        CPPUNIT_ASSERT(VNOC::Message::ParserMessageXML::Instance().LoadFile(strPath.c_str()) == VNOC::Message::MsgStatus_Ok);

        VNOC::Message::CMessage BaseTest("MSG_ALI");
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
        std::string strPath;

        test = VNOC::Message::ParserMessageXML::Instance().GetMsgObject("MSG_ALI");

        CPPUNIT_ASSERT(test->GetName() == "MSG_ALI");
        CPPUNIT_ASSERT(test->GetId() == 23);
        CPPUNIT_ASSERT(test->GetItem("LoginResult")->GetName() == "LoginResult");
        CPPUNIT_ASSERT(test->GetItem("LoginResult")->GetMType() == VNOC::Message::MsgDataMType_Data);
        CPPUNIT_ASSERT(test->GetItem("LoginResult")->GetType() == VNOC::Message::MsgDataType_Uint8);
    }


};
CPPUNIT_TEST_SUITE_REGISTRATION ( testNMessage );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(testNMessage, "testNMessage");