#ifdef CPPTEST
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
#include "../../NMessage/MsgDataValue/UInt32Data.h"
#include "../../NMessage/Message2Pack.h"

using namespace VNOC::Message;

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
    CPPUNIT_TEST( TestPack );
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
        MsgDataValue* pUInt16 = new UInt16Data(8);
        MsgDataValue* pReadData = NULL;

        std::string strPath;
        CPPUNIT_ASSERT(DisposePath(L"msgdef.xml", strPath) == true);
        CPPUNIT_ASSERT(ParserMessageXML::Instance().LoadFile(strPath.c_str()) == MsgStatus_Ok);

        CMessage BaseTest("MSG_ALI");
        BaseTest.Write("LoginResult", pUInt16);
        CPPUNIT_ASSERT(BaseTest.Write("LoginResult2", pUInt16) != MsgStatus_Ok);
        CPPUNIT_ASSERT(BaseTest.Read("LoginResult", pReadData) == MsgStatus_Ok);
        uint16 num = 0;
        pReadData->ToUInt16(num);
        CPPUNIT_ASSERT(num == 8);

        MsgDataValue* pStr = new StringData(std::string("hello"));
        BaseTest.Write("ATLGUID", pStr);
        BaseTest.Read("ATLGUID", pReadData);
        std::string str = "";
        pReadData->ToStr(str);
        CPPUNIT_ASSERT(str == "hello");
    }
    void TestNMessageXML()
    {
        XMLObject* test = NULL;
        std::string strPath;

        test = ParserMessageXML::Instance().GetMsgObject("MSG_ALI");

		CPPUNIT_ASSERT(test);
		if (test)
		{
			CPPUNIT_ASSERT(test->GetName() == "MSG_ALI");
			CPPUNIT_ASSERT(test->GetId() == 23);
			CPPUNIT_ASSERT(test->GetItem("LoginResult")->GetName() == "LoginResult");
			CPPUNIT_ASSERT(test->GetItem("LoginResult")->GetMType() == MsgDataMType_Data);
			CPPUNIT_ASSERT(test->GetItem("LoginResult")->GetType() == MsgDataType_Uint8);
		}
    }

    void TestPack()
    {
        CMessage2Pack m2p;
        CMessage BaseTest("MSG_ALI");
        BaseTest.Write("LoginResult", new UInt16Data(8));
        BaseTest.Write("Token", new UInt32Data(811));
        BaseTest.Write("ATLGUID", new StringData("hello"));
        int nSize = 0;
        CPPUNIT_ASSERT(m2p.GetPackSize(&BaseTest, nSize) == MsgStatus_Ok);
        CPPUNIT_ASSERT(nSize == 22); // 8(head) + 4Token + 1LoginResult + 4ATLGUID +5hello
    }


};
CPPUNIT_TEST_SUITE_REGISTRATION ( testNMessage );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(testNMessage, "testNMessage");

#endif