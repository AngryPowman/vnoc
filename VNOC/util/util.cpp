#include "stdafx.h"
#include "util.h"
#include "MD5.h"

namespace Util
{
	// 用于缓存Util中使用的可复用的数据
	class CUtilData
	{
	public:
		CUtilData()
		{
			m_pIMalloc = NULL;
			SHGetMalloc(&m_pIMalloc);
		};
		~CUtilData()
		{
			if(m_pIMalloc)
			{
				m_pIMalloc->Release();
			}
		}
	public:
		LPMALLOC GetLPMALLOC()
		{
			if (!m_pIMalloc)
			{
				SHGetMalloc(&m_pIMalloc);
			}
			return m_pIMalloc;
		}
	private:
		IMalloc *m_pIMalloc;
	}Data;
}

std::wstring Util::String::StringA2W( std::string strA )
{
	const char *szStr = strA.c_str(); 
	std::wstring wszStr;
	if( NULL == szStr )
	{
		return wszStr;
	}
	DWORD iSize = ::MultiByteToWideChar(  CP_ACP, 0, szStr, -1, NULL, 0 );
	wchar_t* p = new wchar_t[iSize+10];
	memset( p, 0, (iSize+10)*sizeof(wchar_t) );
	iSize = ::MultiByteToWideChar( CP_ACP, 0, szStr, -1, p, iSize);
	wszStr = p;
	delete p;
	return wszStr;
}

std::string Util::String::StringW2A( std::wstring strW )
{
	const wchar_t *wszStr = strW.c_str(); 
	std::string szStr;
	if( NULL == wszStr )
	{
		return szStr;
	}
	DWORD iSize = ::WideCharToMultiByte(  CP_ACP, 0, wszStr, -1, NULL, 0 ,NULL,NULL);
	char* p = new char[iSize+10];
	memset( p, 0, (iSize+10)*sizeof(char) );
	iSize = ::WideCharToMultiByte( CP_ACP, 0, wszStr, -1, p, iSize,NULL,NULL);
	szStr = p;
	delete p;
	return szStr;
}

std::wstring Util::String::StringToWstringNoConvert(std::string strA)
{
	std::wstring strW;
	wchar_t *wp;
	strW.resize(strA.length());
	wp = const_cast<wchar_t*>(strW.data());
	memcpy(wp,strA.data(),strA.length());
	return strW;
}

CStringA Util::String::UnicodeToGBK( CStringW unicode )
{
	CStringA strGBK = "";
	DWORD dwMinSize = 0;
	dwMinSize = WideCharToMultiByte(936, NULL, unicode, unicode.GetLength(),NULL, 0, NULL, FALSE);
	strGBK.GetBufferSetLength(dwMinSize);
	LPSTR lpszStr =  strGBK.GetBuffer();
	INT ok = WideCharToMultiByte(936, NULL, unicode, unicode.GetLength(), lpszStr, dwMinSize, NULL, FALSE);
	strGBK.ReleaseBuffer();
	return strGBK;
}

CStringW Util::String::GBKToUnicode( CStringA gbk )
{
	CStringW strUnicode;
	DWORD dwMinSize = 0;
	dwMinSize = MultiByteToWideChar(936, NULL, gbk, gbk.GetLength(),NULL, 0);
	strUnicode.GetBufferSetLength(dwMinSize);
	LPWSTR lpszStr =  strUnicode.GetBuffer();
	INT ok = MultiByteToWideChar(936, NULL, gbk, gbk.GetLength(), lpszStr, dwMinSize);
	strUnicode.ReleaseBuffer();
	return strUnicode;
}

char Util::GetHex( UINT num,BOOL bUpperWord/*=TRUE*/ )
{
	return num<10? num+'0': num-10+'A';
}

inline char Util::FromHex(const char x)
{
	return isdigit(x) ? x-'0' : x-'A'+10;
}


wchar_t Util::GetHexW( UINT num,BOOL bUpperWord/*=TRUE*/ )
{
	return num<10? num+L'0': num-10+L'A';
}

CStringA Util::String::Unicode2UTF8(const wchar_t* pUnicode)   
{   
	ATLASSERT(NULL != pUnicode);

	int targetLen = WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)pUnicode, -1, NULL, 0, NULL, NULL);   

	char* pTargetData = new char[targetLen+1];   
	memset(pTargetData, 0, targetLen+1);   
	WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)pUnicode, -1, (char *)pTargetData, targetLen, NULL, NULL);   

	CStringA strRet(pTargetData);
	delete pTargetData;   
	return strRet;   
}

