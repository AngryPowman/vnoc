#include "TLS.h"
#include <WinUser.h>

CTLS::CTLS( LPCTSTR helpString )
{
	m_helpString = helpString;
	m_tlsIndex = TlsAlloc();
	if (m_tlsIndex == 0xffffffff)
	{
		ATLASSERT(FALSE && "TLSError");
		throw helpString;
	}
}

CTLS::~CTLS()
{
	TlsFree(m_tlsIndex);
}

void CTLS::Set( LPVOID value )
{
	TlsSetValue(m_tlsIndex,value);
}

LPVOID CTLS::Get()
{
	return TlsGetValue(m_tlsIndex);
}
