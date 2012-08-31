// $_FILEHEADER_BEGIN ****************************
// 文件名称：MacroDefine.h
// 创建日期：2010.12.01
// 创建人：董明
// 文件说明：一些常用的缩写宏，为了代码美观
// $_FILEHEADER_END ******************************

#ifndef _MACRO_DEFINE_H_
#define _MACRO_DEFINE_H_

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)                       \
{                                            \
    if (p != NULL)                           \
    {                                        \
        delete p;                            \
        p = NULL;                            \
    }                                        \
}
#endif

#ifndef SAFE_DELETE_OBJECT
#define SAFE_DELETE_OBJECT(p)                \
{                                            \
    if (p != NULL)                           \
    {                                        \
        ::DeleteObject(p);                   \
        p = NULL;                            \
    }                                        \
}
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p)                 \
{                                            \
    if (p != NULL)                           \
    {                                        \
        delete[] p;                          \
        p = NULL;                            \
    }                                        \
}
#endif

#ifndef SAFE_DELETE_DLG
#define SAFE_DELETE_DLG(p)                   \
{                                            \
    if (p != NULL)                           \
    {                                        \
        if (p->m_hWnd != NULL)               \
        {                                    \
            ::DestroyWindow(p->m_hWnd);      \
        }                                    \
        delete p;                            \
        p = NULL;                            \
    }                                        \
}
#endif

#ifndef SAFE_DELETE_DLL
#define SAFE_DELETE_DLL(p)                   \
{                                            \
    if (p != NULL)                           \
    {                                        \
        ::FreeLibrary(p);                    \
        p = NULL;                            \
    }                                        \
}                                   
#endif                              
                                    
#ifndef SAFE_CLOSE_HANDLE 
#define SAFE_CLOSE_HANDLE(h)                 \
{                                            \
    if (h != NULL)                           \
    {                                        \
        CloseHandle(h);                      \
        h = NULL;                            \
    }                                        \
}
#endif

#ifndef SAFE_CLOSE_REG_KEY
#define SAFE_CLOSE_REG_KEY(p)                \
{                                            \
    if (p != NULL)                           \
    {                                        \
        ::RegCloseKey(p);                    \
        p = NULL;                            \
    }                                        \
}                                   
#endif   


#ifndef SAFE_DELETE_VEC_P
#define SAFE_DELETE_VEC_P(v)                \
{                                           \
    for (size_t i = 0; i < v.size(); i++)   \
    {                                       \
        SAFE_DELETE(v[i]);                  \
    }                                       \
    v.clear();                              \
}
#endif  

#ifndef SAFE_DELETE_MAP_P
#define SAFE_DELETE_MAP_P(m)                \
{                                           \
    for (size_t i = 0; i < m.size(); i++)   \
    {                                       \
        SAFE_DELETE(m[i]);                  \
    }                                       \
    m.clear();                              \
}
#endif 


//////////////////////////////////////////////////////////////////////////
// continue
#ifndef IF_CONTINUE
#define IF_CONTINUE(x)     \
{                          \
    if (x)                 \
    {                      \
        continue;          \
    }                      \
}
#endif

//////////////////////////////////////////////////////////////////////////
// break
#ifndef IF_BREAK
#define IF_BREAK(x)     \
{                       \
    if (x)              \
    {                   \
        break;          \
    }                   \
}
#endif

// break
#ifndef IF_BREAK_ERR
#define IF_BREAK_ERR(x,err)     \
{                       \
	if (x)              \
	{                   \
	{err;}				\
	break;          \
	}                   \
}
#endif

//////////////////////////////////////////////////////////////////////////
// return
#ifndef IF_RETURN
#define IF_RETURN(x)    \
{                       \
    if (x)              \
    {                   \
        return;         \
    }                   \
}
#endif

//////////////////////////////////////////////////////////////////////////
// throw
#ifndef IF_THROW
#define IF_THROW(x, err)    \
{                           \
    if (x)                  \
    {                       \
        throw err;          \
    }                       \
}
#endif

//////////////////////////////////////////////////////////////////////////
// thread
#ifndef SAFE_CLOSE_THREAD
#define SAFE_CLOSE_THREAD(h, timeout)                    \
{                                                        \
    if (h != NULL)                                       \
    {                                                    \
        DWORD dwRes = WaitForSingleObject(h, timeout);   \
        if (dwRes == WAIT_TIMEOUT)                       \
        {                                                \
            BDTerminateThread(h);                    \
        }                                                \
        SAFE_CLOSE_HANDLE(h);                            \
    }                                                    \
}
#endif

//////////////////////////////////////////////////////////////////////////
// internet
#ifndef SAFE_CLOSE_INTERNET
#define SAFE_CLOSE_INTERNET(h)                     \
    if (h != NULL)                                 \
    {                                              \
        InternetCloseHandle(h);                    \
        h = NULL;                                  \
    }
#endif

#endif  // _MACRO_DEFINE_H_

