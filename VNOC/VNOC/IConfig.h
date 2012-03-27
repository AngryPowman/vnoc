#pragma once
#include "IModule.h"
#include <vector>
#include <map>

typedef std::map<CString,CString>	AttributeMap;
typedef std::vector<CString>		ConfigPath;

// 存储着属性的所有信息
struct ConfigNode
{
	ConfigPath	path;			// 路径
	void xPath(LPCTSTR path);	// 可以填入"VNOC/Language/UI"这样的字符串，函数负责拆分
	CString		strValue;		// 值
	AttributeMap attr;			// 属性表
};

interface IConfigMonitor
{
	STDMETHOD( ConfigChanged(const ConfigNode& node) = 0 );
	// 这一块逻辑还没完成
	STDMETHOD( LanguageChanged(CString language) = 0);
};

// 所有属性都已留在内存中，用时立取，不要长时间缓存子树，没有多少性能损失
interface IConfig : public IModule
{
public:
	STDMETHOD( LoadConfigFromXML(LPCTSTR filePath) = 0 );
	STDMETHOD( SaveConfigToXML(LPCTSTR filePath=NULL) = 0 );		// 默认存入打开时的文件
	// 传入前需要填好node中的path
	// 返回值:
	// E_FAIL	不存在该节点。
	// S_OK		顺利取到
	STDMETHOD( Get(ConfigNode& node) = 0 );
	// 返回值:
	// S_OK		修改成功
	// S_FALSE	原来不存在值，新添。
	STDMETHOD( Set(const ConfigNode& node) = 0 );
	// 添加监视。一旦该节点下任何节点被修改，将得到通知。
	STDMETHOD( AddMonitor(ConfigPath path,IConfigMonitor* pMonitor) = 0 );
	//! 析构前一定要删除，不然会崩溃。
	STDMETHOD( RemoveMonitor(IConfigMonitor* pMonitor) = 0 );
};
