#include "../FrameWork/FrameBase.hpp"

class CTestRunner : public CFrameBase
{
public:
	CTestRunner():CFrameBase(module_CppTestMain)
	{};
	STDMETHOD( Run() );

	CarelessXMessage(CTestRunner);
};