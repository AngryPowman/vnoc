#include "../FrameWork/FrameBase.hpp"

class CTestRunner : public CFrameBase
{
public:
	CTestRunner():CFrameBase(module_CppTestMain)
	{};

	VOID StartTest(XMessage_StartTest* msg);

	Begin_XMessage(CTestRunner)
		OnXMessage(XMessage_StartTest,StartTest)
	End_XMessage()
};