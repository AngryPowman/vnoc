#pragma once



// CMainPage ������ͼ

class CMainPage : public CFormView
{
	DECLARE_DYNCREATE(CMainPage)

protected:
	CMainPage();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CMainPage();

public:
	enum { IDD = IDD_FORMVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
};


