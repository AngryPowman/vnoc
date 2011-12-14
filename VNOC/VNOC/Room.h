#pragma once


// CRoom 对话框

class CRoom : public CDialog
{
	DECLARE_DYNAMIC(CRoom)

public:
	CRoom(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRoom();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	virtual BOOL OnInitDialog();

private:
	HMODULE m_hDll;
	HWND m_CodeCtrl;
};
