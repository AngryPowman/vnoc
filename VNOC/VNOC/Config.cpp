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
		m_filePath = filePath;
		Global->Log(LogFile_Config,_T("�ļ����سɹ���"));
		CLogPrefix p(LogFile_Config,_T("[XML����]"));
		CLogIndent i(LogFile_Config);
		bRet = _ParseXMLTree(m_doc.RootElement(),m_rootNode);
	}
	Global->Log(LogFile_Config,_T("XML���ع�������"));
	return bRet? S_OK:S_FALSE;
}

HRESULT CConfig::SaveConfigToXML( LPCTSTR filePath )
{
	if (filePath == NULL)
	{
		filePath = m_filePath;
	}
	if (!m_doc.RootElement())
	{
		TiXmlElement rootElement("root");	// ���ֵ����Ҫ����д���������дһ��
		m_doc.InsertEndChild(rootElement);
	}
	m_doc.RootElement()->Clear();
	_CreateXMLTree(*m_doc.RootElement(),m_rootNode);
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
				_ParseXML(curr->FirstChild(),ret.first->second);
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

HRESULT CConfig::Get( ConfigNode& node )
{
	Util::CAutoCS ac(m_cs);
	CLogPrefix	lp(LogFile_Config,_T("[Config]"));
	ConfigNode *pNode = _Find(node.path);
	if (pNode)
	{
		Global->Logf(LogFile_Config,_T("ȡֵ�ɹ�(%s)"),pNode->strValue);
		node = *pNode;
		return S_OK;
	}
	else
	{
		Global->Log(LogFile_Config,_T("ȡ����ʧ�ܣ������ڽڵ�"));
		return E_FAIL;
	}
}

HRESULT CConfig::Set( const ConfigNode& node )
{
	Util::CAutoCS ac(m_cs);
	CLogPrefix	lp(LogFile_Config,_T("[Config]"));
	ConfigNode *pNode = _Find(node.path,TRUE);
	if (pNode)
	{
		pNode->attr = node.attr;
		pNode->strValue = node.strValue;
		Global->Logf(LogFile_Config,_T("���óɹ�(%s)"),pNode->strValue);
		return S_OK;
	}
	return E_FAIL;
}

ConfigNode* CConfig::_Find( ConfigPath path,BOOL createIfNotExist)
{
	ConfigPath::iterator i;
	ConfigMap::iterator mi;
	i = path.begin();
	ConfigTreeNode *pos=&m_rootNode;
	while (i != path.end())
	{
		mi = pos->branch.find(*i);
		if (mi == pos->branch.end())
		{
			if (!createIfNotExist)
			{
				return NULL;
			}
			pos->branch.insert(std::make_pair(*i,ConfigTreeNode()));
		}
		else
		{
			pos = &(mi->second);
			ASSERT(pos);
		}
		++i;
	}
	ASSERT(pos);	// �����ϵ�����Ӧ����������NULL
	return pos? &pos->config: NULL;
}

BOOL CConfig::_CreateXMLTree(TiXmlNode& tree,const ConfigTreeNode& root )
{
	CLogPrefix lp(LogFile_Config,_T("CreateTree"));
	TiXmlElement* element = tree.ToElement();
	while (element)
	{
		element->SetValue(CStringA(root.config.strValue));
		AttributeMap::const_iterator i;
		for (i=root.config.attr.begin(); i!=root.config.attr.end(); ++i)
		{
			element->SetAttribute(CStringA(i->first),CStringA(i->second));
		}
		ConfigMap::const_iterator mi;
		for (mi=root.branch.begin(); mi!=root.branch.end(); ++mi)
		{
			TiXmlNode* child = element->FirstChild(CStringA(mi->first));
			if (!child)
			{	// �¼���Ľڵ�
				TiXmlNode *newChild = NULL;
				if (mi->second.branch.empty() && mi->second.config.attr.empty())
				{	// �½ڵ�û���ӽڵ㣬û�����ԣ���Ϊһ��Text�ڵ㡣
					newChild = new TiXmlText(CStringA(mi->first));
				}
				else
				{
					newChild = new TiXmlElement(CStringA(mi->first));
				}
				ASSERT(newChild);
				if (newChild)
				{
					element->InsertEndChild(*newChild);
					delete newChild;
					child = element->FirstChild(CStringA(mi->first));
				}
			}
			if (child)
			{
				_CreateXMLTree(*child,mi->second);
			}
		}
		element = element->NextSiblingElement();
	}
	return TRUE;
}

HRESULT CConfig::AddMonitor( ConfigPath path,IConfigMonitor* pMonitor )
{
	return S_OK;
}

HRESULT CConfig::RemoveMonitor( IConfigMonitor* pMonitor )
{
	return S_OK;
}
