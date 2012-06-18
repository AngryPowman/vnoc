// MessageAutoDisposeDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

// CMessageAutoDisposeDlg �Ի���
class CMessageAutoDisposeDlg : public CDialog
{
// ����
public:
	CMessageAutoDisposeDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MESSAGEAUTODISPOSE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CButton m_OK;
	// ��Ϣ�б�
	CTreeCtrl m_Messagelist;
private:
	TVINSERTSTRUCT tvInsert;
	HTREEITEM  m_CTOS;
	HTREEITEM  m_STOC;
	HTREEITEM  m_CTODP;
	HTREEITEM  m_DPTOC;
public:
	afx_msg void OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	// ��Ϣ����
	CEdit m_EditName;
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	// ID
	CEdit m_EditID;
	// �����б�
	CListCtrl m_ParamList;
	// ע�Ϳ��...
	CEdit m_EditNote;
};
