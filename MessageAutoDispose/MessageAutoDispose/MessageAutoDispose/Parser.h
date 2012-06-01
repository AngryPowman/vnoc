#pragma  once

#include <utility>
#include <vector>


class MessageAutoDispose
{
public:
	MessageAutoDispose(){
		m_LineMax = 0;
	}
	
	~MessageAutoDispose(){
		
	}

	int  Open(TCHAR* Path);
	
	int  GetParam(CString cStr);

	//int  SetParam(CString cStr);

	CString  GetNote(CString cStr);

	CString  GetID(CString cStr);

	int  SetID(byte in_byte);
	
	std::vector<CString> GetMessageParam(){
		return m_Msg_Param_List;
	}
	
	std::vector<CString> GetMessageParamNote(){
		return m_Msg_ParamNote_List;
	}

	std::vector<CString> GetMessageParamType(){
		return m_Msg_ParamType_List;
	}

	std::vector<CString> GetMessageList(){
		return m_Msg_List;
	}	

private:

	void _Analysis();
	
	CStdioFile   m_File;
	//ÐÐÊý
	std::vector<CString>     m_ListString;
	std::vector<int>		 m_ListIndex;
	std::vector<CString>	 m_MSGTYPE_List;
	std::vector<CString>     m_MSGCONMMAND_List;
	std::vector<CString>     m_Msg_List;
	std::vector<CString>     m_Msg_ParamType_List;
	std::vector<CString>     m_Msg_Param_List;
	std::vector<CString>     m_Msg_ParamNote_List;
	std::vector<int>		 m_MsgConst;
	std::vector<int>		 m_MsgNote;
	int			 m_LineMax;
	int			 m_MSGTYPE;
	int			 m_MSGCONMMAND;
	LPTSTR*		 m_Buffer;  
	//void _Close();
};


