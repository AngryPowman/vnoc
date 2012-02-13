#include "CBuffer.h"
#include "stdio.h"
#include <atldef.h>

CBuffer::CBuffer( LPCTSTR _debug/*=NULL*/ )
{
#ifndef _DEBUG
	UNREFERENCED_PARAMETER(_debug);
#endif
	m_pBuf=NULL;
	m_bufSize=0;
	m_attached = FALSE;
#ifdef _DEBUG
	m_debug = _debug;
#endif
}

CBuffer::~CBuffer()
{
	_free();
}

BYTE* CBuffer::Alloc( DWORD size )
{
	void *pTemp = realloc(m_pBuf,size+1);
	if (pTemp != NULL)
	{
		m_pBuf = (BYTE*)pTemp;
		m_bufSize = size;
		m_pBuf[size]=0;
	}
	return m_pBuf;
}

BYTE* CBuffer::GetBuffer() const
{
	return m_pBuf;
}

DWORD CBuffer::GetSize() const
{
	return m_bufSize;
}

const BYTE* CBuffer::Copy( const void* src,DWORD len )
{
	Alloc(len);
	if (m_pBuf)
	{
		::memcpy(m_pBuf,src,len);
		m_bufSize=len;
	}
	return *this;
}

const BYTE* CBuffer::Copy( const CBuffer& src )
{
	return Copy(src,src.GetSize());
}

DWORD CBuffer::Write( void* p,DWORD size,DWORD startPos )
{
	if (startPos >= m_bufSize || p==NULL || m_pBuf==NULL)
	{
		return 0;
	}
	if (startPos + size >= m_bufSize)
	{
		size = m_bufSize-startPos-1;
	}
	if (size)
	{
		::memcpy(m_pBuf+startPos,p,size);
	}
	return size;
}

CBuffer::operator const BYTE*() const
{
	return m_pBuf;
}

void CBuffer::_free()
{
	if (m_attached)
	{
		m_attached = FALSE;
		m_pBuf = NULL;
		m_bufSize = 0;
		return;
	}
	if(m_pBuf)
	{
		free(m_pBuf);
		m_pBuf = NULL;
		m_bufSize = 0;
	}
}

void CBuffer::memset( byte value/*=0*/ )
{
	::memset(m_pBuf,value,m_bufSize);
}

BOOL CBuffer::WriteToFile( LPCTSTR filePath ) const
{
	FILE* fp=NULL;
	_wfopen_s(&fp,filePath,L"wb");
	if (fp)
	{
		size_t temp;
		temp = fwrite(GetBuffer(),GetSize(),1,fp);
		ATLASSERT(temp == 1);
		fclose(fp);
		return TRUE;
	}
	return FALSE;
}

VOID CBuffer::Clear()
{
	_free();
}

VOID CBuffer::Attach( void* buffer,size_t bufsize )
{
	ATLASSERT(buffer);
	if (buffer && bufsize)
	{
		_free();
		m_attached = TRUE;
		m_pBuf = (BYTE*)buffer;
		m_bufSize = (DWORD)bufsize;
	}
}

VOID CBuffer::Detach()
{
	_free();
}

//////////////////////////////////////////////////////////////////////////

CStreamBuffer::CStreamBuffer()
{
	m_writePos = 0;
	m_readPos = 0;
}

CStreamBuffer::~CStreamBuffer()
{
}

DWORD CStreamBuffer::Append( const void *pData,DWORD dataSize )
{
	if (m_writePos >= m_bufSize || pData==NULL || m_pBuf==NULL)
	{
		return 0;
	}
	if (m_writePos+dataSize > m_bufSize)
	{
		dataSize = m_bufSize-m_writePos-1;
	}
	if (dataSize)
	{
		::memcpy(m_pBuf+m_writePos,pData,dataSize);
		m_writePos += dataSize;
	}
	return dataSize;
}

DWORD CStreamBuffer::Get( void *pOut,DWORD bytesToRead )
{
	if (m_pBuf==NULL || pOut==NULL || bytesToRead==0)
	{
		return 0;
	}
	if (bytesToRead+m_readPos >= m_writePos)
	{
		bytesToRead = m_writePos-m_readPos;
	}
	if (bytesToRead)
	{
		::memcpy(pOut,GetBuffer() + m_readPos,bytesToRead);
		m_readPos += bytesToRead;
	}
	return bytesToRead;
}

void CStreamBuffer::ResetWritePos()
{
	m_writePos = 0;
}

void CStreamBuffer::ResetReadPos()
{
	m_readPos = 0;
}