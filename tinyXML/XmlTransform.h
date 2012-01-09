/*
////////////////////////////////////////////////////////////////////////
文件名：XmlTransform.h
创建日期：2011年09月28日
最后更新：xxxx年xx月xx日
编写者：Sum
功能描述：
    负责XML的解析和生成
 
版权所有：VNOC团队
///////////////////////////////////////////////////////////////////////
*/
#ifndef XML_TRANSFORM
#define XML_TRANSFORM

#include <tchar.h>

typedef unsigned int  uint;

class XmlTransform
{
public:

	XmlTransform(){};
	XmlTransform(const TCHAR* lpszStr);
	virtual ~XmlTransform(){};
	
	bool     Analysis(const TCHAR* lpszStr);
	void     Flag(const TCHAR* lpszFlag); 
	TCHAR*   GetAttribute(const TCHAR* lpszAttribute);
	void     SetAttribute(const TCHAR* lpszAttribute);
	bool	 GetText();

private:

	void* NewList(uint Index);

private:
	
	TCHAR** m_List;
	int		m_LineMax;
	TCHAR*  m_Flag;
};

XmlTransform::XmlTransform(const TCHAR* lpszStr)
{
	if (lpszStr)
	{
		this->Analysis(lpszStr);
	}
}

void* XmlTransform::NewList(uint Index)
{
	if (Index == 0)
	{
		return 0;
	}

	TCHAR** lpsztmp = NULL;
	if (m_List != 0)
	{
		_asm
		{
			mov lpsztmp,m_List[type int * 1]
		}
	}
	else
	{
		m_LineMax = Index;
		m_List = new TCHAR*[Index];
		memset(m_List,0,sizeof(m_List));
	}
	return 0;
}

bool XmlTransform::Analysis(const TCHAR* lpszStr)
{
	if (!lpszStr)
	{
		return false;
	}
	int Sign = 0;       // 记录<>的中匹配的长度
	bool Shut = false;  // 记录是否是<xxx />闭合
	for (int len = 0; (unsigned int)len < _tcslen(lpszStr); len++)
	{
		if (lpszStr[len] == '<')
		{
			for (Sign = len;(unsigned int)Sign < _tcslen(lpszStr);Sign++)
			{
				lpszStr[Sign];
				if (lpszStr[Sign] == '>')
				{
					
				}
			}
		}
	}
	return true;

}

void XmlTransform::Flag(const TCHAR* lpszFlag)
{

}

#endif