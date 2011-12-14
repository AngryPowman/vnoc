#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CMainDlg 对话框

class CMainDlg : public CDialog
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	CMainDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMainDlg();

// 对话框数据
	enum { IDD = IDD_MAINDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);

private:
	BOOL m_bIsMove;
private:
	CEdit m_SearchEdit;
	CFont m_EditFont;
	CListCtrl m_ClassList;
};
