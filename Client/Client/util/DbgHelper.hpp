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

/** ���õ�ǰ�߳�����
*  ͨ����һ��������쳣���õ������洢����ʾ�߳�����
*  \param dwThreadID �߳�ID, -1��ʾ��ǰ�߳�
*  \param pszThreadName �߳���
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