// $_FILEHEADER_BEGIN ****************************
// 文件名称：DTrace.h
// 创建日期：2010.12.01
// 创建人：董明
// 文件说明：调试日志类debug trace
// 参数设置：   SetAppName      设置产品名
// 使用方法:    DP              无条件输出
//              DP_Tag          带Tag输出
//              DP_Debug        Tag=Debug输出
//              DP_Info         Tag=Info输出
//              DP_Warning      Tag=Warning输出
//              DP_Error        Tag=Error输出
//              DP_Critical     Tag=Critical输出
// $_FILEHEADER_END ******************************
#pragma once

#include <atlstr.h>
#include <set>
#include <time.h>

//////////////////////////////////////////////////////////////////////////
// prepare
#ifdef  UNICODE
    #define __TFILE__       __FILEW__
    #define __TFUNCTION__   __FUNCTIONW__
#else
    #define __TFILE__       __FILE__
    #define __TFUNCTION;__   __FUNCTION__
#endif

#define DP(...)             CDTrace::GetInstance().Prepare(__TFILE__, __LINE__, __TFUNCTION__).Format(__VA_ARGS__)
#define DP_Tag(tag, ...)    if (CDTrace::GetInstance().CanTrace(tag))   DP(__VA_ARGS__)
#define DP_Debug(...)       DP_Tag(L"Debug",    __VA_ARGS__)
#define DP_Info(...)        DP_Tag(L"Info",     __VA_ARGS__)
#define DP_Warning(...)     DP_Tag(L"Warning",  __VA_ARGS__)
#define DP_Error(...)       DP_Tag(L"Error",    __VA_ARGS__)
#define DP_Critical(...)    DP_Tag(L"Critical", __VA_ARGS__)

//////////////////////////////////////////////////////////////////
// CDTrace
class CDTrace
{
private:
    CDTrace();
    ~CDTrace();

public:
    static CDTrace& GetInstance();

    void        SetAppName(LPCTSTR szName);
    CDTrace&    Prepare(LPCTSTR szSrcFile, int iSrcLen, LPCTSTR szFunc);
    void        Format(LPCTSTR szFormat, ...);
    BOOL        CanTrace(LPCTSTR szTag);

private:
    void        InitBuf(int iLen);
    void        FreeBuf();
    void        LoadConfig();

private:
    enum OptionsEnum
    {	
        OE_Print        = 0x1,                      // 输出日志      
        OE_LogToFile    = (OE_Print << 1),          // 把日志追加到文件中
        OE_Timestamp    = (OE_Print << 2),          // 打印时间
        OE_FileAndLine  = (OE_Print << 3),          // 打印源文件名和行号	 
        OE_Fuction      = (OE_Print << 4),          // 打印函数名	 
        OE_PID          = (OE_Print << 5),          // 打印进程号
    };

    const static int    MIN_PRINT_BUF_LEN = 512;    // 512 as min
    const static int    SECTION_SIZE = 4096;

    TCHAR*              m_pPrintBuf;                // 打印数据缓存
    int                 m_iMaxLen;                  // 缓冲长度
    int                 m_iDataLen;                 // 数据长度
    CString             m_strAppName;               // 日志AppName
    CString             m_strSavePath;              // 日志文件保存路径
    DWORD               m_dwTraceOptions;           // 打印日志选项	
    time_t              m_tPreReadConfig;           // 上次加载日志时间
    CRITICAL_SECTION    m_cs;                       // 打印日志同步临界区
    std::set<CString>   m_Tags;                     // Tags
};

inline CDTrace::CDTrace()
{
    m_tPreReadConfig = 0;
    m_strAppName = _T("Default");
    LoadConfig();

    InitializeCriticalSection(&m_cs);
    m_pPrintBuf = NULL;
    m_dwTraceOptions = OE_Print | OE_LogToFile | OE_Timestamp | OE_Fuction | OE_PID;
    InitBuf(m_iMaxLen);	
}

inline CDTrace::~CDTrace()
{
    FreeBuf();
    DeleteCriticalSection(&m_cs);
}

inline void CDTrace::FreeBuf()
{
    if (m_pPrintBuf != NULL)
    {
        delete[] m_pPrintBuf;
        m_pPrintBuf = NULL;
    }
}

inline void CDTrace::InitBuf(int iLen)
{
    FreeBuf();
    m_iMaxLen = (iLen < MIN_PRINT_BUF_LEN) ? MIN_PRINT_BUF_LEN : iLen;
    m_iDataLen = 0;

    m_pPrintBuf = new TCHAR[m_iMaxLen];	
}

inline CDTrace& CDTrace::GetInstance()
{
    static CDTrace s_dt;
    return s_dt;
}

