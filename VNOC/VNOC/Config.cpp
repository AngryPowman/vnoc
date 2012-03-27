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
	Global->Log(LogFile_Config,_T("初始化"));
	return S_OK;
}

HRESULT CConfig::UnInitialize()
{
	Global->Log(LogFile_Config,_T("反初始化"));
	return S_OK;
}

HRESULT CConfig::Run()
{
	Global->Log(LogFile_Config,_T("启动"));
	return S_OK;
}

HRESULT CConfig::Terminate()
{
	Global->Log(LogFile_Config,_T("中止"));
	return S_OK;
}

HRESULT CConfig::LoadConfigFromXML( LPCTSTR filePath )
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

HRESULT CConfig::SaveConfigToXML( LPCTSTR filePath )
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
		Global->Log(LogFile_Config,_T("XML树解析完成"));
		return TRUE;
	}
	return FALSE;
}

// 将root分析后填入treeRoot.branch
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
	assert(root && "节点为空");
	return TRUE;
}

BOOL CConfig::_ParseNode( TiXmlNode *node,ConfigNode& cfg )
{
	CLogIndent i(LogFile_Config);
	cfg.strValue = node->Value();
	int type = node->Type();
	Global->Logf(LogFile_Config,_T("分析配置点(value = %s,type = %d)"),cfg.strValue.GetString(),type);
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
		Global->Logf(LogFile_Config,_T("取值成功(%s)"),pNode->strValue);
		node = *pNode;
		return S_OK;
	}
	else
	{
		Global->Log(LogFile_Config,_T("取配置失败，不存在节点"));
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
		Global->Logf(LogFile_Config,_T("设置成功(%s)"),pNode->strValue);
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
	ASSERT(pos);	// 理论上到这里应当不可能是NULL
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
			{	// 新加入的节点
				TiXmlNode *newChild = NULL;
				if (mi->second.branch.empty() && mi->second.config.attr.empty())
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