CStringW Util::String::UTF82Unicode(const char* pChar)   
{   
	ATLASSERT(NULL != pChar);

	int targetLen = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)pChar, -1, NULL, 0);   

	WCHAR* pTargetData = new WCHAR[targetLen+1];   
	memset(pTargetData, 0, (targetLen+1) * sizeof(WCHAR));   
	MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)pChar, -1, (WCHAR*)pTargetData, targetLen);   

	CStringW strRet(pTargetData);
	delete pTargetData;   
	return strRet;   
}


CStringW Util::String::Unicode_URLEncode(const wchar_t* pStr)
{
	return PHP_URLEncode(Unicode2UTF8(pStr));
}

CStringW Util::String::Unicode_URLDecode(const char* pStr)
{
	return UTF82Unicode(PHP_URLDecode(pStr));
}

CStringW Util::String::Unicode_URLDecode(const wchar_t* pStr)
{
	return UTF82Unicode(PHP_URLDecode(CStringA(pStr)));
}

CStringA Util::String::PHP_URLDecode( const CStringA& src )
{
	CStringA decodeURL;
	for(int i = 0; i < src.GetLength(); i++)
	{
		unsigned char ch = 0;
		if(src[i]=='%')
		{
			ATLASSERT(i + 2 < src.GetLength());

			unsigned char c1 = src[i + 1];
			unsigned char c2 = src[i + 2];
			ch = ((FromHex(c1) << 4) & 0xf0);
			ch |= (FromHex(c2) & 0x0f);
			i += 2;
		}
		else if(src[i] == '+')
		{
			ch = ' ';
		}
		else
		{
			ch = src[i];
		}
		decodeURL += (unsigned char)ch;
	}
	
	return decodeURL;
}

CString Util::String::PHP_URLEncode( const CStringA& src )
{
	CString encodeURL;

	for(int i = 0; i < src.GetLength(); i++)
	{
		unsigned char c = src.GetAt(i);
		if (c == ' ')
		{
			encodeURL += L'+';
		}
		else if ((c < '0' && c != '-' && c != '.') ||
			(c < 'A' && c > '9') ||
			(c > 'Z' && c < 'a' && c != '_') ||
			(c > 'z'))
		{
			encodeURL += '%';
			encodeURL += GetHex((c & 0xf0) >> 4);
			encodeURL += GetHex(c & 0x0f);
		}
		else
		{
			encodeURL += c;
		}
	}
	return encodeURL;
}

