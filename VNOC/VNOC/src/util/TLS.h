#pragma once
#include <atlstr.h>

class CTLS
{
public:
	CTLS(LPCTSTR helpString);
	~CTLS();
public:
	void Set(LPVOID value);
	LPVOID Get();
private:
	DWORD m_tlsIndex;
	CString m_helpString;
};
