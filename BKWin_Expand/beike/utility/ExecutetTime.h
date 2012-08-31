// $_FILEHEADER_BEGIN ****************************
// �ļ����ƣ�ExecuteTime.h
// �������ڣ�2010.09.20
// �����ˣ�����
// �ļ�˵����ͳ��ִ�к�ʱ
//          DPT_FUN     ͳ��һ�������ĺ�ʱ(���ں������)
//          DPT_B1      ͳ������һֱ��}�ĺ�ʱ
//          DPT_B2      ͳ�ƽ�����һ��{}�ĺ�ʱ
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
