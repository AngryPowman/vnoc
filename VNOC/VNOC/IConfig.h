#pragma once
#include "IModule.h"
#include <vector>
#include <map>

typedef std::map<CString,CString>	AttributeMap;
typedef std::vector<CString>		ConfigPath;

// �洢�����Ե�������Ϣ
struct ConfigNode
{
	ConfigPath	path;
	CString		strValue;		// ֵ
	AttributeMap attr;			// ���Ա�
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
};
