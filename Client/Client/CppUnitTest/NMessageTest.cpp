#ifdef CPPTEST
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../../NMessage/CMessage.h"
#include "../../NMessage/MsgDataValue/MsgDataValue.h"
#include "../../NMessage/MsgDataValue/StringData.h"
#include "../../NMessage/ParaserMessageXML.h"
#include "../../NMessage/XMLObject.h"
#include <Windows.h>
#include <string.h>
#include <shlwapi.h>
#include <atlstr.h>
#include "../../NMessage/MsgDataValue/NumData.hpp"
#include "../../NMessage/Message2Pack.h"
#include "../../NMessage/NMSG_ALI.h"
#include "../../NMessage/NMSG_ACL.h"

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
        MsgDataValue* pUInt16 = new NumData<uint16>(8);
        MsgDataValue* pReadData = NULL;

        std::string strPath;
        CPPUNIT_ASSERT(DisposePath(L"msgdef.xml", strPath) == true);
        CPPUNIT_ASSERT(ParserMessageXML::Instance().LoadFile(strPath.c_str()) == MsgStatus_Ok);

        CMessage BaseTest("NMSG_ALI");
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

        test = ParserMessageXML::Instance().GetMsgObject("NMSG_ALI");

        CPPUNIT_ASSERT(test->GetName() == "NMSG_ALI");
        CPPUNIT_ASSERT(test->GetId() == 23);
        CPPUNIT_ASSERT(test->GetItem("LoginResult")->GetName() == "LoginResult");
        CPPUNIT_ASSERT(test->GetItem("LoginResult")->GetMType() == MsgDataMType_Data);
        CPPUNIT_ASSERT(test->GetItem("LoginResult")->GetType() == MsgDataType_Uint8);
    }

    void TestPack()
    {
        CMessage2Pack m2p;
        CMessage BaseTest("NMSG_ALI");
        BaseTest.Write("LoginResult", new NumData<uint16>(8));
        BaseTest.Write("Token", new NumData<uint32>(811));
        BaseTest.Write("ATLGUID", new StringData("hello"));
        int nSize = 0;
        CPPUNIT_ASSERT(m2p.GetPackSize(&BaseTest, nSize) == MsgStatus_Ok);
        CPPUNIT_ASSERT(nSize == 25);

        byte TestBufALI[25] =
        {
        MSG_BEGIN,MSG_VER,0x17,0x00,0x00,0x00,0x19,0x00,0x00,0x00,
        0x05,0x00,0x00,0x00,'h','e','l','l','o',0x08,0x08,0x00,0x00,0x00,
        MSG_END
        };
        NMSG_ALI ali;
        CBufferMessage buf;
        ali.SetLoginResult(8);
        ali.SetToken(8);
        ali.SetATLGUID("hello");
        m2p.PackMessage(&ali,buf);
        byte* TestCompBuf = buf.GetBuffer();

        int Success = 1;
        for (uint32 index = 0; index < buf.GetSize(); index++)
        {
            if (TestBufALI[index] != TestCompBuf[index])
            {
                Success = 0;
                std::cout<<index<<std::endl;
                std::cout<<TestBufALI[index]<<std::endl;
                std::cout<<TestCompBuf[index]<<std::endl;
                break;
            }
        }
        CPPUNIT_ASSERT(Success == 1);

        byte TestBuf[100] = {0};
        byte TestBufACL[] =
        {
            MSG_BEGIN,MSG_VER,0x1F,0x00,0x00,0x00,0x1B,0x00,0x00,0x00,
            0x03,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x03,0x00,0x00,0x00,
            MSG_END
        };
        NMSG_ACL acl;
        std::vector<uint32> TestArr;
        TestArr.push_back(1);
        TestArr.push_back(2);
        TestArr.push_back(3);
        acl.SetRoomList(TestArr);
        buf.SetValue(0);
        m2p.PackMessage(&acl,buf);
        TestCompBuf = buf.GetBuffer();
        memcpy(TestBuf, buf.GetBuffer(), buf.GetSize());

        Success = 1;
        for (uint32 index = 0; index < buf.GetSize(); index++)
        {
            if (TestBufACL[index] != TestCompBuf[index])
            {
                Success = 0;
                std::cout<<index<<std::endl;
                std::cout<<TestBufACL[index]<<std::endl;
                std::cout<<TestCompBuf[index]<<std::endl;
                break;
            }
        }
        CPPUNIT_ASSERT(Success == 1);
    }


};
CPPUNIT_TEST_SUITE_REGISTRATION ( testNMessage );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(testNMessage, "testNMessage");

#endif