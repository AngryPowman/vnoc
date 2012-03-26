#pragma once
#include "IModule.h"
#include <vector>
#include <map>

typedef std::map<CString,CString>	AttributeMap;
typedef std::vector<CString>		ConfigPath;

// �洢�����Ե�������Ϣ
struct ConfigNode
{
	CString		strValue;		// ֵ
	AttributeMap attr;			// ���Ա�
};

// �������Զ��������ڴ��У���ʱ��ȡ����Ҫ���棬û�ж���������ʧ
interface IConfig : public IModule
{
public:
	STDMETHOD( LoadConfigFromXML(LPCTSTR filePath) = 0 );
	STDMETHOD( SaveConfigToXML(LPCTSTR filePath) = 0 );
	// ����Getһ���ɹ������xml��ɾ�������ʧ�ܣ������Ĭ������
	STDMETHOD( Get(ConfigNode* node) = 0 );
	// ����ֵ:
	// S_OK		�޸ĳɹ�
	// S_FALSE	ԭ��������ֵ������
	STDMETHOD( Set(const ConfigNode& node) = 0 );
};
