#ifdef CPPTEST
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <Windows.h>
#include <string.h>
#include <shlwapi.h>
#include <atlstr.h>
#include <string>
#include <vector>
#include "../../NMessage/MessageUnion.h"
#include "../../NMessage/NMSG_ACL.h"
#include "../../NMessage/Message2Pack.h"
#include "../../NMessage/Message2Parser.h"
#include "../../NMessage/ParserMessageXML.h"
#include "../../NMessage/BufferMessage.h"

using namespace VNOC::Message;

CMessage2Pack g_m2pack;
CMessage2Parser g_m2parser;

bool _TDisposePath(
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

class MSG_Message_Test : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( MSG_Message_Test );
    CPPUNIT_TEST( _LoadXML );
    CPPUNIT_TEST( MSG_AnswerClassInfo_Test );
    CPPUNIT_TEST( MSG_AnswerClassList_Test );
    CPPUNIT_TEST( MSG_AnswerLogin_Test );
    CPPUNIT_TEST( MSG_AnswerProfileSync_Test );
    CPPUNIT_TEST( MSG_AnswerRegister_Test );
    CPPUNIT_TEST( MSG_AnswerVerificationCode_Test );
    CPPUNIT_TEST( MSG_RequestClassInfo_Test );
    CPPUNIT_TEST( MSG_RequestClassList_Test );
    CPPUNIT_TEST( MSG_RequestLogin_Test );
    CPPUNIT_TEST( MSG_RequestProfileSync_Test );
    CPPUNIT_TEST( MSG_RequestRegister_Test );
    CPPUNIT_TEST( MSG_RequestVerificationCode_Test );
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp()
    {

    }
    void tearDown()
    {

    }
public:

    void _LoadXML()
    {
        std::string strPath;
        CPPUNIT_ASSERT(_TDisposePath(L"NewMsgdef.xml", strPath) == true);
        CPPUNIT_ASSERT(ParserMessageXML::Instance().LoadFile(strPath.c_str()) == MsgStatus_Ok);
    }

    void MSG_AnswerClassInfo_Test()
    {
        CBufferMessage buf;
        MSG_AnswerClassInfo TMAci;
        std::vector<uint32> PeopListId;
        TMAci.SetRoomName("啦啦啦");
        TMAci.SetRoomID(22);
        TMAci.SetRoomManageId(12);
        TMAci.SetRoomPassword("123456789");
        TMAci.SetRoomRank(10);
        TMAci.SetRoomPeopleNumMax(1000);
        TMAci.SetRoomType(25);
        TMAci.SetRoomState(100);
        PeopListId.push_back(1);
        PeopListId.push_back(2);
        PeopListId.push_back(3);
        TMAci.SetRoomPeopleListId(PeopListId);
        PeopListId.clear();

        g_m2pack.PackMessage(&TMAci, buf);
        CMessage parserMsg(CMessage2Parser::GetMsgType(buf));
        CPPUNIT_ASSERT(CMessage2Parser::GetMsgType(buf) == MSG_AnswerClassInfo_Id);
        g_m2parser.Parser(&parserMsg, buf);
        MSG_AnswerClassInfo TParserAci(parserMsg);

        uint32 RoomId = 0;
        uint32 RoomRank = 0;
        uint32 RoomType = 0;
        uint32 RoomState = 0;
        uint32 RoomPeopleNumMax = 0;
        uint32 RoomManageId = 0;
        std::string strRoomName;
        std::string strPassword;

        TParserAci.GetRoomName(strRoomName);
        CPPUNIT_ASSERT(strRoomName == "啦啦啦");
        TParserAci.GetRoomID(RoomId);
        CPPUNIT_ASSERT(RoomId == 22);
        TParserAci.GetRoomRank(RoomRank);
        CPPUNIT_ASSERT(RoomRank == 10);
        TParserAci.GetRoomPeopleNumMax(RoomPeopleNumMax);
        CPPUNIT_ASSERT(RoomPeopleNumMax == 1000);
        TParserAci.GetRoomPassword(strPassword);
        CPPUNIT_ASSERT(strPassword == "123456789");
        TParserAci.GetRoomState(RoomState);
        CPPUNIT_ASSERT(RoomState == 100);
        TParserAci.GetRoomManageId(RoomManageId);
        CPPUNIT_ASSERT(RoomManageId == 12);
        TParserAci.GetRoomType(RoomType);
        CPPUNIT_ASSERT(RoomType == 25);
        TParserAci.GetRoomPeopleListId(PeopListId);
        CPPUNIT_ASSERT(PeopListId[0] == 1);
        CPPUNIT_ASSERT(PeopListId[1] == 2);
        CPPUNIT_ASSERT(PeopListId[2] == 3);
    }

    void MSG_AnswerClassList_Test()
    {
        CBufferMessage buf;
        MSG_AnswerClassList TMAcl;
        std::vector<uint32> RoomIdList;
        std::vector<uint32> RoomStateList;
        std::vector<std::string> RoomNameList;
        RoomIdList.push_back(111);
        RoomIdList.push_back(1112);
        RoomIdList.push_back(11123);
        TMAcl.SetRoomIdList(RoomIdList);
        RoomStateList.push_back(111);
        RoomStateList.push_back(1112);
        RoomStateList.push_back(1113);
        TMAcl.SetRoomStateList(RoomStateList);
        RoomNameList.push_back("房间1！");
        RoomNameList.push_back("房间2！");
        RoomNameList.push_back("房间3！");
        TMAcl.SetRoomNameList(RoomNameList);
        RoomIdList.clear();
        RoomNameList.clear();
        RoomStateList.clear();

        g_m2pack.PackMessage(&TMAcl, buf);
        CMessage parserMsg(CMessage2Parser::GetMsgType(buf));
        CPPUNIT_ASSERT(CMessage2Parser::GetMsgType(buf) == MSG_AnswerClassList_Id);
        g_m2parser.Parser(&parserMsg, buf);
        MSG_AnswerClassList TParserAcl(parserMsg);

        TParserAcl.GetRoomIdList(RoomIdList);
        CPPUNIT_ASSERT(RoomIdList[0] == 111);
        CPPUNIT_ASSERT(RoomIdList[1] == 1112);
        CPPUNIT_ASSERT(RoomIdList[2] == 11123);
        TParserAcl.GetRoomStateList(RoomStateList);
        CPPUNIT_ASSERT(RoomStateList[0] == 111);
        CPPUNIT_ASSERT(RoomStateList[1] == 1112);
        CPPUNIT_ASSERT(RoomStateList[2] == 1113);
        TParserAcl.GetRoomNameList(RoomNameList);
        CPPUNIT_ASSERT(RoomNameList[0] == "房间1！");
        CPPUNIT_ASSERT(RoomNameList[1] == "房间2！");
        CPPUNIT_ASSERT(RoomNameList[2] == "房间3！");
    }

    void MSG_AnswerLogin_Test()
    {
        CBufferMessage buf;
        MSG_AnswerLogin TMAl;
        std::string ATLGUID;
        uint8 LoginResult = 0;
        uint32 Token = 0;
        TMAl.SetATLGUID("啦啦啦啦啦啦！！！！！！！！！！！！！！");
        TMAl.SetLoginResult(10);
        TMAl.SetToken(10000);

        g_m2pack.PackMessage(&TMAl, buf);
        CMessage parserMsg(CMessage2Parser::GetMsgType(buf));
        CPPUNIT_ASSERT(CMessage2Parser::GetMsgType(buf) == MSG_AnswerLogin_Id);
        g_m2parser.Parser(&parserMsg, buf);
        MSG_AnswerLogin TParserAl(parserMsg);

        TParserAl.GetATLGUID(ATLGUID);
        CPPUNIT_ASSERT(ATLGUID == "啦啦啦啦啦啦！！！！！！！！！！！！！！");
        TParserAl.GetToken(Token);
        CPPUNIT_ASSERT(Token == 10000);
        TParserAl.GetLoginResult(LoginResult);
        CPPUNIT_ASSERT(LoginResult == 10);
    }

    void MSG_AnswerProfileSync_Test()
    {
        CBufferMessage buf;
        MSG_AnswerProfileSync TMAps;
        std::string strMessageSynchro;
        TMAps.SetMessageSynchro("!!ee e e e ");

        g_m2pack.PackMessage(&TMAps, buf);
        CMessage parserMsg(CMessage2Parser::GetMsgType(buf));
        CPPUNIT_ASSERT(CMessage2Parser::GetMsgType(buf) == MSG_AnswerProfileSync_Id);
        g_m2parser.Parser(&parserMsg, buf);
        MSG_AnswerProfileSync TParserAps(parserMsg);
        TParserAps.GetMessageSynchro(strMessageSynchro);
        CPPUNIT_ASSERT(strMessageSynchro == "!!ee e e e ");
    }

    void MSG_AnswerRegister_Test()
    {
        CBufferMessage buf;
        MSG_AnswerRegister TMAr;
        uint32 RetTag = 0;
        TMAr.SetRetTag(15000);

        g_m2pack.PackMessage(&TMAr, buf);
        CMessage parserMsg(CMessage2Parser::GetMsgType(buf));
        CPPUNIT_ASSERT(CMessage2Parser::GetMsgType(buf) == MSG_AnswerRegister_Id);
        g_m2parser.Parser(&parserMsg, buf);
        MSG_AnswerRegister TParserAr(parserMsg);

        TParserAr.GetRetTag(RetTag);
        CPPUNIT_ASSERT(RetTag == 15000);
    }

    void MSG_AnswerVerificationCode_Test()
    {
        CBufferMessage buf;
        MSG_AnswerVerificationCode TMAvc;
        std::string strCaptcha;
        uint8 CaptchaType = 0;
        uint8 LoginTag = 0;
        TMAvc.SetCaptcha("我是标题！");
        TMAvc.SetCaptchaType(105);
        TMAvc.SetLoginTag(3);

        g_m2pack.PackMessage(&TMAvc, buf);
        CMessage parserMsg(CMessage2Parser::GetMsgType(buf));
        CPPUNIT_ASSERT(CMessage2Parser::GetMsgType(buf) == MSG_AnswerVerificationCode_Id);
        g_m2parser.Parser(&parserMsg, buf);
        MSG_AnswerVerificationCode TParserAvc(parserMsg);

        TParserAvc.GetCaptcha(strCaptcha);
        CPPUNIT_ASSERT(strCaptcha == "我是标题！");
        TParserAvc.GetCaptchaType(CaptchaType);
        CPPUNIT_ASSERT(CaptchaType == 105);
        TParserAvc.GetLoginTag(LoginTag);
        CPPUNIT_ASSERT(LoginTag == 3);
    }

    void MSG_RequestClassInfo_Test()
    {
        CBufferMessage buf;
        MSG_RequestClassInfo TMRci;
        uint32 RoomId = 0;
        TMRci.SetRoomID(15016);

        g_m2pack.PackMessage(&TMRci, buf);
        CMessage parserMsg(CMessage2Parser::GetMsgType(buf));
        CPPUNIT_ASSERT(CMessage2Parser::GetMsgType(buf) == MSG_RequestClassInfo_Id);
        g_m2parser.Parser(&parserMsg, buf);
        MSG_RequestClassInfo TParserAci(parserMsg);

        TParserAci.GetRoomID(RoomId);
        CPPUNIT_ASSERT(RoomId == 15016);
    }

    void MSG_RequestClassList_Test()
    {
        CBufferMessage buf;
        uint32 UserType = 0;
        MSG_RequestClassList TMRcl;
        TMRcl.SetUserType(10000000);

        g_m2pack.PackMessage(&TMRcl, buf);
        CMessage parserMsg(CMessage2Parser::GetMsgType(buf));
        CPPUNIT_ASSERT(CMessage2Parser::GetMsgType(buf) == MSG_RequestClassList_Id);
        g_m2parser.Parser(&parserMsg, buf);
        MSG_RequestClassInfo TParserRcl(parserMsg);

        TMRcl.GetUserType(UserType);
        CPPUNIT_ASSERT(UserType == 10000000);
    }

    void MSG_RequestLogin_Test()
    {
        CBufferMessage buf;
        MSG_RequestLogin TMRl;
        std::string strAccountNumber;
        std::string strPassword;
        std::string strVerificationCode;
        TMRl.SetAccountNumber("Sum");
        TMRl.SetPassword("123456789");
        TMRl.SetVerificationCode("EFDA");

        g_m2pack.PackMessage(&TMRl, buf);
        CMessage parserMsg(CMessage2Parser::GetMsgType(buf));
        CPPUNIT_ASSERT(CMessage2Parser::GetMsgType(buf) == MSG_RequestLogin_Id);
        g_m2parser.Parser(&parserMsg, buf);
        MSG_RequestLogin TParserRl(parserMsg);

        TParserRl.GetAccountNumber(strAccountNumber);
        CPPUNIT_ASSERT(strAccountNumber == "Sum");
        TParserRl.GetPassword(strPassword);
        CPPUNIT_ASSERT(strPassword == "123456789");
        TParserRl.GetVerificationCode(strVerificationCode);
        CPPUNIT_ASSERT(strVerificationCode == "EFDA");
    }

    void MSG_RequestProfileSync_Test()
    {
        CBufferMessage buf;
        MSG_RequestProfileSync TMRps;
        uint8 Rank = 0;
        uint8 HeadForm = 0;
        std::string strAutograph;
        std::string strHeadPortrait;
        std::string strNickName;
        TMRps.SetRank(10);
        TMRps.SetHeadForm(20);
        TMRps.SetAutograph("!!!!!!!!!!!!!!!!!!!!!");
        TMRps.SetHeadPortrait("lalala");
        TMRps.SetNickname("yahoooooooooooooooooooooooooooooooooooo");

        g_m2pack.PackMessage(&TMRps, buf);
        CMessage parserMsg(CMessage2Parser::GetMsgType(buf));
        CPPUNIT_ASSERT(CMessage2Parser::GetMsgType(buf) == MSG_RequestProfileSync_Id);
        g_m2parser.Parser(&parserMsg, buf);
        MSG_RequestProfileSync TParserRps(parserMsg);

        TParserRps.GetRank(Rank);
        CPPUNIT_ASSERT(Rank == 10);
        TParserRps.GetHeadForm(HeadForm);
        CPPUNIT_ASSERT(HeadForm == 20);
        TParserRps.GetAutograph(strAutograph);
        CPPUNIT_ASSERT(strAutograph == "!!!!!!!!!!!!!!!!!!!!!");
        TParserRps.GetHeadPortrait(strHeadPortrait);
        CPPUNIT_ASSERT(strHeadPortrait == "lalala");
        TParserRps.GetNickname(strNickName);
        CPPUNIT_ASSERT(strNickName == "yahoooooooooooooooooooooooooooooooooooo");
    }

    void MSG_RequestRegister_Test()
    {
        CBufferMessage buf;
        MSG_RequestRegister TMRr;
        uint8 Rank = 0;
        uint8 HeadForm = 0;
        std::string strAutograph;
        std::string strHeadPortrait;
        std::string strNickName;
        std::string strEmailAddress;
        TMRr.SetRank(10);
        TMRr.SetHeadForm(20);
        TMRr.SetAutograph("!!!!!!!!!!!!!!!!!!!!!");
        TMRr.SetHeadPortrait("lalala");
        TMRr.SetNickname("继续yahoooooooooooooooooooooooooooooooooooo");
        TMRr.SetEmailAddress("123456@123.com");

        g_m2pack.PackMessage(&TMRr, buf);
        CMessage parserMsg(CMessage2Parser::GetMsgType(buf));
        CPPUNIT_ASSERT(CMessage2Parser::GetMsgType(buf) == MSG_RequestRegister_Id);
        g_m2parser.Parser(&parserMsg, buf);
        MSG_RequestRegister TParserRr(parserMsg);

        TParserRr.GetRank(Rank);
        CPPUNIT_ASSERT(Rank == 10);
        TParserRr.GetHeadForm(HeadForm);
        CPPUNIT_ASSERT(HeadForm == 20);
        TParserRr.GetAutograph(strAutograph);
        CPPUNIT_ASSERT(strAutograph == "!!!!!!!!!!!!!!!!!!!!!");
        TParserRr.GetHeadPortrait(strHeadPortrait);
        CPPUNIT_ASSERT(strHeadPortrait == "lalala");
        TParserRr.GetNickname(strNickName);
        CPPUNIT_ASSERT(strNickName == "继续yahoooooooooooooooooooooooooooooooooooo");
        TMRr.GetEmailAddress(strEmailAddress);
        CPPUNIT_ASSERT(strEmailAddress == "123456@123.com");
    }

    void MSG_RequestVerificationCode_Test()
    {
        CBufferMessage buf;
        std::string strMachineAddress;
        MSG_RequestVerificationCode TMRvc;
        TMRvc.SetMachineAddress("q q q q q F F");

        g_m2pack.PackMessage(&TMRvc, buf);
        CMessage parserMsg(CMessage2Parser::GetMsgType(buf));
        CPPUNIT_ASSERT(CMessage2Parser::GetMsgType(buf) == MSG_RequestVerificationCode_Id);
        g_m2parser.Parser(&parserMsg, buf);
        MSG_RequestVerificationCode TParserRvc(parserMsg);

        TParserRvc.GetMachineAddress(strMachineAddress);
        CPPUNIT_ASSERT(strMachineAddress == "q q q q q F F");
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION ( MSG_Message_Test );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MSG_Message_Test, "testNMessage");

#endif