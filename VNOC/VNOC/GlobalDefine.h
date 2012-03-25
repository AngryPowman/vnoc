#pragma once

#define PRODUCT_NAME	_T("VNOC")
#define PathSplit		_T('\\')
#define _LogFileExt		_T(".log")
#define LogMask_ALL		0xffffffff

//////////////////////////////////////////////////////////////////////////
// logœ‡πÿ

#define LogFile_Config	_T("Config")

class CGlobalUtilBase
{
public:
	CString m_file;
	CString	m_prefix;
};
