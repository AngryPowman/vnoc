#include "StdAfx.h"
#include "Config.h"
#include "../util/util.h"

//////////////////////////////////////////////////////////////////////////
// CConfig

CConfig::CConfig(void)
{
}

CConfig::~CConfig(void)
{
}

HRESULT CConfig::Initialize( IModule* UpperFrame/*=NULL*/ )
{
	Global->Log(LogFile_Config,_T("��ʼ��"));
	return S_OK;
}

HRESULT CConfig::UnInitialize()
{
	Global->Log(LogFile_Config,_T("����ʼ��"));
	return S_OK;
}

HRESULT CConfig::Run()
{
	Global->Log(LogFile_Config,_T("����"));
	return S_OK;
}

HRESULT CConfig::Terminate()
{
	Global->Log(LogFile_Config,_T("��ֹ"));
	return S_OK;
}

HRESULT CConfig::LoadConfigFromXML( LPCTSTR filePath )
{
	Global->Log(LogFile_Config,_T("��ʼ����XML"));
	m_doc.Clear();
	BOOL bRet = m_doc.LoadFile(CStringA(filePath));		// �ӵ�����ֻ֧��UTF8
	assert(bRet && "����xml�ļ������쳣");
	if (bRet)
	{
		Global->Log(LogFile_Config,_T("�ļ����سɹ���"));
		CLogPrefix p(LogFile_Config,_T("[XML����]"));
		CLogIndent i(LogFile_Config);
		bRet = _ParseXML(m_doc.RootElement(),m_rootNode);
	}
	Global->Log(LogFile_Config,_T("XML���ع�������"));
	return bRet? S_OK:S_FALSE;
}

HRESULT CConfig::SaveConfigToXML( LPCTSTR filePath )
{
	return m_doc.SaveFile(CStringA(filePath));
}

// �����ڵ㣬֮����ӽڵ���_ParseXML����ݹ鴦��
BOOL CConfig::_ParseXMLTree(TiXmlNode *root,ConfigTreeNode& treeRoot)
{
	ATLASSERT(root);
	if (root)
	{
		_ParseNode(root,treeRoot.config);
		root = root->FirstChild();
		while(root)
		{
			_ParseXML(root,treeRoot);
			root = root->NextSibling();
		}
		Global->Log(LogFile_Config,_T("XML���������"));
		return TRUE;
	}
	return FALSE;
}

// ��root����������treeRoot.branch
BOOL CConfig::_ParseXML(TiXmlNode *root,ConfigTreeNode& treeRoot)
{
	CLogIndent li(LogFile_Config);
	TiXmlNode *curr = root;
	while(curr)
	{
		std::pair<ConfigMap::iterator,bool>	ret;
		ret = treeRoot.branch.insert(std::make_pair(CStringA(curr->Value()),ConfigTreeNode()));
		if (ret.second)
		{
			ConfigNode& node = ret.first->second.config;
			_ParseNode(curr,node);
			if (curr->FirstChild())
			{
				_ParseXML(curr->FirstChild(),treeRoot);
			}
		}
		curr = curr->NextSibling();
	}
	assert(root && "�ڵ�Ϊ��");
	return TRUE;
}

BOOL CConfig::_ParseNode( TiXmlNode *node,ConfigNode& cfg )
{
	CLogIndent i(LogFile_Config);
	cfg.strValue = node->Value();
	int type = node->Type();
	Global->Logf(LogFile_Config,_T("�������õ�(value = %s,type = %d)"),cfg.strValue.GetString(),type);
	switch(type)
	{
	case TiXmlNode::TINYXML_ELEMENT:
		TiXmlAttribute *attr = node->ToElement()->FirstAttribute();
		if (attr)
		{
			Global->Log(LogFile_Config,_T("(Attributes:"),FALSE,FALSE);
			while(attr)
			{
				Global->Lognf(LogFile_Config,_T("|%s:%s|"),CString(attr->Name()),CString(attr->Value()));
				cfg.attr.insert(std::make_pair(attr->Name(), attr->Value()));
				attr = attr->Next();
			}
			Global->Log(LogFile_Config,_T(")"),FALSE,FALSE);
		}
	}
	Global->Log(LogFile_Config,_T("\n"),FALSE,FALSE);
	return TRUE;
}

HRESULT CConfig::Get( ConfigNode* node )
{
	return S_OK;
}

HRESULT CConfig::Set( const ConfigNode& node )
{
	return S_OK;
}
