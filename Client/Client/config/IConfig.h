#pragma once
#include "IModule.h"
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
	virtual ResultCode ConfigChanged(const ConfigNode& node)=0;
	// 这一块逻辑还没完成
	virtual ResultCode LanguageChanged(CString language)=0;
};

// 所有属性都已留在内存中，用时立取，不要长时间缓存子树，没有多少性能损失
interface IConfig : public IModule
{
public:
	virtual ResultCode LoadConfigFromXML(LPCTSTR filePath)=0;
	virtual ResultCode SaveConfigToXML(LPCTSTR filePath=NULL)=0;

	// 不需要直接使用，CConfig已做好了封装。
	virtual ResultCode Get(const ConfigPath& path,ConfigPtrVec& node)=0;

	virtual ResultCode AddMonitor(ConfigPath path,IConfigMonitor* pMonitor)=0;
	virtual ResultCode RemoveMonitor(IConfigMonitor* pMonitor)=0;
};

//////////////////////////////////////////////////////////////////////////
//
