#include "StdAfx.h"
#include "Config.h"
#include "../util/util.h"

//////////////////////////////////////////////////////////////////////////
// CConfig

void CConfig::xPath(LPCTSTR cfgPath)
{
	CString tempStr = cfgPath;
	LPTSTR thePath = tempStr.GetBuffer(tempStr.GetLength());
	ASSERT(thePath);
	if (thePath)
	{
		LPCTSTR currPos = thePath;
		LPCTSTR startPos = thePath;
		do 
		{
			if (*thePath == ConfigPathSplit)
			{
				*thePath = _T('\0');
				path.push_back(startPos);
				++thePath;
				while(*thePath == ConfigPathSplit)
				{	//��ֹ����д�˶��'/'����
					++thePath;
				}
				startPos = thePath;
			}
			else if(*thePath == _T('\0'))
			{
				path.push_back(startPos);
				startPos = thePath;
			}
			else
			{
				++thePath;
			}
		} while (*startPos);
	}
	tempStr.ReleaseBuffer(0);;
}

CConfig::CConfig()
{}

CConfig::CConfig(LPCTSTR path)
{
	xPath(path);
}

CConfig::~CConfig()
{}

BOOL CConfig::Get(LPCTSTR path)
{
	if (path)
	{
		xPath(path);
	}
	IConfig *pConf=NULL;
	Global->GetIConfig(&pConf);
	if (pConf)
	{
		HRESULT hr = pConf->Get(this->path,m_vec);
		return SUCCEEDED(hr);
	}
	return FALSE;
}

UINT CConfig::Count()
{
	return m_vec.size();
}

ConfigNode& CConfig::GetNode(UINT index)
{
	ASSERT(index < m_vec.size());
	if (index < m_vec.size())
	{
		return *m_vec[index];
	}
	return emptyNode;
}

ConfigNode& CConfig::operator [](UINT index)
{
	return GetNode(index);
}

BOOL ConfigNode::GetAttribute(LPCTSTR key,CString& value)
{
	value.Empty();
	AttributeMap::iterator i;
	i = attr.find(key);
	if (i != attr.end())
	{
		value = i->second;
		return TRUE;
	}
	return FALSE;
}

BOOL ConfigNode::GetAttribute(LPCTSTR key,int& value)
{
	CString attrValue;
	if (GetAttribute(key,attrValue))
	{
		value = _ttoi(attrValue);
		return TRUE;
	}
	return FALSE;
}

BOOL ConfigNode::GetAttribute(LPCTSTR key,double& value)
{
	CString attrValue;
	if (GetAttribute(key,attrValue))
	{
		value = _ttof(attrValue);
		return TRUE;
	}
	return FALSE;
}

VOID ConfigNode::SetAttribute(LPCTSTR key,LPCTSTR value)
{
	ATLASSERT(value);
	if (value)
	{
		attr[key] = value;
	}
}

VOID ConfigNode::SetAttribute(LPCTSTR key,int value)
{
	CString str;
	str.Format(_T("%d"),value);
	SetAttribute(key,str);
}

VOID ConfigNode::SetAttribute(LPCTSTR key,double value)
{
	CString str;
	str.Format(_T("%lf"),value);
	SetAttribute(key,str);
}

VOID ConfigNode::Lock()
{
	m_cs.Enter();
}

VOID ConfigNode::UnLock()
{
	m_cs.Leave();
}

//////////////////////////////////////////////////////////////////////////
// CConfigImpl

CConfigImpl::CConfigImpl(void)
{
}

CConfigImpl::~CConfigImpl(void)
{
}

HRESULT CConfigImpl::Initialize( IModule* UpperFrame/*=NULL*/ )
{
	Global->Log(LogFile_Config,_T("��ʼ��"));
	return S_OK;
}

HRESULT CConfigImpl::UnInitialize()
{
	Global->Log(LogFile_Config,_T("����ʼ��"));
	return S_OK;
}

HRESULT CConfigImpl::Run()
{
	Global->Log(LogFile_Config,_T("Configģ������"));
	return S_OK;
}

HRESULT CConfigImpl::Terminate()
{
	Global->Log(LogFile_Config,_T("Configģ����ֹ"));
	return S_OK;
}

HRESULT CConfigImpl::LoadConfigFromXML( LPCTSTR filePath )
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

