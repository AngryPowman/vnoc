#pragma once


// CRoomDlg �Ի���

class CRoomDlg : public CDialog
{
	DECLARE_DYNAMIC(CRoomDlg)

public:
	CRoomDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRoomDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
