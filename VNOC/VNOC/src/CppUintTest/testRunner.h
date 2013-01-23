#include "../FrameWork/FrameBase.hpp"

class CTestRunner : public CFrameBase
{
public:
	CTestRunner():CFrameBase(module_CppTest_Main)
	{};
	STDMETHOD( Run() );

	CarelessXMessage(CTestRunner);
};