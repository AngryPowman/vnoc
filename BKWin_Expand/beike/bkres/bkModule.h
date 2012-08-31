#pragma once

#include <atlapp.h>

class CBkModule
{
private:
	CBkModule()
        :m_pModule(NULL)
        ,m_pMsgloop(NULL)
        ,m_dwRunOneLevel(0)
    {
    }

	CBkModule( CAppModule* appModule,CMessageLoop* msgLoop )
    {
    }

public:
	~CBkModule()
    {
    }

public:
	static CBkModule* GetInstance()
    {
		static CBkModule module;
		return &module;
    }

public:
	void Init( CAppModule* appModule,CMessageLoop* msgLoop )
    {
        m_pModule = appModule;
        m_pMsgloop = msgLoop;
        m_dwRunOneLevel = 0;
    }

    void Release()
    {
        m_pModule = NULL;
        m_pMsgloop = NULL;
        m_dwRunOneLevel = 0;
    }

	void AddRunOneLevel()
    {
        ++m_dwRunOneLevel;
    }

    void ReleaseRunOnLevel()
    {
        --m_dwRunOneLevel;
    }

	DWORD GetRunOneLevel()
    {
        return m_dwRunOneLevel;
    }

	DWORD RunOne( MSG &msg )
    {
        BOOL bDoIdle = TRUE;
        int nIdleCount = 0;
        BOOL bRet = FALSE;
        CMessageLoop *MsgLoop = GetMessageLoop();
        do
        {
            try
            {
                while ( bDoIdle && !::PeekMessage( &MsgLoop->m_msg, NULL, 0, 0, PM_NOREMOVE ) )
                {
                    if ( !MsgLoop->OnIdle( nIdleCount++ ) )
                        bDoIdle = FALSE;
                }
                msg = MsgLoop->m_msg;
    
                if ( msg.message == WM_QUIT )
                    return (DWORD)msg.wParam;
    
                bRet = ::GetMessage( &MsgLoop->m_msg, NULL, 0, 0 );
                if ( bRet == -1 )
                {
                    ATLTRACE2( atlTraceUI, 0, _T( "::GetMessage in Run One returned -1 (error)\n" ) );
                    continue;   // error, don't process
                }
                else if ( !bRet )
                {
                    ATLTRACE2( atlTraceUI, 0, _T( "CMessageLoop::RunOne - exiting\n" ) );
                    break;   // WM_QUIT, exit message loop
                }
    
                if ( !MsgLoop->PreTranslateMessage( &MsgLoop->m_msg ) )
                {
                    ::TranslateMessage( &MsgLoop->m_msg );
                    ::DispatchMessage( &MsgLoop->m_msg );
                }
    
                if ( MsgLoop->IsIdleMessage( &MsgLoop->m_msg ) )
                {
                    bDoIdle = TRUE;
                    nIdleCount = 0;
                }
            }
            catch(...)
            {
            }
        }while ( FALSE );
    
        return (DWORD)MsgLoop->m_msg.wParam;
    }

	DWORD GetMainThreadID()
    {
        if( m_pModule )
        {
            return m_pModule->m_dwMainThreadID;
        }
        return 0;
    }

	CAppModule* GetModule()
    {
        ATLASSERT( m_pModule );
        return m_pModule;
    }

	CMessageLoop* GetMessageLoop()
    {
        ATLASSERT( m_pMsgloop );
        return m_pMsgloop;
    }

private:
	CAppModule*		m_pModule;		// WTL 进程Module
	CMessageLoop*	m_pMsgloop;		// 主消息循环
	DWORD m_dwRunOneLevel;
};

