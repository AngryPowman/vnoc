// VNOCDlg.h : ͷ�ļ�
//

#pragma once

#include "HyperLink.h"
#include "VNOCDialogBase.h"

// CVNOCDlg �Ի���
class CVNOCLoginDlg : public CDialog
	,public CVNOCDialogBase
{
// ����
public:
	CVNOCLoginDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_LoginDlg };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
public:
	virtual BOOL Refresh();
	virtual BOOL LanguageChanged();

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
