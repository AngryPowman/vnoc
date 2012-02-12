#pragma once

#define SCINTILLA_CLASS_NAME L"scintilla"
#define SCINTILLA_ADVANCED_DLL_NAME L"scilexer.dll"
#define SCINTILLA_DLL_NAME L"scintilla.dll"

class CSciEdit : public CWnd
{
public:
	CSciEdit(HINSTANCE hInst = 0);
	virtual ~CSciEdit();
public:
	BOOL Create(LPCTSTR lpszWindowName, const RECT& rect, CWnd* pParentWnd, UINT nID);
private:
	VOID _AddRef();
	VOID _Release();
private:
	HINSTANCE m_hinst;		// ָ����Դ�ļ�����λ��
	static HMODULE m_dllHandle;
	static LONG m_refCount;
};