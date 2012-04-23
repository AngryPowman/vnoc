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
				{	//防止连着写了多个'/'符号
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
	Global->Log(LogFile_Config,_T("初始化"));
	return S_OK;
}

HRESULT CConfigImpl::UnInitialize()
{
	Global->Log(LogFile_Config,_T("反初始化"));
	return S_OK;
}

HRESULT CConfigImpl::Run()
{
	Global->Log(LogFile_Config,_T("Config模块启动"));
	return S_OK;
}

HRESULT CConfigImpl::Terminate()
{
	Global->Log(LogFile_Config,_T("Config模块中止"));
	return S_OK;
}

HRESULT CConfigImpl::LoadConfigFromXML( LPCTSTR filePath )
{
	Global->Log(LogFile_Config,_T("开始加载XML"));
	m_doc.Clear();
	BOOL bRet = m_doc.LoadFile(CStringA(filePath));		// 坑爹啊！只支持UTF8
	assert(bRet && "配置xml文件加载异常");
	if (bRet)
	{
		m_filePath = filePath;
		Global->Log(LogFile_Config,_T("文件加载成功。"));
		CLogPrefix p(LogFile_Config,_T("[XML解析]"));
		CLogIndent i(LogFile_Config);
		bRet = _ParseXMLTree(m_doc.RootElement(),m_rootNode);
	}
	Global->Log(LogFile_Config,_T("XML加载工作结束"));
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
		TiXmlElement rootElement("root");	// 这个值反正要被改写，这里随便写一个
		m_doc.InsertEndChild(rootElement);
	}
	m_doc.RootElement()->Clear();
	_CreateXMLTree(*m_doc.RootElement(),m_rootNode);
	return m_doc.SaveFile(CStringA(filePath));
}

// 填充根节点，之后的子节点由_ParseXML负责递归处理
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
		Global->Log(LogFile_Config,_T("XML树解析完成"));
		return TRUE;
	}
	return FALSE;
}

// 将root分析后填入treeRoot.branch
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
	assert(root && "节点为空");
	return TRUE;
}

BOOL CConfigImpl::_ParseNode( TiXmlNode *node,ConfigNode& cfg )
{
	CLogIndent i(LogFile_Config);

	int type = node->Type();
	Global->Logf(LogFile_Config,_T("分析配置点(value = %s,type = %d)"),CString(node->ValueStr().c_str()),type);
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
		Global->Logf(LogFile_Config,_T("取值成功(%s)"),pNode->value);
		node.push_back(pNode);
		return S_OK;
	}
	else
	{
		Global->Log(LogFile_Config,_T("取配置失败，不存在节点"));
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
	ASSERT(pos);	// 理论上到这里应当不可能是NULL
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
			{	// 新加入的节点
				TiXmlNode *newChild = NULL;
				Global->PtrAssert(mi->second);
				if (mi->second->branch.empty() && mi->second->attr.empty())
				{	// 新节点没有子节点，没有属性，作为一个Text节点。
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
