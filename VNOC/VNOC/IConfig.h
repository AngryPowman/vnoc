#pragma once
#include "IModule.h"
#include <vector>
#include <map>

typedef std::map<CString,CString>	AttributeMap;
typedef std::vector<CString>		ConfigPath;

// �洢�����Ե�������Ϣ
struct ConfigNode
{
	ConfigPath	path;			// ·��
	void xPath(LPCTSTR path);	// ��������"VNOC/Language/UI"�������ַ���������������
	CString		strValue;		// ֵ
	AttributeMap attr;			// ���Ա�
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
	// ����ǰ��Ҫ���node�е�path
	// ����ֵ:
	// E_FAIL	�����ڸýڵ㡣
	// S_OK		˳��ȡ��
	STDMETHOD( Get(ConfigNode& node) = 0 );
	// ����ֵ:
	// S_OK		�޸ĳɹ�
	// S_FALSE	ԭ��������ֵ������
	STDMETHOD( Set(const ConfigNode& node) = 0 );
	// ��Ӽ��ӡ�һ���ýڵ����κνڵ㱻�޸ģ����õ�֪ͨ��
	STDMETHOD( AddMonitor(ConfigPath path,IConfigMonitor* pMonitor) = 0 );
	//! ����ǰһ��Ҫɾ������Ȼ�������
	STDMETHOD( RemoveMonitor(IConfigMonitor* pMonitor) = 0 );
};
