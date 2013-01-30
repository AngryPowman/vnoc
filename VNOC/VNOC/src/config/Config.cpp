#include "StdAfx.h"
#include "Config.h"

//////////////////////////////////////////////////////////////////////////
// ConfigNode
ConfigNode::ConfigNode()
{
}

ConfigNode::~ConfigNode()
{
// 	attr.clear();
// 	ConfigBranchs::iterator i;
// 	for (i=branch.begin(); i!=branch.end(); ++i)
// 	{
// 		ConfigVec::iterator j;
// 		for (j=i->second.begin(); j!=i->second.end(); ++j)
// 		{
// 			delete &(*j);
// 		}
// 	}
}

ConfigNode& CConfig::GetNode(UINT index)
{
	ATLASSERT(index < m_vec.size());
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

BOOL ConfigNode::GetAttribute(LPCTSTR key,UINT& value)
{
	CString attrValue;
	if (GetAttribute(key,attrValue))
	{
		value = (UINT)_ttoi(attrValue);
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

VOID ConfigNode::SetAttribute(LPCTSTR key,UINT value)
{
	SetAttribute(key,(int)value);
}

VOID ConfigNode::SetAttribute(LPCTSTR key,double value)
{
	CString str;
	str.Format(_T("%lf"),value);
	SetAttribute(key,str);
}

//////////////////////////////////////////////////////////////////////////
// CConfig

void CConfig::xPath(LPCTSTR cfgPath)
{
	CString tempStr = cfgPath;
	LPTSTR thePath = tempStr.GetBuffer(tempStr.GetLength());
	ATLASSERT(thePath);
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
{
}

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
	_Clear();
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
	TiXmlDocument doc;
	doc.Clear();
	BOOL bRet = doc.LoadFile(CStringA(filePath));		// 坑爹啊！只支持UTF8
	ATLASSERT(bRet && "配置xml文件加载异常");
	if (bRet)
	{
		m_filePath = filePath;
		Global->Log(LogFile_Config,_T("文件加载成功。"));
		CLogPrefix p(LogFile_Config,_T("[XML解析]"));
		CLogIndent i(LogFile_Config);
		bRet = _ParseXML(doc.RootElement(),m_rootNode);
	}
	Global->Log(LogFile_Config,_T("XML加载工作结束"));
	return bRet? S_OK:S_FALSE;
}

// 将root分析后填入treeRoot.branch
BOOL CConfigImpl::_ParseXML(TiXmlNode *root,ConfigNode& treeRoot)
{
	CLogIndent li(LogFile_Config);
	TiXmlNode *curr = root;

	while(curr)
	{
		ConfigVec& i = treeRoot.branch[CString(curr->Value())];

		ConfigNode newNode;

		_ParseXMLNode(curr,newNode);
		if (curr->FirstChild())
		{
			_ParseXML(curr->FirstChild(),newNode);
		}
		i.push_back(newNode);

		curr = curr->NextSibling();
	}
	ATLASSERT(root && "节点为空");
	return TRUE;
}

BOOL CConfigImpl::_ParseXMLNode( TiXmlNode *tiNode,ConfigNode& node )
{
	CLogIndent i(LogFile_Config);

	int type = tiNode->Type();
	Global->Logf(LogFile_Config,_T("分析配置点(value = %s,type = %d)"),CString(tiNode->ValueTStr().c_str()),type);
	node.value = tiNode->ValueTStr().c_str();
	switch(type)
	{
	case TiXmlNode::ELEMENT:
		TiXmlAttribute *attr = tiNode->ToElement()->FirstAttribute();
		if (attr)
		{
			Global->Log(LogFile_Config,_T("(Attributes:"),FALSE,FALSE);
			while(attr)
			{
				Global->Lognf(LogFile_Config,_T("|%s:%s|"),CString(attr->Name()),CString(attr->Value()));
				node.attr.insert(std::make_pair(CString(attr->Name()), CString(attr->Value())));
				attr = attr->Next();
			}
			Global->Log(LogFile_Config,_T(")"),FALSE,FALSE);
		}
	}
	Global->Log(LogFile_Config,_T("\n"),FALSE,FALSE);
	return TRUE;
}

HRESULT CConfigImpl::SaveConfigToXML( LPCTSTR filePath )
{
	if (filePath == NULL)
	{
		filePath = m_filePath;
	}
	TiXmlDocument doc;
	if (!doc.RootElement())
	{
		TiXmlElement rootElement("root");	// 这个值反正要被改写，这里随便写一个
		doc.InsertEndChild(rootElement);
	}
	doc.RootElement()->Clear();
	Global->Log(LogFile_Config,_T("创建XML树."));
	BOOL bRet = _CreateXML(doc,m_rootNode);
	Global->Logf(LogFile_Config,_T("XML树创建%s.\n"),bRet? _T("成功"): _T("失败"));
	if (bRet)
	{
		bool bRet = doc.SaveFile(CStringA(filePath));
	}
	return bRet? S_OK: E_FAIL;
}

BOOL CConfigImpl::_CreateXML(TiXmlNode& tree,const ConfigNode& root )
{
	CLogPrefix lp(LogFile_Config,_T("CreateTree"));

	ConfigBranchs::const_iterator i;
	for (i=root.branch.begin(); i!=root.branch.end(); ++i)
	{
		ConfigVec::const_iterator j;
		for (j=i->second.begin(); j!=i->second.end(); ++j)
		{
			TiXmlNode* pNode=NULL;
			BOOL bRet = _CreateXMLNode(&pNode,*j);
			_CreateXML(*pNode,*j);
			if (bRet && pNode)
			{
				tree.InsertEndChild(*pNode);
				delete pNode;
			}
		}
	}
	return TRUE;
}

BOOL CConfigImpl::_CreateXMLNode(TiXmlNode **node,const ConfigNode& root)
{
	if (!node)
	{
		return FALSE;
	}
	ATLASSERT(!*node);
	ATLASSERT(!root.value.IsEmpty());
	if (!root.value.IsEmpty())
	{
		if (root.branch.empty() && root.attr.empty())
		{
			*node = new TiXmlText(CStringA(root.value));
			Global->PtrAssert(*node);
		}
		else
		{
			*node = new TiXmlElement(CStringA(root.value));
			Global->PtrAssert(*node);
			TiXmlElement* element = (*node)->ToElement();

			AttributeMap::const_iterator i;
			for (i=root.attr.begin(); i!=root.attr.end(); ++i)
			{
				element->SetAttribute(CStringA(i->first),CStringA(i->second));
			}
		}
		return TRUE;
	}
	return FALSE;
}

HRESULT CConfigImpl::Get(const ConfigPath& path,ConfigPtrVec& node )
{
	Util::CAutoCS ac(m_cs);
	CLogPrefix	lp(LogFile_Config,_T("[Config]"));
	BOOL bRet = _Find(path,node);
	if (bRet)
	{
		Global->Logf(LogFile_Config,_T("取值成功."));
		return S_OK;
	}
	else
	{
		Global->Log(LogFile_Config,_T("取配置失败，不存在节点"));
		return E_FAIL;
	}
}

BOOL CConfigImpl::_Find( ConfigPath path,ConfigPtrVec& vec,BOOL createIfNotExist/*=FALSE*/ )
{
	if (path.empty())
	{
		return FALSE;
	}
	ConfigPath::iterator i;
	ConfigNode *pCurr = &m_rootNode;

	ConfigBranchs::iterator j;
	for(i=path.begin(); i!=path.end(); ++i)
	{
		j = pCurr->branch.find(*i);
		if (j == pCurr->branch.end())
		{
			if (createIfNotExist)
			{
				ConfigNode node;
				node.value = *i;
				pCurr->branch[*i].push_back(node);
			}
			else
			{
				return FALSE;
			}
		}
		pCurr = &((j->second)[0]);
	}
	ConfigVec::iterator iVec;
	for(iVec=j->second.begin(); iVec!=j->second.end(); ++iVec)
	{
		vec.push_back(&(*iVec));
	}
	return TRUE;
}

VOID CConfigImpl::_Clear()
{
	m_rootNode.branch.clear();
}

HRESULT CConfigImpl::AddMonitor( ConfigPath path,IConfigMonitor* pMonitor )
{
	return S_OK;
}

HRESULT CConfigImpl::RemoveMonitor( IConfigMonitor* pMonitor )
{
	return S_OK;
}
