#pragma once
#include "SciEdit.h"


// CRoomDlg 对话框

class CRoomDlg : public CDialog
{
	DECLARE_DYNAMIC(CRoomDlg)

public:
	CRoomDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRoomDlg();

// 对话框数据
	enum { IDD = IDD_RoomDlg };
public:
	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

private:
	CSciEdit m_codeEdit;

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
};
