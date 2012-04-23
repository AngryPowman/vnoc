#pragma once

#include <vector>
#include <map>
#include <atlsync.h>
#include <atlsafe.h>

// ���ڵײ�����ͨ���ַ�����ȡ�ģ���������ֱ��ʹ���ַ�����ʽ��
typedef std::map<CString,CString>	AttributeMap;
typedef std::vector<CString>		ConfigPath;

struct ConfigNodeBase
{
	ATL::CCriticalSection m_cs;
};

struct ConfigNode;
typedef std::map<CString,ConfigNode*>	ConfigBranchs;
typedef std::vector<ConfigNode>			ConfigVec;
typedef std::vector<ConfigNode*>		ConfigPtrVec;
