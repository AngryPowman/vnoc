#pragma once


// CRoomDlg 对话框

class CRoomDlg : public CDialog
{
	DECLARE_DYNAMIC(CRoomDlg)

public:
	CRoomDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRoomDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeRichedit21();
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	virtual BOOL OnInitDialog();

private:	
	HMODULE m_hDll;
	HWND m_CodeCtrl;

};
