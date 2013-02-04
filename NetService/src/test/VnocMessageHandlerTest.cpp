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
#include "../../Message/MSG_UNION.h"
#include "../../Message/PackMessage.h"

#include <ctime>

class VnocMessageHandlerTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( VnocMessageHandlerTest );
    CPPUNIT_TEST( testRVC );
    CPPUNIT_TEST( testRLIwithNothing );
    CPPUNIT_TEST( testRLIwithAccountNumber );
    CPPUNIT_TEST( testRLIwithAccountNumberAndPassword );
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
        MSG_RVC rvcMessage;
        PackMessage packer;
        int len = packer.GetMessageLen(&rvcMessage);
        char *buf = new char[len];
        packer.Pack(&rvcMessage, (byte *)buf, len);
        conn_->setRecv(buf, len);
        const char *sendBuf = conn_->getSendBuf();
        //return an AVC message
        CMessageParser parser;
        CMessage *msg = parser.Parse((byte*)sendBuf, conn_->getSendLen());
        CPPUNIT_ASSERT(msg->GetMessageType() == MSG_AVC_TYPE);
        delete msg;
        delete buf;
    }

    void testRLIwithNothing()
    {
        RliMessageHandler rlihandler(protocol_);
        VnocMessageSocketHandler<MockTcpConnection> handler(conn_);
        handler.setProtocol(protocol_);
        handler.start();
        MSG_RLI rliMessage;
        PackMessage packer;
        int len = packer.GetMessageLen(&rliMessage);
        char *buf = new char[len];
        packer.Pack(&rliMessage, (byte *)buf, len);
        conn_->setRecv(buf, len);
        char *sendBuf = (char*)conn_->getSendBuf();
        //return an ALI message with login-failure.
        CMessageParser parser;
        CMessage *msg = parser.Parse((byte*)sendBuf, conn_->getSendLen());
        CPPUNIT_ASSERT(msg->GetMessageType() == MSG_ALI_TYPE);
        CPPUNIT_ASSERT(((MSG_ALI*)msg)->GetLoginResult() == 1);
        delete msg;
        delete buf;
    }

    void testRLIwithAccountNumber()
    {
        RliMessageHandler rlihandler(protocol_);
        VnocMessageSocketHandler<MockTcpConnection> handler(conn_);
        handler.setProtocol(protocol_);
        handler.start();
        MSG_RLI rliMessage;
        unsigned char tmp[16] = {0};
        srand((unsigned int)time(0));
        for(int i = 0; i < 16; ++i)
        {
            tmp[i] = rand() % 256;
        }
        rliMessage.SetAccountNumber(tmp, 16);
        PackMessage packer;
        int len = packer.GetMessageLen(&rliMessage);
        char *buf = new char[len];
        packer.Pack(&rliMessage, (byte *)buf, len);
        conn_->setRecv(buf, len);
        char *sendBuf = (char*)conn_->getSendBuf();
        //return an ALI message with login-success
        CMessageParser parser;
        CMessage *msg = parser.Parse((byte*)sendBuf, conn_->getSendLen());
        CPPUNIT_ASSERT(msg->GetMessageType() == MSG_ALI_TYPE);
        CPPUNIT_ASSERT(((MSG_ALI*)msg)->GetLoginResult()==0);
        delete msg;
        delete buf;
    }

    void testRLIwithAccountNumberAndPassword()
    {
        RliMessageHandler rlihandler(protocol_);
        VnocMessageSocketHandler<MockTcpConnection> handler(conn_);
        handler.setProtocol(protocol_);
        handler.start();
        MSG_RLI rliMessage;
        unsigned char tmp[16] = {0};
        srand((unsigned int)time(0));
        for(int i = 0; i < 16; ++i)
        {
            tmp[i] = rand() % 256;
        }
        rliMessage.SetAccountNumber(tmp, 16);
        for(int i = 0; i < 16; ++i)
        {
            tmp[i] = rand() % 256;
        }
        rliMessage.SetPassword(tmp, 16);
        PackMessage packer;
        int len = packer.GetMessageLen(&rliMessage);
        char *buf = new char[len];
        packer.Pack(&rliMessage, (byte *)buf, len);
        conn_->setRecv(buf, len);
        char *sendBuf = (char*)conn_->getSendBuf();
        //return an ALI message with login-success
        CMessageParser parser;
        CMessage *msg = parser.Parse((byte*)sendBuf, conn_->getSendLen());
        CPPUNIT_ASSERT(msg->GetMessageType() == MSG_ALI_TYPE);
        CPPUNIT_ASSERT(((MSG_ALI*)msg)->GetLoginResult() == 0);
        delete msg;
        delete buf;
    }

    void testRCL()
    {
        RclMessageHandler rclhandler(protocol_);
        VnocMessageSocketHandler<MockTcpConnection> handler(conn_);
        handler.setProtocol(protocol_);
        handler.start();
        MSG_RCL rclMessage;
        PackMessage packer;
        int len = packer.GetMessageLen(&rclMessage);
        char *buf = new char[len];
        packer.Pack(&rclMessage, (byte *)buf, len);
        conn_->setRecv(buf, len);
        const char *sendBuf = conn_->getSendBuf();
        //return an ACL message
        CMessageParser parser;
        CMessage *msg = parser.Parse((byte*)sendBuf, conn_->getSendLen());
        CPPUNIT_ASSERT(msg->GetMessageType() == MSG_ACL_TYPE);
        delete msg;
        delete buf;
    }

};
CPPUNIT_TEST_SUITE_REGISTRATION ( VnocMessageHandlerTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(VnocMessageHandlerTest, "VnocMessageHandlerTest");
