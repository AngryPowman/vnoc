// VNOC.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "VNOC.h"

#ifdef USE_VLD
#ifdef _DEBUG
#include "vld.h"
#endif
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVNOCApp

BEGIN_MESSAGE_MAP(CVNOCApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CVNOCApp ����

CVNOCApp::CVNOCApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CVNOCApp ����

CVNOCApp theApp;


// CVNOCApp ��ʼ��

BOOL CVNOCApp::InitInstance()
{
	setlocale(LC_ALL,"chs");
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	Global->SetThreadName(_T("App���߳�"));
	Global->Initialize();
	m_frame.Initialize();
	//TODO: ����û������m_pMainWnd

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return TRUE;
}


int CVNOCApp::Run()
{
	// TODO: Add your specialized code here and/or call the base class

	m_frame.Run();
	//ExitInstance();
	return CWinApp::Run();
}


int CVNOCApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class
	m_frame.UnInitialize();
	Global->UnInitialize();
	return CWinApp::ExitInstance();
}
