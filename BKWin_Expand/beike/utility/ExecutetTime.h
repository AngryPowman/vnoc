// $_FILEHEADER_BEGIN ****************************
// 文件名称：ExecuteTime.h
// 创建日期：2010.09.20
// 创建人：董明
// 文件说明：统计执行耗时
//          DPT_FUN     统计一个函数的耗时(放在函数入口)
//          DPT_B1      统计声明一直到}的耗时
//          DPT_B2      统计紧跟的一个{}的耗时
// $_FILEHEADER_END ******************************

#pragma once
#ifndef _EXECUTE_TIME_
#define _EXECUTE_TIME_

#include <MMSystem.h>
#pragma comment(lib, "Winmm.lib") 
#include <DTrace.h>

#define DPT_FUN         CExecuteTime et = __TFUNCTION__;
#define DPT_B1(x)       CExecuteTime et(x);
#define DPT_B2(x)       if (CExecuteTime et = x)

class CExecuteTime
{
public:
    CExecuteTime(LPCTSTR info = NULL)
    {
        m_strInfo = info;
        m_dwBegin = timeGetTime();
    }

    ~CExecuteTime()
    {
        DP_Tag(L"DPT", _T("%s execute %d ms"), m_strInfo, timeGetTime() - m_dwBegin);
    }

    operator BOOL()
    {
        return TRUE;
    }

private:
    CString m_strInfo;
    DWORD   m_dwBegin;
};

#endif
