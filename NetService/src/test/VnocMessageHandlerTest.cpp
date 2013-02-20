#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../VnocMessageSocketHandler.hpp"
#include "../VnocProtocol.hpp"
#include "MockTcpConnection.hpp"
#include "../RvcMessageHandler.hpp"
#include "../RliMessageHandler.hpp"
#include "../RclMessageHandler.hpp"
#include "../FileUserStorage.h"
#include "../UserManage.hpp"
#include <shlwapi.h>
#include <atlstr.h>
#include "../../NMessage/MessageUnion.h"
#include "../../NMessage/Message2Pack.h"
#include "../../NMessage/Message2Parser.h"
#include "../../NMessage/ParserMessageXML.h"

#include <ctime>

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

class VnocMessageHandlerTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( VnocMessageHandlerTest );
    CPPUNIT_TEST( testRVC );
    CPPUNIT_TEST( testRLIdefault );
    CPPUNIT_TEST( testRLIwithEmptyAccountNumber );
    CPPUNIT_TEST( testRLIwithAccountNumber );
    CPPUNIT_TEST( testRLIwithAccountNumberAndPassword );
    CPPUNIT_TEST( testRLIofLoginTwoTimes );
    CPPUNIT_TEST( testRCL );
    CPPUNIT_TEST_SUITE_END();
    MockTcpConnection *conn_;
    VnocProtocol *protocol_;
public:
    fUserStorage us;

    void setUp()
    {
        conn_ = new MockTcpConnection;
        protocol_ = new VnocProtocol();
        CUserManage::GetInstance()->initial(&us);
        std::string strPath;
        CPPUNIT_ASSERT(_TDisposePath(L"../NewMsgdef.xml", strPath) == true);
        CPPUNIT_ASSERT(ParserMessageXML::Instance().LoadFile(strPath.c_str()) == MsgStatus_Ok);
    }
    void tearDown()
    {
        delete protocol_;
    }