HRESULT CConfigImpl::SaveConfigToXML( LPCTSTR filePath )
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
BOOL CConfigImpl::_ParseXMLTree(TiXmlNode *root,ConfigNode& treeRoot)
{
	ATLASSERT(root);
	if (root)
	{
		_ParseNode(root,treeRoot);
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
BOOL CConfigImpl::_ParseXML(TiXmlNode *root,ConfigNode& treeRoot)
{
	CLogIndent li(LogFile_Config);
	TiXmlNode *curr = root;
	while(curr)
	{
		std::pair<ConfigBranchs::iterator,bool>	ret;
		ConfigNode *pNewNode = new ConfigNode;
		Global->PtrAssert(pNewNode);
		ret = treeRoot.branch.insert(std::make_pair(CStringA(curr->Value()),pNewNode));
		if (ret.second)
		{
			ConfigNode& node = *ret.first->second;
			_ParseNode(curr,node);
			if (curr->FirstChild())
			{
				_ParseXML(curr->FirstChild(),*ret.first->second);
			}
		}
		curr = curr->NextSibling();
	}
	assert(root && "�ڵ�Ϊ��");
	return TRUE;
}

BOOL CConfigImpl::_ParseNode( TiXmlNode *node,ConfigNode& cfg )
{
	CLogIndent i(LogFile_Config);

	int type = node->Type();
	Global->Logf(LogFile_Config,_T("�������õ�(value = %s,type = %d)"),CString(node->ValueStr().c_str()),type);
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

HRESULT CConfigImpl::Get(const ConfigPath& path,ConfigPtrVec& node )
{
	Util::CAutoCS ac(m_cs);
	CLogPrefix	lp(LogFile_Config,_T("[Config]"));
	ConfigNode *pNode = _Find(path);
	if (pNode)
	{
		Global->Logf(LogFile_Config,_T("ȡֵ�ɹ�(%s)"),pNode->value);
		node.push_back(pNode);
		return S_OK;
	}
	else
	{
		Global->Log(LogFile_Config,_T("ȡ����ʧ�ܣ������ڽڵ�"));
		return E_FAIL;
	}
}

ConfigNode* CConfigImpl::_Find( ConfigPath path,BOOL createIfNotExist)
{
	ConfigPath::iterator i;
	ConfigBranchs::iterator mi;
	i = path.begin();
	ConfigNode *pos=&m_rootNode;
	while (i != path.end())
	{
		mi = pos->branch.find(*i);
		if (mi == pos->branch.end())
		{
			if (!createIfNotExist)
			{
				return NULL;
			}
			ConfigNode* pNewNode = new ConfigNode;
			Global->PtrAssert(pNewNode);
			pos->branch.insert(std::make_pair(*i,pNewNode));
		}
		else
		{
			pos = mi->second;
			ASSERT(pos);
		}
		++i;
	}
	ASSERT(pos);	// �����ϵ�����Ӧ����������NULL
	return pos;
}

BOOL CConfigImpl::_CreateXMLTree(TiXmlNode& tree,const ConfigNode& root )
{
	CLogPrefix lp(LogFile_Config,_T("CreateTree"));
	TiXmlElement* element = tree.ToElement();
	while (element)
	{
		element->SetValue(CStringA(root.value));
		AttributeMap::const_iterator i;
		for (i=root.attr.begin(); i!=root.attr.end(); ++i)
		{
			element->SetAttribute(CStringA(i->first),CStringA(i->second));
		}
		ConfigBranchs::const_iterator mi;
		for (mi=root.branch.begin(); mi!=root.branch.end(); ++mi)
		{
			TiXmlNode* child = element->FirstChild(CStringA(mi->first));
			if (!child)
			{	// �¼���Ľڵ�
				TiXmlNode *newChild = NULL;
				Global->PtrAssert(mi->second);
				if (mi->second->branch.empty() && mi->second->attr.empty())
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
				_CreateXMLTree(*child,*mi->second);
			}
		}
		element = element->NextSiblingElement();
	}
	return TRUE;
}

HRESULT CConfigImpl::AddMonitor( ConfigPath path,IConfigMonitor* pMonitor )
{
	return S_OK;
}

HRESULT CConfigImpl::RemoveMonitor( IConfigMonitor* pMonitor )
{
	return S_OK;
}