inline void CDTrace::SetAppName(LPCTSTR szName)
{
    m_strAppName = szName;
    LoadConfig();
}

inline CDTrace& CDTrace::Prepare(LPCTSTR szSrcFile, int iSrcLen, LPCTSTR szFunc)
{
    EnterCriticalSection(&m_cs);

    LoadConfig();

    m_iDataLen = 0;
    if (m_dwTraceOptions & OE_Timestamp) 
    {
        SYSTEMTIME _curT;
        GetLocalTime(&_curT);
        m_iDataLen += _stprintf_s(m_pPrintBuf + m_iDataLen, m_iMaxLen - m_iDataLen, _T("[%02d:%02d:%02d:%03d] ")
            , _curT.wHour, _curT.wMinute, _curT.wSecond, _curT.wMilliseconds);
    }
    if (m_dwTraceOptions & OE_PID)
    {
        m_iDataLen += _stprintf_s(m_pPrintBuf + m_iDataLen, m_iMaxLen - m_iDataLen, _T("[P.%d] ")
            , GetCurrentProcessId());
    }
    if (m_dwTraceOptions & OE_FileAndLine) 
    {
        m_iDataLen += _stprintf_s(m_pPrintBuf + m_iDataLen, m_iMaxLen - m_iDataLen, _T("[A.%s:%d] ")
            , szSrcFile, iSrcLen);
    }
    if (m_dwTraceOptions & OE_Fuction)
    {
        m_iDataLen += _stprintf_s(m_pPrintBuf + m_iDataLen, m_iMaxLen - m_iDataLen, _T("[F.%s()] ")
            , szFunc);
    }
    return *this;
}

inline void CDTrace::Format(LPCTSTR szFormat, ...)
{
    va_list argList;    
    va_start(argList, szFormat);
    m_iDataLen += _vstprintf_s(m_pPrintBuf + m_iDataLen, m_iMaxLen - m_iDataLen, szFormat, argList);
    va_end(argList);

    if (m_dwTraceOptions & OE_Print) 
    {
        OutputDebugString(m_pPrintBuf);
        OutputDebugString(_T("\n"));
    }

    if ((m_dwTraceOptions & OE_LogToFile) && !m_strSavePath.IsEmpty())
    {
        FILE* pFile = NULL;
        if (_tfopen_s(&pFile, m_strSavePath, _T("a, ccs=UTF-8")) == 0)
        {
            _ftprintf_s(pFile, _T("%s\n"), m_pPrintBuf);
            fclose(pFile);
        }
    }

    LeaveCriticalSection(&m_cs);
}

inline void CDTrace::LoadConfig()
{
    if (time(NULL) < m_tPreReadConfig + 60)
    {
        return;
    }

    TCHAR buffer[MAX_PATH] = {0};
    DWORD dw = ::GetModuleFileName(NULL, buffer, MAX_PATH);
    CString strTemp(buffer);
    CString strPath = strTemp.Left(strTemp.ReverseFind('\\') + 1);
    CString strConfigPath = strPath + _T("DTrace.ini");

    DWORD dwOption = GetPrivateProfileInt(m_strAppName, _T("Option"), 0xf000, strConfigPath);
    if (dwOption != 0xf000)
    {
        m_dwTraceOptions = dwOption;
    }

    GetPrivateProfileString(m_strAppName, _T("SavePath"), NULL, m_strSavePath.GetBuffer(_MAX_PATH), _MAX_PATH, strConfigPath);
    m_strSavePath.ReleaseBuffer();    

    CString strSectionApp;
    GetPrivateProfileString(m_strAppName, _T("Tags"), NULL, strSectionApp.GetBuffer(_MAX_PATH), _MAX_PATH, strConfigPath);
    strSectionApp.ReleaseBuffer();
    m_Tags.clear();
    if (!strSectionApp.IsEmpty())
    {
        CString strBuffer;
        GetPrivateProfileSection(strSectionApp, strBuffer.GetBuffer(SECTION_SIZE), SECTION_SIZE, strConfigPath);
        strBuffer.ReleaseBuffer();
        LPCTSTR p = strBuffer;
        while (*p)
        {
            m_Tags.insert(p);
            p += _tcslen(p) + 1;
        }
    }

    m_tPreReadConfig = time(NULL);
}

inline BOOL CDTrace::CanTrace(LPCTSTR szTag)
{
    BOOL bCan = FALSE;
    if (m_Tags.empty())
    {
        bCan = TRUE;
    }
    else
    {
        bCan = (m_Tags.find(szTag) != m_Tags.end());
    }
    return bCan;
}