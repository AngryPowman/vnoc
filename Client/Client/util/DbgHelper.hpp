#pragma once

#pragma pack(push,8)
struct ThreadNameInfo
{
	DWORD  dwType;
	LPCSTR szName;
	DWORD  dwThreadID;
	DWORD  dwFlags;
};
#pragma pack(pop)

/** 设置当前线程名字
*  通过抛一个特殊的异常，让调试器存储并显示线程名字
*  \param dwThreadID 线程ID, -1表示当前线程
*  \param pszThreadName 线程名
*/
inline void SetThreadNameToDebugInfo(DWORD dwThreadID, LPCSTR pszThreadName)
{
	if (!::IsDebuggerPresent()) return;

	if (dwThreadID == -1)
	{
		dwThreadID = ::GetCurrentThreadId();
	}

	struct ThreadNameInfo info = {0x1000, pszThreadName, dwThreadID, 0};

	__try
	{
		RaiseException(0x406D1388, 0, sizeof(info)/sizeof(ULONG_PTR), (ULONG_PTR*)&info);
	}
	__except(EXCEPTION_CONTINUE_EXECUTION)
	{
	}
}