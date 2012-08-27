#pragma once
#include <atlstr.h>

//////////////////////////////////////////////////////////////////////////
// Լ����

#define VNOC_Private	public

//////////////////////////////////////////////////////////////////////////
// ����

#define PRODUCT_NAME	_T("VNOC")
#define PathSplit		_T('\\')

#define CONFIGXMLFILE	_T("config.xml")
#define ConfigPathSplit	_T('/')				// ����xPath�еķָ���

#define _LogFileExt		_T(".log")
#define _LogFileMaxSizeBytes	64*1024*1024
#define _LogFormatBufferSize	1024
#define LogMask_ALL		0xffffffff

//////////////////////////////////////////////////////////////////////////
// FrameWork

//////////////////////////////////////////////////////////////////////////
// log���

#define LogFile_General	_T("General")
#define LogFile_Config	_T("Config")
#define LogFile_FrameWork	_T("FrameWork")
#define LogFile_Scintilla	_T("Scintilla")
#define LogFile_Net		_T("Net")

class CGlobalUtilBase
{
public:
	CString m_file;
	CString	m_prefix;
};
