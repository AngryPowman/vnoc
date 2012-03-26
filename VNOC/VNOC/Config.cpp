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
		Global->Log(LogFile_Config,_T("文件加载成功。"));
		CLogPrefix p(LogFile_Config,_T("[XML解析]"));
		CLogIndent i(LogFile_Config);
		bRet = _ParseXML(m_doc.RootElement(),m_rootNode);
	}
	Global->Log(LogFile_Config,_T("XML加载工作结束"));
	return bRet? S_OK:S_FALSE;
}

HRESULT CConfig::SaveConfigToXML( LPCTSTR filePath )
{
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
				_ParseXML(curr->FirstChild(),treeRoot);
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

HRESULT CConfig::Get( ConfigNode* node )
{
	return S_OK;
}

HRESULT CConfig::Set( const ConfigNode& node )
{
	return S_OK;
}