public:
    void testRVC()
    {
        RvcMessageHandler rvchandler(protocol_);
        VnocMessageSocketHandler<MockTcpConnection> handler(conn_);
        handler.setProtocol(protocol_);
        handler.start();
        MSG_RequestVerificationCode rvcMessage;
        CBufferMessage buff;
        CMessage2Pack packer;
        rvcMessage.SetMachineAddress("EASB-ASDASD-ASD");
        packer.PackMessage(&rvcMessage, buff);
        conn_->setRecv((char*)buff.GetBuffer(), buff.GetSize());
        const char *sendBuf = conn_->getSendBuf();
        //return an AVC message
        CMessage2Parser parser;
        buff.Copy(sendBuf, conn_->getSendLen());
        CMessage msg(CMessage2Parser::GetMsgType(buff));
        CPPUNIT_ASSERT(msg.MsgId() == MSG_AnswerVerificationCode_Id);
    }

    void testRLIdefault()
    {
        RliMessageHandler rlihandler(protocol_);
        VnocMessageSocketHandler<MockTcpConnection> handler(conn_);
        handler.setProtocol(protocol_);
        handler.start();
        MSG_RequestLogin rliMessage;
        CBufferMessage buff;
        CMessage2Pack packer;
        packer.PackMessage(&rliMessage, buff);
        conn_->setRecv((char*)buff.GetBuffer(), buff.GetSize());
        char *sendBuf = (char*)conn_->getSendBuf();
        //return an ALI message with login-failure.
        CMessage2Parser parser;
        CBufferMessage Rbuff;
        Rbuff.Copy(sendBuf, conn_->getSendLen());
        MSG_AnswerLogin msg;
        parser.Parser(&msg, Rbuff);
        CPPUNIT_ASSERT(msg.MsgId() == MSG_AnswerLogin_Id);
        uint8 LoginResult = 0;
        msg.GetLoginResult(LoginResult);
        CPPUNIT_ASSERT(LoginResult == 1);
    }

    void testRLIwithEmptyAccountNumber()
    {
        RliMessageHandler rlihandler(protocol_);
        VnocMessageSocketHandler<MockTcpConnection> handler(conn_);
        handler.setProtocol(protocol_);
        handler.start();
        MSG_RequestLogin rliMessage;
        CBufferMessage buff;
        CMessage2Pack packer;
        rliMessage.SetAccountNumber("");
        packer.PackMessage(&rliMessage, buff);
        conn_->setRecv((char*)buff.GetBuffer(), buff.GetSize());
        char *sendBuf = (char*)conn_->getSendBuf();
        //return an ALI message with login-failure.
        CMessage2Parser parser;
        CBufferMessage Rbuff;
        Rbuff.Copy(sendBuf, conn_->getSendLen());
        MSG_AnswerLogin msg;
        parser.Parser(&msg, Rbuff);
        CPPUNIT_ASSERT(msg.MsgId() == MSG_AnswerLogin_Id);
        uint8 LoginResult = 0;
        msg.GetLoginResult(LoginResult);
        CPPUNIT_ASSERT(LoginResult == 1);
    }

    void testRLIwithAccountNumber()
    {
        RliMessageHandler rlihandler(protocol_);
        VnocMessageSocketHandler<MockTcpConnection> handler(conn_);
        handler.setProtocol(protocol_);
        handler.start();
        MSG_RequestLogin rliMessage;
        rliMessage.SetPassword("#asd");
        CBufferMessage buff;
        CMessage2Pack packer;
        packer.PackMessage(&rliMessage, buff);
        conn_->setRecv((char*)buff.GetBuffer(), buff.GetSize());
        char *sendBuf = (char*)conn_->getSendBuf();
        //return an ALI message with login-success
        CMessage2Parser parser;
        CBufferMessage Rbuff;
        Rbuff.Copy(sendBuf, conn_->getSendLen());
        MSG_AnswerLogin msg;
        parser.Parser(&msg, Rbuff);
        CPPUNIT_ASSERT(msg.MsgId() == MSG_AnswerLogin_Id);
        uint8 LoginResult = 0;
        msg.GetLoginResult(LoginResult);
        CPPUNIT_ASSERT(LoginResult == 1);
    }

    void testRLIwithAccountNumberAndPassword()
    {
        RliMessageHandler rlihandler(protocol_);
        VnocMessageSocketHandler<MockTcpConnection> handler(conn_);
        handler.setProtocol(protocol_);
        handler.start();
        MSG_RequestLogin rliMessage;
        rliMessage.SetPassword("1111");
        rliMessage.SetAccountNumber("!asd");
        CBufferMessage buff;
        CMessage2Pack packer;
        packer.PackMessage(&rliMessage, buff);
        conn_->setRecv((char*)buff.GetBuffer(), buff.GetSize());
        char *sendBuf = (char*)conn_->getSendBuf();
        //return an ALI message with login-success
        CMessage2Parser parser;
        CBufferMessage Rbuff;
        Rbuff.Copy(sendBuf, conn_->getSendLen());
        MSG_AnswerLogin msg;
        parser.Parser(&msg, Rbuff);
        CPPUNIT_ASSERT(msg.MsgId() == MSG_AnswerLogin_Id);
        uint8 LoginResult = 0;
        msg.GetLoginResult(LoginResult);
        CPPUNIT_ASSERT(LoginResult == 0);
    }

    void testRLIofLoginTwoTimes()
    {
        RliMessageHandler rlihandler(protocol_);
        VnocMessageSocketHandler<MockTcpConnection> handler(conn_);
        handler.setProtocol(protocol_);
        handler.start();
        MSG_RequestLogin rliMessage;
        rliMessage.SetAccountNumber("asd");
        CBufferMessage buff;
        CMessage2Pack packer;
        packer.PackMessage(&rliMessage, buff);
        conn_->setRecv((char*)buff.GetBuffer(), buff.GetSize());
        char *sendBuf = (char*)conn_->getSendBuf();
        //return an ALI message with login-success
        CMessage2Parser parser;
        CBufferMessage Rbuff;
        Rbuff.Copy(sendBuf, conn_->getSendLen());
        MSG_AnswerLogin msg;
        parser.Parser(&msg, Rbuff);
        CPPUNIT_ASSERT(msg.MsgId() == MSG_AnswerLogin_Id);
        uint8 LoginResult = 0;
        msg.GetLoginResult(LoginResult);
        CPPUNIT_ASSERT(LoginResult == 0);

        conn_->setRecv((char*)buff.GetBuffer(), buff.GetSize());
        sendBuf = (char*)conn_->getSendBuf();
        //return an ALI message with login-failure
        Rbuff.Copy(sendBuf, conn_->getSendLen());
        parser.Parser(&msg, Rbuff);
        CPPUNIT_ASSERT(msg.MsgId() == MSG_AnswerLogin_Id);
        msg.GetLoginResult(LoginResult);
        CPPUNIT_ASSERT(LoginResult == 1);
    }

    void testRCL()
    {
        RclMessageHandler rclhandler(protocol_);
        VnocMessageSocketHandler<MockTcpConnection> handler(conn_);
        handler.setProtocol(protocol_);
        handler.start();
        MSG_RequestClassList rclMessage;
        CBufferMessage buff;
        CMessage2Pack packer;
        rclMessage.SetUserType(0);
        packer.PackMessage(&rclMessage, buff);
        conn_->setRecv((char*)buff.GetBuffer(), buff.GetSize());
        char *sendBuf = (char*)conn_->getSendBuf();
        //return an ACL message
        CMessage2Parser parser;
        CBufferMessage Rbuff;
        Rbuff.Copy(sendBuf, conn_->getSendLen());
        MSG_AnswerClassList msg;
        parser.Parser(&msg, Rbuff);
        CPPUNIT_ASSERT(msg.MsgId() == MSG_AnswerClassList_Id);
    }

};
CPPUNIT_TEST_SUITE_REGISTRATION ( VnocMessageHandlerTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(VnocMessageHandlerTest, "VnocMessageHandlerTest");
