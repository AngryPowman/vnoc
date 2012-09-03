#pragma once
#include "../base/IModule.h"
#include "ConfigDefine.h"

class ConfigNode;

typedef std::vector<ConfigNode>			ConfigVec;
typedef std::vector<ConfigNode*>		ConfigPtrVec;
typedef std::map<CString,ConfigVec>		ConfigBranchs;

class ConfigNode
{
public:
	CString			value;
	AttributeMap	attr;			// 属性表
	ConfigBranchs	branch;			// 其下面子树

	virtual BOOL	GetAttribute(LPCTSTR key,CString& value);
	virtual BOOL	GetAttribute(LPCTSTR key,int& value);
	virtual BOOL	GetAttribute(LPCTSTR key,UINT& value);
	virtual BOOL	GetAttribute(LPCTSTR key,double& value);
	virtual VOID	SetAttribute(LPCTSTR key,LPCTSTR value);
	virtual VOID	SetAttribute(LPCTSTR key,int value);
	virtual VOID	SetAttribute(LPCTSTR key,UINT value);
	virtual VOID	SetAttribute(LPCTSTR key,double value);

	ConfigNode();
	~ConfigNode();
};

class CConfig
{
public:
	CConfig();
	CConfig(LPCTSTR path);
	~CConfig();

	// xml树路径
	ConfigPath		path;

	// 设定好path之后调用Get来获取。
	// 如果手动设置了path，这里的path参数可以不填。
	BOOL			Get(LPCTSTR path=NULL);

	// 节点数量
	UINT			Count();
	// 获取节点。有时一个path对应着count个节点。
	// 所有对该引用的修改都将是永久的。
	// index超过范围，将返回一个空节点的引用
	ConfigNode&		GetNode(UINT index=0);
	ConfigNode&		operator[](UINT index);
	// 通过属性查找第一个符合的节点。
	ConfigNode&		GetNodeByAttr(LPCTSTR key,LPCTSTR value);
	ConfigNode&		GetNodeByAttr(LPCTSTR key,int value);
	ConfigNode&		GetNodeByAttr(LPCTSTR key,double value);
private:
	void	xPath(LPCTSTR path);
	ConfigPtrVec	m_vec;
	ConfigNode		emptyNode;
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

	//////////////////////////////////////////////////////////////////////////
	// 以下两个接口不需要直接使用，CConfig已做好了封装。
	// 返回值:
	// E_FAIL	不存在该节点。
	// S_OK		顺利取到
	STDMETHOD( Get(const ConfigPath& path,ConfigPtrVec& node) = 0 );

	// 添加监视。一旦该节点下任何节点被修改，将得到通知。
	STDMETHOD( AddMonitor(ConfigPath path,IConfigMonitor* pMonitor) = 0 );
	//! 析构前一定要删除，不然会崩溃。
	STDMETHOD( RemoveMonitor(IConfigMonitor* pMonitor) = 0 );
	// 通知发生改变。会递归通知子树。
};

//////////////////////////////////////////////////////////////////////////
//
