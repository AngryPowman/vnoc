#pragma once

//////////////////////////////////////////////////////////////////////////
// 约束宏

#define VNOC_Private	public

//////////////////////////////////////////////////////////////////////////
// 配置

#define PRODUCT_NAME	_T("VNOC")
#define PathSplit		_T('\\')

#define CONFIGXMLFILE	_T("config.xml")
#define ConfigPathSplit	_T('/')				// 用于xPath中的分隔符

#define _LogFileExt		_T(".log")
#define _LogFileMaxSizeBytes	64*1024*1024
#define _LogFormatBufferSize	1024
#define LogMask_ALL		0xffffffff

//////////////////////////////////////////////////////////////////////////
// log相关

#define LogFile_General	_T("General")
#define LogFile_Config	_T("Config")
#define LogFile_Scintilla	_T("Scintilla")
#define LogFile_Net		_T("Net")

class CGlobalUtilBase
{
public:
	CString m_file;
	CString	m_prefix;
};
