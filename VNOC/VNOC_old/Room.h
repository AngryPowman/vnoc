#pragma once


// CRoom �Ի���

class CRoom : public CDialog
{
	DECLARE_DYNAMIC(CRoom)

public:
	CRoom(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRoom();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	virtual BOOL OnInitDialog();

private:
	HMODULE m_hDll;
	HWND m_CodeCtrl;
};
