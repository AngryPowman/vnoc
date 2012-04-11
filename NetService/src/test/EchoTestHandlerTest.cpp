#ifndef ECHOTESTHANDLERTEST_H
#define ECHOTESTHANDLERTEST_H
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "EchoTestHandler.hpp"
class EchoTestHandlerTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( EchoTestHandlerTest );
	CPPUNIT_TEST( SimpleTest );
	CPPUNIT_TEST_SUITE_END();
public:
	void SimpleTest()
	{
		Connection *conn = new Connection;
		EchoTestHandler<Connection> handler(conn);
		CPPUNIT_ASSERT(true);
		delete conn;
	}
};
CPPUNIT_TEST_SUITE_REGISTRATION ( EchoTestHandlerTest );
#endif /*ECHOTESTHANDLERTEST_H*/