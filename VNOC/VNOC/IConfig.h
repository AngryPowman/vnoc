#pragma once 

#include "Config.h"

class IConfig
{
public:
	IConfig(LPCSTR strRoot);
	~IConfig();
public:
	// �����Ҫ�������ú��������������������֮
	virtual BOOL _ParseConfig(const TiXmlElement* root);

	// ��ȡ�ڵ㣬����֤�̰߳�ȫ��

	// ���ֻ�����ɲ���
	const TiXmlElement* _ReadConfig(LPCSTR rootName=NULL);

	// ��д
	//! �в�֧����������,��ȡ����Ȼ���ӽڵ㣬��Ҳ���������ڵ�
	TiXmlElement* _QueryConfig(LPCSTR rootName=NULL);

	// �ͷŽڵ�
	void _ReleaseConfig(BOOL writeToFileNow=FALSE);

private:
protected:
	std::string m_rootName;
};