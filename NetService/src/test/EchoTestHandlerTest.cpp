#ifndef ECHOTESTHANDLERTEST_H
#define ECHOTESTHANDLERTEST_H
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "EchoTestHandler.hpp"
#include "MockTcpConnection.hpp"

class EchoTestHandlerTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( EchoTestHandlerTest );
    CPPUNIT_TEST( simpleTest );
    CPPUNIT_TEST( testSpecialChar );
    CPPUNIT_TEST_SUITE_END();
    MockTcpConnection *conn_;
public:
    void setUp()
    {
        conn_ = new MockTcpConnection;
    }
    void tearDown()
    {
        delete conn_;
    }
public:
    void simpleTest()
    {
        EchoTestHandler<MockTcpConnection> handler(conn_);
        CPPUNIT_ASSERT(true);
    }
    void testSpecialChar()
    {
        EchoTestHandler<MockTcpConnection> handler(conn_);
        CPPUNIT_ASSERT(true);
    }
};
CPPUNIT_TEST_SUITE_REGISTRATION ( EchoTestHandlerTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(EchoTestHandlerTest, "EchoTestHandlerTest");

#endif /*ECHOTESTHANDLERTEST_H*/