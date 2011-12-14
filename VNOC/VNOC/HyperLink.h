// HyperLink.h: interface for the CHyperLink class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HYPERLINK_H__154C0837_A095_4A53_B1FC_AE766AA60485__INCLUDED_)
#define AFX_HYPERLINK_H__154C0837_A095_4A53_B1FC_AE766AA60485__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_TEXT_LEN 32
#define MAX_URL_LEN 1024

class CHyperLink  
{
public:
	CHyperLink();
	virtual ~CHyperLink();
	BOOL Attach(HWND hWnd);
	HWND Detach();
	HWND GetHwnd();
	BOOL SetLink(LPCTSTR lpszText, LPCTSTR lpszUrl, BOOL bRedraw);
protected:
	void OnMouseMove(WPARAM wParam, LPARAM lParam);
	BOOL m_bMouseOn;
	HWND m_hWnd;
	HFONT m_hFont;
	TCHAR m_szText[MAX_TEXT_LEN];
	TCHAR m_szUrl[MAX_URL_LEN];
private:
	static HRESULT CALLBACK NewProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	WNDPROC m_pOldProc;
};

#endif // !defined(AFX_HYPERLINK_H__154C0837_A095_4A53_B1FC_AE766AA60485__INCLUDED_)
