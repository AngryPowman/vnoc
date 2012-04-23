#include "stdafx.h"
#include "SciEdit.h"

//////////////////////////////////////////////////////////////////////////
// CSciEditConfigBase

CSciEditConfigBase::CSciEditConfigBase()
	: m_langID(0),m_lexerID(SCLEX_CPP)
{
	;
}

CSciEditConfigBase::~CSciEditConfigBase()
{
	;
}

HRESULT CSciEditConfigBase::ConfigChanged(const ConfigNode& node)
{
	return E_NOTIMPL;
}

HRESULT CSciEditConfigBase::LanguageChanged(CString language)
{
	return E_NOTIMPL;
}

BOOL GetConfigNode(LPCTSTR path,ConfigNode& node)
{
	return FALSE;
}
