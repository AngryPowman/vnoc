/*
////////////////////////////////////////////////////////////////////////
�ļ�����XmlTransform.h
�������ڣ�2011��09��28��
�����£�xxxx��xx��xx��
��д�ߣ�Sum
����������
    ����XML�Ľ���������
 
��Ȩ���У�VNOC�Ŷ�
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
	int Sign = 0;       // ��¼<>����ƥ��ĳ���
	bool Shut = false;  // ��¼�Ƿ���<xxx />�պ�
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