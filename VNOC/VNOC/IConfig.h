#pragma once 

#include "Config.h"

class IConfig
{
public:
	IConfig(LPCSTR strRoot);
	~IConfig();
public:
	// 如果需要加载配置后做特殊操作，可以重载之
	virtual BOOL _ParseConfig(const TiXmlElement* root);

	// 获取节点，并保证线程安全。

	// 如果只读，可并发
	const TiXmlElement* _ReadConfig(LPCSTR rootName=NULL);

	// 读写
	//! 尚不支持子树锁定,获取的虽然是子节点，但也会锁定根节点
	TiXmlElement* _QueryConfig(LPCSTR rootName=NULL);

	// 释放节点
	void _ReleaseConfig(BOOL writeToFileNow=FALSE);

private:
protected:
	std::string m_rootName;
};