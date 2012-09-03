// VNOCDlg.h : 头文件
//

#pragma once

#include "VNOCDialogBase.h"
#include "LoginImpl.h"
#include "INet.h"
#include "HyperLink.h"

// CVNOCDlg 对话框
class CVNOCLoginDlg : public CDialog
	,public CVNOCDialogBase
	,public INetListener
{
// 构造
public:
	CVNOCLoginDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CVNOCLoginDlg();

// 对话框数据
	enum { IDD = IDD_LoginDlg };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
public:
	virtual BOOL Refresh();
	virtual BOOL LanguageChanged();

	STDMETHOD( OnMessage(const CMessage& msg));

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	void _SetVerifyState(BOOL bVerifying);

private:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);

private:
	CHyperLink m_LinkRegist;
	CHyperLink m_FoundPassword;
    CString m_strUsername;
    CString m_strPassword;
	BOOL m_bVerifying;

	CNetListenerHelper m_nlHelper;
};
