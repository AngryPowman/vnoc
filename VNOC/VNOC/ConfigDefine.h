#pragma once

#include <vector>
#include <map>
#include <atlsync.h>
#include <atlsafe.h>

// 由于底层库就是通过字符串存取的，所以这里直接使用字符串形式。
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
