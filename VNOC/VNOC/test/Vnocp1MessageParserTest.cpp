#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../MessageParser.h"
#include <string.h>
class Vnocp1MessageParserTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( Vnocp1MessageParserTest );
    CPPUNIT_TEST( MSGAVCParseTest );
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp()
    {
        
    }
    void tearDown()
    {

    }
public:
    void MSGAVCParseTest()
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
		byte testParamRvc [] = {0x02,0x02,0x02,0x02,
			0x02,0x02,0x02,0x02,
			0x02,0x02,0x02,0x02,
			0x02,0x02,0x02,0x02};
		byte testComLen[] = {0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x10};

        byte testAVC [] = {0x55,
		0x00,0x00,0x00,0x1E,
		0x01,
		0x01,
		0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,
		0x15,
		0x00,0x00,0x00,0x00,
		0x03,
		0x00,0x00,0x00,0x01,
		0x00,0x00,0x00,0x01,
		0x00,0x00,0x00,0x10,
		0x01,
		0x01,
 		0x33,0x33,0x33,0x33,
 		0x33,0x33,0x33,0x33,
		0x33,0x33,0x33,0x33,
		0x33,0x33,0x33,0x33,
		0x00,0x05,
		0x43};
		//AVC
		byte testPackAVC[62] = {0};
		byte testPackRVC[52] = {0};
		byte testPackRLI[100] = {0};
		byte testPackALI[100] = {0};
		byte testPackRPS[100] = {0};
		byte testPackAPS[100] = {0};
		CMessageParser msgParser;
		CMessage* msg = msgParser.Parse(testAVC);
		CPPUNIT_ASSERT(msg->GetMessageType() == MSG_AVC_TYPE);//验证消息类型是AVC
		MSG_AVC* msg_avc = (MSG_AVC *)msg;
		msg_avc->SetGUID(testParamO);
//		msg_avc->SetCmlListLen(3);
		msg_avc->SetCaptcha(testParamO,16);
		msg_avc->SetLoginTag(0x1e);
		msg_avc->SetCaptchaType(0x1e);
		PackMessage(msg_avc,testPackAVC,62);

		msg = msgParser.Parse(testPackAVC);
		CPPUNIT_ASSERT(msg->GetMessageType() == MSG_AVC_TYPE);//验证消息类型是AVC
		msg_avc = (MSG_AVC *)msg;


		//已经填充过的对象 使用前请保证调用过_Close否则会导致内存泄露
		//需要自己填充参数长度msg_xxx.SetCmlListLen(byte* in_byte_ptr);
		//AVC
		MSG_AVC msg_avc_;
		msg_avc_.SetCaptcha(testParamO,16);
		msg_avc_.SetLoginTag(0x1e);
		msg_avc_.SetCaptchaType(0x1e);
		PackMessage(&msg_avc_,testPackAVC,62);
		//RVC
		MSG_RVC msg_rvc;
		msg_rvc.SetMachineAddress(testParamRvc,16);
		PackMessage(&msg_rvc,testPackRVC,52);


		//RLI
		MSG_RLI msg_rli;
		msg_rli.SetVerificationCode(testParamO,16);
		msg_rli.SetAccountNumber(testParamT,16);
		msg_rli.SetPassword(testParamS,16);
		PackMessage(&msg_rli,testPackRLI,100);
		//ALI
		MSG_ALI msg_ali;
		msg_ali.SetLoginResult(0x1E);
		msg_ali.SetToken(testParamO,16);
		msg_ali.SetATLGUID(testParamT,16);
		PackMessage(&msg_ali,testPackALI,100);


		//RPS
		MSG_RPS msg_rps;
		msg_rps.SetRank(0x01);
		msg_rps.SetNickname(testParamO,16);
		msg_rps.SetAutograph(testParamT,16);
		msg_rps.SetHeadForm('A');
		msg_rps.SetHeadPortrait(testParamS,16);
		PackMessage(&msg_rps,testPackRPS,100);


		//APS
		MSG_APS msg_aps;
		msg_aps.SetMessageSynchro(testParamO,16);
		PackMessage(&msg_aps,testPackAPS,100);

// 		//验证令牌
// 		byte expectToken[]={0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66};
// 		CPPUNIT_ASSERT(memcmp(msg_avc->GetGUID(),expectToken, sizeof(expectToken)) == 0);
// 		//验证验证码类型
// 		CPPUNIT_ASSERT(msg_avc->GetCaptchaType() == 0x01);
// 		//验证验证码内容
// 		byte CaptchaContent[]={0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33};
// 		CPPUNIT_ASSERT(memcmp(msg_avc->GetCaptcha(),CaptchaContent, sizeof(CaptchaContent)) == 0);

		CPPUNIT_ASSERT(true);
    }

};
CPPUNIT_TEST_SUITE_REGISTRATION ( Vnocp1MessageParserTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Vnocp1MessageParserTest, "Vnocp1MessageParserTest");