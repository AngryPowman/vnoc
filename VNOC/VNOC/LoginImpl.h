#pragma once
#include "ILogin.h"
#include "VNOCLoginDlg.h"

class CLoginImpl
	: public ILogin
{
public:
	CLoginImpl(void);
	~CLoginImpl(void);
public:
	STDMETHOD( Initialize(IUnknown* UpperFrame=NULL) );
	STDMETHOD( UnInitialize() );
	STDMETHOD( Run() );
	STDMETHOD( Terminate() );
	STDMETHOD( Show(BOOL bShow=TRUE) );
	STDMETHOD( SetAccount(LPCTSTR userName) );
private:
	CVNOCLoginDlg* m_dlg;
};

