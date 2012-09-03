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
	AttributeMap	attr;			// ���Ա�
	ConfigBranchs	branch;			// ����������

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

	// xml��·��
	ConfigPath		path;

	// �趨��path֮�����Get����ȡ��
	// ����ֶ�������path�������path�������Բ��
	BOOL			Get(LPCTSTR path=NULL);

	// �ڵ�����
	UINT			Count();
	// ��ȡ�ڵ㡣��ʱһ��path��Ӧ��count���ڵ㡣
	// ���жԸ����õ��޸Ķ��������õġ�
	// index������Χ��������һ���սڵ������
	ConfigNode&		GetNode(UINT index=0);
	ConfigNode&		operator[](UINT index);
	// ͨ�����Բ��ҵ�һ�����ϵĽڵ㡣
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
	// ��һ���߼���û���
	STDMETHOD( LanguageChanged(CString language) = 0);
};

// �������Զ��������ڴ��У���ʱ��ȡ����Ҫ��ʱ�仺��������û�ж���������ʧ
interface IConfig : public IModule
{
public:
	STDMETHOD( LoadConfigFromXML(LPCTSTR filePath) = 0 );
	STDMETHOD( SaveConfigToXML(LPCTSTR filePath=NULL) = 0 );		// Ĭ�ϴ����ʱ���ļ�

	//////////////////////////////////////////////////////////////////////////
	// ���������ӿڲ���Ҫֱ��ʹ�ã�CConfig�������˷�װ��
	// ����ֵ:
	// E_FAIL	�����ڸýڵ㡣
	// S_OK		˳��ȡ��
	STDMETHOD( Get(const ConfigPath& path,ConfigPtrVec& node) = 0 );

	// ��Ӽ��ӡ�һ���ýڵ����κνڵ㱻�޸ģ����õ�֪ͨ��
	STDMETHOD( AddMonitor(ConfigPath path,IConfigMonitor* pMonitor) = 0 );
	//! ����ǰһ��Ҫɾ������Ȼ�������
	STDMETHOD( RemoveMonitor(IConfigMonitor* pMonitor) = 0 );
	// ֪ͨ�����ı䡣��ݹ�֪ͨ������
};

//////////////////////////////////////////////////////////////////////////
//
