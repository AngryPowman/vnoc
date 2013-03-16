#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../RoomPool.cpp"

class RoomPoolTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( RoomPoolTest );
    CPPUNIT_TEST( testInitPool );
    CPPUNIT_TEST( testAcquire );
    CPPUNIT_TEST( testRelease );
    CPPUNIT_TEST_SUITE_END();

public:
    void testInitPool()
    {
        RoomPool rPool;
        CPPUNIT_ASSERT(rPool.initPool(0, 0) == false);
        CPPUNIT_ASSERT(rPool.initPool(0) == false);
        CPPUNIT_ASSERT(rPool.initPool(1, 0) == false);
        CPPUNIT_ASSERT(rPool.initPool() == true);
        CPPUNIT_ASSERT(rPool.initPool(1, 1) == true);
    }

    void testAcquire()
    {
        RoomPool rPool;
        rPool.initPool();
        for(int i = 0; i < 16; ++i)
        {
            CPPUNIT_ASSERT (rPool.acquire() != NULL);
        }
        rPool.destroy();
        rPool.initPool(10, 10);
        for(int i = 0; i < 16; ++i)
        {
            CPPUNIT_ASSERT(rPool.acquire() != NULL);
        }
    }

    void testRelease()
    {
        RoomPool rPool;
        rPool.initPool();
        for(int i = 0; i < 256; ++i)
            rPool.acquire();
        for(int i = 64; i < 128; ++i)
            rPool.release(i);
        for(int i = 0; i < 64; ++i)
            CPPUNIT_ASSERT(rPool.acquire() != NULL);
    }

};
CPPUNIT_TEST_SUITE_REGISTRATION ( RoomPoolTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RoomPoolTest, "RoomPoolTest");