#pragma once
#include "IModule.h"
#include <vector>
#include <map>

typedef std::map<CString,CString>	AttributeMap;
typedef std::vector<CString>		ConfigPath;

// 存储着属性的所有信息
struct ConfigNode
{
	CString		strValue;		// 值
	AttributeMap attr;			// 属性表
};

// 所有属性都已留在内存中，用时立取，不要缓存，没有多少性能损失
interface IConfig : public IModule
{
public:
	STDMETHOD( LoadConfigFromXML(LPCTSTR filePath) = 0 );
	STDMETHOD( SaveConfigToXML(LPCTSTR filePath) = 0 );
	// 假设Get一定成功。如果xml被删除或加载失败，会加载默认配置
	STDMETHOD( Get(ConfigNode* node) = 0 );
	// 返回值:
	// S_OK		修改成功
	// S_FALSE	原来不存在值，新添。
	STDMETHOD( Set(const ConfigNode& node) = 0 );
};
