// VNOC.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "VNOCFrame.h"


// CVNOCApp:
// �йش����ʵ�֣������ VNOC.cpp
//

class CVNOCApp : public CWinApp
{
public:
	CVNOCApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int Run();
	virtual int ExitInstance();

private:
	CVNOCFrame m_frame;
};

extern CVNOCApp theApp;