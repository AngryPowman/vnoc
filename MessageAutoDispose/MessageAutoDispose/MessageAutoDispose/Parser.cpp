
#pragma  once 

#include "stdafx.h"
#include "Parser.h"
#include <fstream>
#include <string>
#include <AtlBase.h>
using namespace std;

int MessageAutoDispose::Open(TCHAR* Path)
{
	int Command = 0;
	ifstream  file;
	string line;
	if (Path == NULL)
	{
		return -1;
	}
	file.open(Path);
	if (!file){
		return -1;
	}
	for (;getline(file,line); m_LineMax++)
	{
		CString ctemporary=CA2CT(line.c_str(), CP_ACP);
		//MSGTYPE
		if ((ctemporary.Find(_T("enum")) != -1) && (ctemporary.Find(_T("MSGTYPE")) != -1))
		{
			m_MSGTYPE = m_LineMax;
		}
		//MSGCONMMAND
		if ((ctemporary.Find(_T("enum")) != -1) && (ctemporary.Find(_T("MSGCONMMAND")) != -1))
		{
			m_MSGCONMMAND = m_LineMax;
		}
		if ((ctemporary.Find(_T("class")) != -1) && (ctemporary.Find(_T("public CMessage")) != -1))
		{
			m_MsgConst.push_back(m_LineMax);
			m_MsgNote.push_back(m_LineMax - 1);
		}
		m_ListIndex.push_back(m_LineMax);
		m_ListString.push_back(ctemporary);
	}
	_Analysis();
	return 0;
}

void MessageAutoDispose::_Analysis()
{
	CString ctmp;
	//MSGTYPE
	ctmp = m_ListString[m_MSGTYPE];
	if (ctmp.Find(_T("{")) == -1)
	{
		ctmp.Empty();
		ctmp.FreeExtra();
		ctmp = m_ListString[m_MSGTYPE + 1];
		if (ctmp.Find(_T("{")) == -1)
		{
			return;
		}
	}
	for (int index = m_MSGTYPE + 1;ctmp.Find(_T("}")) == -1; index++)
	{
		ctmp.Empty();
		ctmp.FreeExtra();
		ctmp = m_ListString[index];
		if (((ctmp.Find(_T("{")) == -1)&&((ctmp.Find(_T("}")))&&(ctmp.IsEmpty() == 0))))
		{
			m_MSGTYPE_List.push_back(m_ListString[index]);
		}
	}
	//MSGCONMMAND
	ctmp = m_ListString[m_MSGCONMMAND];
	if (ctmp.Find(_T("{")) == -1)
	{
		ctmp.Empty();
		ctmp.FreeExtra();
		ctmp = m_ListString[m_MSGCONMMAND + 1];
		if (ctmp.Find(_T("{")) == -1)
		{
			return;
		}
	}
	for (int index = m_MSGCONMMAND + 1;ctmp.Find(_T("}")) == -1; index++)
	{
		ctmp.Empty();
		ctmp.FreeExtra();
		ctmp = m_ListString[index];
		if (((ctmp.Find(_T("{")) == -1)&&((ctmp.Find(_T("}")))&&(ctmp.IsEmpty() == 0))))
		{
			m_MSGCONMMAND_List.push_back(m_ListString[index]);
		}
	}

	//消息载体
	ctmp.Empty();
	ctmp.FreeExtra();
	for(int index = 0;index < (int)m_MsgConst.size(); index++)
	{
		ctmp = m_ListString[m_MsgConst[index]];
		if (ctmp.IsEmpty() == 0)
		{
			ctmp.TrimLeft(_T("class"));
			ctmp.TrimRight(_T("public CMessage"));
			ctmp.Remove(_T(' '));
			ctmp.Remove(_T(':'));
			ctmp.Remove(_T('	'));
			m_Msg_List.push_back(ctmp);
		}
	}

}

CString MessageAutoDispose::GetID(CString cStr)
{
	CString ctmp;
	cStr = cStr + _T("_COM");
	for (int index = 0; index < (int)m_MSGCONMMAND_List.size();index++)
	{
		if (m_MSGCONMMAND_List[index].Find(cStr) != -1)
		{
			ctmp = m_MSGCONMMAND_List[index];
			ctmp.Replace(cStr,_T(" "));
			ctmp.Remove(_T(' '));
			ctmp.Remove(_T('='));
			ctmp.Remove(_T(','));
			ctmp.Remove(_T('	'));
			break;
		}
	}
	return ctmp;
}

int MessageAutoDispose::SetID(byte in_byte)
{
	return 0;
}

int MessageAutoDispose::GetParam( CString cStr )
{
	CString ctmp;
	CString cType;
	CString cName;
	m_Msg_Param_List.clear();
	m_Msg_ParamType_List.clear();
	m_Msg_ParamNote_List.clear();
	int     Pos = 0;
	for (int index = 0; index < (int)m_MSGCONMMAND_List.size();index++)
	{
		if (m_Msg_List[index].Find(cStr) != -1)
		{
			for (int in_index = 0; in_index < (int)(m_ListString.size() - m_MsgConst[index]);in_index++)
			{
				ctmp = m_ListString[m_MsgConst[index] + in_index];
				Pos  = ctmp.Find(_T("Get"));
				if (Pos != -1)
				{
					//获取参数名
					cName = ctmp.Mid(Pos + 3 );
					cName.Remove(_T('('));
					cName.Remove(_T(')'));
					cName.Remove(_T(';'));
					cName.Remove(_T(' '));
					cName.Remove(_T('{'));
					cName.Remove(_T('	'));
					if (cName.Right(3) !=  (_T("Len")))
					{
						//取参数类型
						cType = ctmp.Left(Pos);
						cType.Remove(_T(' '));
						cType.Remove(_T('	'));
						m_Msg_ParamType_List.push_back(cType);
						m_Msg_Param_List.push_back(cName);
						//取参数注释
						m_Msg_ParamNote_List.push_back(m_ListString[m_MsgConst[index] + (in_index - 1)]);
					}

				}
				if (ctmp.Find(_T("};")) != -1)
				{
					break;
				}
			}
			break;
		}	
	}
	return 0;
}

CString MessageAutoDispose::GetNote( CString cStr )
{
	CString ctmp;
	cStr = cStr + _T("_COM");
	for (int index = 0; index < (int)m_MSGCONMMAND_List.size();index++)
	{
		if (m_MSGCONMMAND_List[index].Find(cStr) != -1)
		{
			ctmp = m_ListString[m_MsgNote[index - 1]];
			break;
		}
	}
	return ctmp;
}
