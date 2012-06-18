// MainPage.cpp : 实现文件
//

#include "stdafx.h"
#include "VNOC.h"
#include "MainPage.h"


// CMainPage

IMPLEMENT_DYNCREATE(CMainPage, CFormView)

CMainPage::CMainPage()
	: CFormView(CMainPage::IDD)
{

}

CMainPage::~CMainPage()
{
}

void CMainPage::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMainPage, CFormView)
END_MESSAGE_MAP()


// CMainPage 诊断

#ifdef _DEBUG
void CMainPage::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMainPage::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMainPage 消息处理程序

