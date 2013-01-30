#pragma once
#include "CBuffer.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <memory.h>
#include <stdlib.h>
#include <string>
#include <atlsync.h>
#include <atlstr.h>
#include <shlobj.h>

#define EMPTYSTR _T("")

template<typename T>
class CGlobalPtrContainer
{
public:
	CGlobalPtrContainer(T* p=NULL):m_p(p){};
	~CGlobalPtrContainer(){delete p;};
	void Set(T* p){m_p=p;}
	T* Get(){return m_p;};
private:
	T* m_p;
};

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName&);               \
	void operator=(const TypeName&)
#define _private	public

//////////////////////////////////////////////////////////////////////////
const int Size_KB = 1024;
const int Size_MB = 1024*1024;
const int Size_GB = 1024*1024*1024;
const __int64 Size_KBLL = 1024;
const __int64 Size_TB = Size_KBLL*Size_KBLL*Size_KBLL*Size_KBLL;

namespace Util
{
	char FromHex(const char x);
	char GetHex(UINT num,BOOL bUpperWord=TRUE);
	wchar_t GetHexW(UINT num,BOOL bUpperWord=TRUE);
	namespace String
	{
		CStringW  Unicode_URLDecode(const char* pStr);
		CStringW  Unicode_URLDecode(const wchar_t* pStr);
		CStringW  Unicode_URLEncode(const wchar_t* pStr);
		CStringA Unicode2UTF8(const wchar_t* pUnicode);
		CStringW UTF82Unicode(const char* pChar);

		CStringA UnicodeToGBK(CStringW unicode);
		CStringW GBKToUnicode(CStringA gbk);
		CString PHP_URLEncode(const CStringA& src);
		CStringA PHP_URLDecode(const CStringA& src );
		std::wstring StringA2W(std::string strA);
		std::string StringW2A(std::wstring strW);
		// 不经转换的将char转为wchar_t
		std::wstring StringToWstringNoConvert(std::string strA);
		UINT ToUINT(LPCTSTR lpstr);
	}
	namespace Base64
	{
		BOOL Encode(ConstReferenceBuffer src,BufferOut &dest);
	}
	namespace MD5
	{
		BOOL HashMD5(ConstReferenceBuffer buffer,CString &md5);
		BOOL GetFileMD5(LPCTSTR path, CString& md5);
	}
	namespace Filesys
	{
		BOOL ForceCreateDir(LPCTSTR path);
		// 取系统路径.形如CSIDL_APPDATA
		BOOL GetSpecialPath(DWORD dwSpecDirID,CString &path);
		bool FindFileAttribute(const wchar_t* path, WIN32_FIND_DATA& findFileData);
		bool CheckFileName(const wchar_t * str);
		bool GetDirFromPath(LPCTSTR path,int &endposIndex);// 获取路径中的目录
		BOOL SplitPath(LPCTSTR dirPath,CString& dir,CString& fileName);
		//BOOL PathFileExists(LPCTSTR path);		// Windows有这个API
		bool IsFileInUse(LPCTSTR path);

		void FormatPathSplit(CString& strPath,BOOL slashOrNot=TRUE);
		void FormatPathString( CString& strDirRoot,BOOL slashOrNot=TRUE );
		void FormatDirPathString( CString& strDirRoot,BOOL slashOrNot=TRUE );
		BOOL MakeSurePathFileValid(CString& path);
		std::wstring AutoRenameLocalPathIfExists(const std::wstring& path);
	}

	namespace Time
	{
		 void Time_tToFileTime(time_t t, LPFILETIME pft);
		 void Time_tToSystemTime(time_t t, LPSYSTEMTIME  pst);
		 void FileTimeToTime_t(FILETIME  ft,  time_t* t); 
		 void SystemTimeToTime_t(SYSTEMTIME st,  time_t* t);
		 std::wstring TmtToString(tm time);
	}

	namespace Num
	{
		__int64 CombineToINT64(DWORD dwHight, DWORD dwLow);
		unsigned __int64 CombineToUINT64(DWORD dwHight, DWORD dwLow);
	}

	namespace Size
	{
		std::wstring FormatSize(__int64 nSize);
	}
}

namespace Util
{
	class CAutoCS
	{
	public:
		CAutoCS(::ATL::CCriticalSection &cs);;
		~CAutoCS();
	private:
		::ATL::CCriticalSection& m_cs;
	};

	// 等待有时限的临界区
	class CTimedCriticalSection : public CRITICAL_SECTION
	{
	public:
		CTimedCriticalSection(UINT waitTime = 1000);
		~CTimedCriticalSection();
		BOOL Enter();;
		BOOL Try();
		VOID Leave();
		BOOL IsTimedOut();
	private:
		BOOL m_bTimeout;
		UINT m_waitTime;
		HANDLE m_hEvent;
	};

	class CAutoTimedCS
	{
	public:
		CAutoTimedCS(CTimedCriticalSection &cs);;
		~CAutoTimedCS();
		BOOL IsTimedOut();
	private:
		CTimedCriticalSection& m_cs;
	};

	// 自动分配可用的ID
	class CAutoIndex
	{
	public:
		CAutoIndex();
		~CAutoIndex();
		UINT Get(UINT increase = 1);	// 下一个加几
	private:
		UINT m_index;
	};
}