BOOL Util::Base64::Encode( ConstReferenceBuffer src,BufferOut &dest )
{
	BOOL bRet = FALSE;

	if (src==NULL)
	{
		return FALSE;
	}
	DWORD dwInputLen = src.GetSize();
	DWORD dwOutputLen = (dwInputLen / 3 + ((dwInputLen % 3)? 1: 0)) * 4 + 1;

	//编码表
	const char EncodeTable[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	//返回值

	BYTE* data = src.GetBuffer();
	CStringA destStr;
	UINT LineLength;
	LineLength = 0;
	for(int i=0;i<(int)(dwInputLen / 3);i++)
	{
		destStr += EncodeTable[data[0] >> 2];
		destStr += EncodeTable[((data[0] << 4) | (data[1] >> 4)) & 0x3F];
		destStr += EncodeTable[((data[1] << 2) | (data[2] >> 6)) & 0x3F];
		destStr += EncodeTable[data[2] & 0x3F];
		data += 3;
		LineLength += 4;
		if(LineLength==76)
		{
			destStr += "\r\n";
			LineLength=0;
		}
	}
	//对剩余数据进行编码
	int Mod=dwInputLen % 3;
	if(Mod==1)
	{
		destStr += EncodeTable[(data[0] & 0xFC) >> 2];
		destStr += EncodeTable[((data[0] & 0x03) << 4)];
		destStr += "==";
	}
	else if(Mod==2)
	{
		destStr += EncodeTable[(data[0] & 0xFC) >> 2];
		destStr += EncodeTable[((data[0] & 0x03) << 4) | ((data[1] & 0xF0) >> 4)];
		destStr += EncodeTable[((data[1] & 0x0F) << 2)];
		destStr += "=";
	}
	dest.Copy(destStr.GetBuffer(destStr.GetLength()),destStr.GetLength());
	destStr.ReleaseBuffer(destStr.GetLength());
	return TRUE;
}

BOOL Util::MD5::HashMD5( ConstReferenceBuffer buffer,CString &md5 )
{
	md5 = CMD5Checksum::GetMD5(buffer.GetBuffer(),buffer.GetSize());
	return !md5.IsEmpty();
}

BOOL Util::MD5::GetFileMD5(const wchar_t* path, CString& md5)
{
	return CMD5Checksum::GetMD5(path, md5);
}

BOOL Util::Filesys::ForceCreateDir( LPCTSTR path )
{
#define MAX_URL 2048
#define WIN_DIR_SEP_SYM   _T("\\")
	TCHAR   Dir[MAX_URL+1] = {0};
	int	 nNeed;
	LPCTSTR p,pLast;

	_ASSERT( path );

	pLast = path;
	while( NULL!=*pLast )
	{
		p=_tcsstr(pLast,WIN_DIR_SEP_SYM);
		if( NULL==p ) return FALSE;
		nNeed=int(p-path);
		if(nNeed>0)
		{
			memset( Dir,0,sizeof(Dir) );
			_tcsncpy_s( Dir,MAX_URL,path,nNeed );
			BOOL b = CreateDirectory(Dir,NULL);
			if (!b)
			{
				// 这里会出现两个错误：
				// 已存在，那么接着创建子目录，忽略错误
				// 拒绝访问，父目录拒绝访问，子目录可未必，所以也忽略
				// 总之，忽略所有错误。。
			}
		}
		p++;
		pLast=p;
	}
	return TRUE;
#undef MAX_URL
#undef WIN_DIR_SEP_SYM
}

BOOL Util::Filesys::GetSpecialPath( DWORD dwSpecDirID,CString &path )
{
	BOOL bResult = FALSE;
	LPITEMIDLIST pidl;
	LPMALLOC pShellMalloc=Util::Data.GetLPMALLOC();
	
	if(pShellMalloc != NULL)
	{
		if(SUCCEEDED(SHGetSpecialFolderLocation(NULL,dwSpecDirID,&pidl)))
		{
			if(SHGetPathFromIDList(pidl,path.GetBuffer(MAX_PATH)))
			{
				bResult = TRUE;
			}
			path.ReleaseBuffer();
			pShellMalloc->Free(pidl);
		}
	}
	return bResult;
}

bool Util::Filesys::FindFileAttribute(const wchar_t* path, WIN32_FIND_DATA& findFileData)
{
	ATLASSERT(NULL != path);

	memset(&findFileData, 0, sizeof(WIN32_FIND_DATA));
	HANDLE hFind = FindFirstFile(path, &findFileData);
	if(INVALID_HANDLE_VALUE != hFind)
	{
		FindClose(hFind);
		return true;
	}
	return false;
}

bool Util::Filesys::IsFileInUse(const wchar_t* path)
{
	ATLASSERT(NULL != path);
	if(!PathFileExists(path))
	{
		return false;
	}

	HANDLE handle = CreateFile(path, GENERIC_READ | GENERIC_WRITE, 
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); 

	bool ret = (INVALID_HANDLE_VALUE == handle); 
	if(!ret) 
	{
		CloseHandle(handle); 
	}
	return ret; 
}

void Util::Time::Time_tToFileTime(time_t t, LPFILETIME pft)
{
	ATLASSERT(NULL != pft);

	// Note that LONGLONG is a 64-bit value
	LONGLONG ll;

	ll = Int32x32To64(t, 10000000) + 116444736000000000;
	pft->dwLowDateTime = (DWORD)ll;
	pft->dwHighDateTime = (DWORD)(ll >> 32);
}

void Util::Time::FileTimeToTime_t(FILETIME  ft,  time_t* t)
{
	ATLASSERT(NULL != t);

	LONGLONG  ll;  

	ULARGE_INTEGER  ui;  
	ui.LowPart  = ft.dwLowDateTime;  
	ui.HighPart = ft.dwHighDateTime;  
	ll = (LONGLONG(ft.dwHighDateTime) << 32) + ft.dwLowDateTime;  
	*t = ((LONGLONG)(ui.QuadPart  -  116444736000000000)  /  10000000);  
}

void Util::Time::Time_tToSystemTime(time_t t, LPSYSTEMTIME  pst)
{
	ATLASSERT(NULL != pst);

	FILETIME ft;
	Time_tToFileTime(t, &ft);
	FileTimeToSystemTime(&ft, pst);
}

void Util::Time::SystemTimeToTime_t(SYSTEMTIME st,  time_t* t)
{
	ATLASSERT(NULL != t);

	FILETIME ft;
	SystemTimeToFileTime(&st, &ft);
	FileTimeToTime_t(ft, t);
}

std::wstring Util::Time::TmtToString( tm time )
{
	CStringW strRes;
	strRes.Format(L"%d年%d月%d日%d分%d秒",time.tm_year,time.tm_mon,time.tm_mday,time.tm_hour,time.tm_min,time.tm_sec);
	return std::wstring(strRes);
}
__int64 Util::Num::CombineToINT64(DWORD dwHight, DWORD dwLow)
{
	return __int64((__int64(dwHight) << 32) + dwLow);
}

unsigned __int64 Util::Num::CombineToUINT64(DWORD dwHight, DWORD dwLow)
{
	return unsigned __int64((unsigned __int64(dwHight) << 32) + dwLow);
}

std::wstring Util::Size::FormatSize(__int64 nSize)
{
	CStringW strRes;
	if (nSize<Size_KB)
	{
		strRes.Format(L"%I64d b", nSize);
	}
	else if (nSize>=Size_KB && nSize<Size_MB)
	{
		strRes.Format(L"%.2f K", (double)nSize/(double)Size_KB);
	}
	else if (nSize>=Size_MB && nSize<Size_GB)
	{
		strRes.Format(L"%.2f M", (double)nSize/(double)Size_MB);
	}
	else if (nSize>=Size_GB && nSize<Size_TB)
	{
		strRes.Format(L"%.2f G", (double)nSize/(double)Size_GB);
	}
	else if(nSize>=Size_TB)
	{
		int nSizeGB = (int)(nSize/Size_KB);
		strRes.Format(L"%.2f T", (double)nSizeGB/(double)Size_GB);
	}
	return strRes.GetString();
}


bool Util::Filesys::CheckFileName( const wchar_t * str )
{
	while(*str != L'\0')
	{
		if(
			*str == L'/' ||
			*str == L'\\'||
			*str == L';' ||
			*str == L':' ||
			*str == L'*' ||
			*str == L'?' ||
			*str == L'\"'||
			*str == L'<' ||
			*str == L'>'
			)
			return false;
		++str;
	}
	return true;
}

bool Util::Filesys::GetDirFromPath( LPCTSTR path,int &endposIndex )
{
	int length = (int)_tcslen(path);
	for (int n=length-1; n>=0; --n)
	{
		if (path[n]==L'\\' || path[n]==L'/')
		{
			endposIndex = n;
			return true;
		}
	}
	endposIndex = 0;
	return false;
}

UINT Util::String::ToUINT( LPCTSTR lpstr )
{
	ATLASSERT(lpstr);
	DWORD dw = 0;
	if (lpstr)
	{
		while(*lpstr)
		{
			dw *= 10;
			dw += *lpstr - '0';
			++lpstr;
		}
	}
	return dw;
}

namespace Util
{
	CAutoCS::CAutoCS( ::ATL::CCriticalSection &cs ) :m_cs(cs)
	{
		m_cs.Enter();
	}
	CAutoCS::~CAutoCS()
	{
		m_cs.Leave();
	}


	CTimedCriticalSection::CTimedCriticalSection( UINT waitTime /*= 1000*/ ) : m_waitTime(waitTime)
	{
		m_hEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
		ATLASSERT(m_hEvent);
		m_bTimeout = FALSE;
	}
	CTimedCriticalSection::~CTimedCriticalSection()
	{
		if (m_hEvent)
		{
			CloseHandle(m_hEvent);
		}
	}
	BOOL CTimedCriticalSection::Enter()
	{
		ATLASSERT(m_hEvent);
		DWORD dw = WaitForSingleObject(m_hEvent,m_waitTime);
		m_bTimeout = (dw==WAIT_TIMEOUT);
		return m_bTimeout;
	}
	BOOL CTimedCriticalSection::Try()
	{
		ATLASSERT(m_hEvent);
		DWORD dw = WaitForSingleObject(m_hEvent,0);
		m_bTimeout = (dw==WAIT_TIMEOUT);
		return m_bTimeout;
	}
	VOID CTimedCriticalSection::Leave()
	{
		ATLASSERT(m_hEvent);
		SetEvent(m_hEvent);
	}
	BOOL CTimedCriticalSection::IsTimedOut()
	{
		return m_bTimeout;
	}


	CAutoTimedCS::CAutoTimedCS( CTimedCriticalSection &cs ) :m_cs(cs)
	{
		m_cs.Enter();
	}
	BOOL CAutoTimedCS::IsTimedOut()
	{
		return m_cs.IsTimedOut();
	}
	CAutoTimedCS::~CAutoTimedCS()
	{
		m_cs.Leave();
	}

	CAutoIndex::CAutoIndex()
	{
		m_index = 0;
	}
	CAutoIndex::~CAutoIndex()
	{}
	UINT CAutoIndex::Get(UINT increase)
	{
		UINT ret = m_index;
		m_index += increase;		// 利用溢出
		return ret;
	}
}