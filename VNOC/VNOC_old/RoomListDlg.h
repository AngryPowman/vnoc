#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CMainDlg �Ի���

class CRoomListDlg : public CDialog
{
	DECLARE_DYNAMIC(CRoomListDlg)

public:
	CRoomListDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRoomListDlg();

// �Ի�������
	enum { IDD = IDD_RoomListDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);

private:
private:
	BOOL m_bIsMove;
private:
	CEdit m_SearchEdit;
	CFont m_EditFont;
	CListCtrl m_ClassList;
public:
	afx_msg void OnNMDblclkListClass(NMHDR *pNMHDR, LRESULT *pResult);
};
