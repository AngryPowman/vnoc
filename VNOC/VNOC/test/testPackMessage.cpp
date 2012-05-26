#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../PackMessage.h"
#include <string.h>


class testPackMessage :public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( testPackMessage );
	CPPUNIT_TEST( MSGPackTest );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp()
	{

	}
	void tearDown()
	{

	}

public:

	void MSGPackTest()
	{
		byte testParamO [] = {0x02,0x02,0x02,0x02,
			0x02,0x02,0x02,0x02,
			0x02,0x02,0x02,0x02,
			0x02,0x02,0x02,0x02};
		byte testParamT [] = {0x03,0x03,0x03,0x03,
			0x03,0x03,0x03,0x03,
			0x03,0x03,0x03,0x03,
			0x03,0x03,0x03,0x03};
		byte testParamS [] = {0x04,0x04,0x04,0x04,
			0x04,0x04,0x04,0x04,
			0x04,0x04,0x04,0x04,
			0x04,0x04,0x04,0x04};
		byte testComLenRVC[] = {
			0x00,0x00,0x00,0x10,
		};

		byte testComLenAVC[] = {
			0x00,0x00,0x00,0x01,
			0x00,0x00,0x00,0x01,
			0x00,0x00,0x00,0x10
		};
		byte testComLenRLI[] = {
			0x00,0x00,0x00,0x10,
			0x00,0x00,0x00,0x10,
			0x00,0x00,0x00,0x10
		};
		byte testComLenALI[] = {
			0x00,0x00,0x00,0x10,
			0x00,0x00,0x00,0x10,
			0x00,0x00,0x00,0x10
		};

		byte testComLenRPS[] = {
			0x00,0x00,0x00,0x01,
			0x00,0x00,0x00,0x10,
			0x00,0x00,0x00,0x10,
			0x00,0x00,0x00,0x01,
			0x00,0x00,0x00,0x10
		};

		byte testComLenAPS[] = {
			0x00,0x00,0x00,0x10,
		};

		byte testPackAVC[62] = {0};
		byte testPackRVC[52] = {0};
		byte testPackRLI[100] = {0};
		byte testPackALI[100] = {0};
		byte testPackRPS[100] = {0};
		byte testPackAPS[100] = {0};
		//已经填充过的对象 使用前请保证调用过_Close否则会导致内存泄露
		//需要自己填充参数长度msg_xxx.SetCmlListLen(byte* in_byte_ptr);
		//AVC
		MSG_AVC msg_avc;
		PackMessage msg_pack;
		msg_avc.SetSerial(0x01);
		CPPUNIT_ASSERT(msg_avc.GetSerial() == 0x01);
		msg_avc.SetGUID(testParamO);
		CPPUNIT_ASSERT(memcmp(msg_avc.GetGUID(),testParamO,sizeof(byte) * 16) == 0);
		CPPUNIT_ASSERT(msg_avc.SetCmlListLen(testComLenAVC,3));
		msg_avc.SetCaptcha(testParamO,16);
		CPPUNIT_ASSERT(memcmp(msg_avc.GetCaptcha(),testParamO,16) == 0);
		msg_avc.SetLoginTag(0x1e);
		CPPUNIT_ASSERT(msg_avc.GetLoginTag() == 0x1e);
		msg_avc.SetCaptchaType(0x1e);
		CPPUNIT_ASSERT(msg_avc.GetCaptchaType() == 0x1e);
		CPPUNIT_ASSERT(msg_pack.Pack(&msg_avc,testPackAVC,62) == 0);
		//RVC
		MSG_RVC msg_rvc;
		CPPUNIT_ASSERT(msg_rvc.SetCmlListLen(testComLenRVC,1));
		msg_rvc.SetMachineAddress(testParamO,16);
		CPPUNIT_ASSERT(memcmp(msg_rvc.GetMachineAddress(),testParamO,16) == 0);

		CPPUNIT_ASSERT(msg_pack.Pack(&msg_rvc,testPackRVC,52) == 0);


		//RLI
		MSG_RLI msg_rli;

		msg_rli.SetSerial(0x01);
		CPPUNIT_ASSERT(msg_rli.GetSerial() == 0x01);

		msg_rli.SetGUID(testParamO);
		CPPUNIT_ASSERT(memcmp(msg_rli.GetGUID(),testParamO,sizeof(byte) * 16) == 0);

		CPPUNIT_ASSERT(msg_rli.SetCmlListLen(testComLenRLI,3));

		msg_rli.SetVerificationCode(testParamO,16);
		CPPUNIT_ASSERT(memcmp(msg_rli.GetVerificationCode(),testParamO,sizeof(byte) * 16) == 0);

		msg_rli.SetAccountNumber(testParamT,16);
		CPPUNIT_ASSERT(memcmp(msg_rli.GetAccountNumber(),testParamT,16) == 0);

		msg_rli.SetPassword(testParamS,16);
		CPPUNIT_ASSERT(memcmp(msg_rli.GetPassword(),testParamS,16) == 0);
		CPPUNIT_ASSERT(msg_pack.Pack(&msg_rli,testPackRLI,100) == 0);
		//ALI
		MSG_ALI msg_ali;
		msg_ali.SetSerial(0x01);
		CPPUNIT_ASSERT(msg_ali.GetSerial() == 0x01);

		msg_ali.SetGUID(testParamO);
		CPPUNIT_ASSERT(memcmp(msg_ali.GetGUID(),testParamO,sizeof(byte) * 16) == 0);

		CPPUNIT_ASSERT(msg_ali.SetCmlListLen(testComLenALI,3));

		msg_ali.SetLoginResult(0x1E);
		CPPUNIT_ASSERT(msg_ali.GetLoginResult() == 0x1E);

		msg_ali.SetToken(testParamO,16);
		//CPPUNIT_ASSERT(memcmp(msg_ali.GetToken(),testParamO,sizeof(byte) * 16) == 0);

		msg_ali.SetATLGUID(testParamT,16);
		CPPUNIT_ASSERT(memcmp(msg_ali.GetATLGUID(),testParamT,sizeof(byte) * 16) == 0);
		CPPUNIT_ASSERT(msg_pack.Pack(&msg_ali,testPackALI,100) == 0);


		//RPS
		MSG_RPS msg_rps;
		msg_rps.SetSerial(0x01);
		CPPUNIT_ASSERT(msg_rps.GetSerial() == 0x01);

		msg_rps.SetGUID(testParamO);
		CPPUNIT_ASSERT(memcmp(msg_rps.GetGUID(),testParamO,sizeof(byte) * 16) == 0);

		CPPUNIT_ASSERT(msg_rps.SetCmlListLen(testComLenRPS,5));

		msg_rps.SetRank(0x01);
		CPPUNIT_ASSERT(msg_rps.GetRank() == 0x01);

		msg_rps.SetNickname(testParamO,16);
		CPPUNIT_ASSERT(memcmp(msg_rps.GetNickname(),testParamO,sizeof(byte) * 16) == 0);

		msg_rps.SetAutograph(testParamT,16);
		CPPUNIT_ASSERT(memcmp(msg_rps.GetAutograph(),testParamT,sizeof(byte) * 16) == 0);

		msg_rps.SetHeadForm('A');
		CPPUNIT_ASSERT(msg_rps.GetHeadForm() == 'A');

		msg_rps.SetHeadPortrait(testParamS,16);
		CPPUNIT_ASSERT(memcmp(msg_rps.GetHeadPortrait(),testParamS,sizeof(byte) * 16) == 0);

		CPPUNIT_ASSERT(msg_pack.Pack(&msg_rps,testPackRPS,100) == 0);


		//APS
		MSG_APS msg_aps;
		msg_aps.SetSerial(0x01);
		CPPUNIT_ASSERT(msg_aps.GetSerial() == 0x01);

		msg_aps.SetGUID(testParamO);
		CPPUNIT_ASSERT(memcmp(msg_aps.GetGUID(),testParamO,sizeof(byte) * 16) == 0);

		CPPUNIT_ASSERT(msg_aps.SetCmlListLen(testComLenAPS,1));


		msg_aps.SetMessageSynchro(testParamO,16);
		CPPUNIT_ASSERT(memcmp(msg_aps.GetMessageSynchro(),testParamO,sizeof(byte) * 16) == 0);

		CPPUNIT_ASSERT(msg_pack.Pack(&msg_aps,testPackAPS,100) == 0);

		CPPUNIT_ASSERT(true);
	}

};

CPPUNIT_TEST_SUITE_REGISTRATION ( testPackMessage );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(testPackMessage, "testPackMessage");