#pragma once
#include "SciEdit.h"


// CRoomDlg �Ի���

class CRoomDlg : public CDialog
{
	DECLARE_DYNAMIC(CRoomDlg)

public:
	CRoomDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRoomDlg();

// �Ի�������
	enum { IDD = IDD_RoomDlg };
public:
	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

private:
	CSciEdit m_codeEdit;

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
};
