// VNOCDlg.h : 头文件
//

#pragma once

#include "HyperLink.h"
#include "VNOCDialogBase.h"

// CVNOCDlg 对话框
class CVNOCLoginDlg : public CDialog
	,public CVNOCDialogBase
{
// 构造
public:
	CVNOCLoginDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_LoginDlg };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
public:
	virtual BOOL Refresh();
	virtual BOOL LanguageChanged();

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CHyperLink m_LinkRegist;
	CHyperLink m_FoundPassword;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

private:
    CString m_strUsername;
    CString m_strPassword;
};
