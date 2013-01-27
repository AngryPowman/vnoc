
#ifndef VNOC_MSG_CMESSAGE
#define VNOC_MSG_CMESSAGE

#include "MessageDef.h"
/*
class MakeParam
{
public:	

	int MateParam( std::string in_str )
	{
		
		return -1;
	}

	EnumFunList& returnParamMakeObject(){ return m_ParamComList;}

private:

	EnumFunList m_ParamComList;
};
*/

uint byteToInt(const byte* in_byte,size_t len);

void IntTobyte(int in_int,byte* out_byte);


class CMessage
{
public:

	friend class CMessageParser;
	friend class PackMessage;

	CMessage(){
		_Initialization();
	}

	virtual ~CMessage(){
		_Close();
	}
	//获取消息类型
	int  GetMessageType() const;


public:

	bool GetBeginTab() const ;

	bool GetEndTab() const;

	byte GetCommand() const;

	byte GetComListLen(uint in_index) const;

	uint GetSerial() const;

	const byte* GetGUID() const;

	const byte* GetComCommandList(uint in_index) const;


	uint GetVerify() const;

	uint GetObligate() const;

	uint GetVersion() const;

	uint GetCmlCount() const;

	uint GetDataLen() const;


	// Set

	bool SetCommand(byte in_byte);

	bool SetSerial(ushort in_short);

	bool SetGUID(byte* in_byte_ptr);

	bool SetComCommandList(int ComCount);

	bool SetComListLen(byte* in_byte_ptr,int CmlCount);

	bool SetVerify(uint in_Int);

	bool SetObligate(uint in_Int);

	bool SetVersion(uint in_Int);

	bool SetCmlCount(uint in_Int);

	bool SetDataLen(uint in_Int);

protected:

	//模板函数特殊处理

	template<typename Type>
	void   SetParam( uint Pos,const std::vector<Type> PeopleList)
	{
		if (Pos == -1)
		{
			return;
		}
		if (PeopleList.empty())
		{
			return;
		}
		byte tmpByte[4] = {0};
        uint PeopleLen = PeopleList.size();
		byte* tmpParam = new byte[PeopleLen * sizeof(Type)];
		memset(tmpParam,0,PeopleLen * sizeof(Type));
		int  tmpInt = 0;
		for (int index = 0,ParamPos = 0; index <  (int)PeopleLen;index++)
		{
			IntTobyte(PeopleList[index],tmpByte);
			for (int i = 0; i < sizeof(Type); (i++,ParamPos++))
			{
				tmpParam[ParamPos] = tmpByte[i];
			}
			memset(tmpByte,0,4);
		}
		SetParam(Pos,tmpParam,PeopleLen * sizeof(Type));
		if (tmpParam != NULL)
		{
			delete[] tmpParam;
			tmpParam = NULL;
		}
	}


	template<typename Type>
	Type  GetParam(uint int_index,std::vector<Type>& PeopleList) const
	{
		if (int_index != -1)
		{
			std::vector<std::vector<Type> > ComTmpList;
			if (m_ComCommandList[int_index].second.size() == 0)
			{
				return 1;
			}
			if ((m_ComCommandList[int_index].second.size() % sizeof(Type) ) != 0)
			{
				return 1;
			}
			ComTmpList.resize(((int)m_ComCommandList[int_index].second.size() / sizeof(Type)));
			for (int i = 0,index = 0;index < (int)(m_ComCommandList[int_index].second.size() / sizeof(Type)) ;index++)
			{
				for (int j = 0; j < sizeof(Type); (i++,j++))
				{
					ComTmpList[index].push_back(m_ComCommandList[int_index].second[i]);
				}
			}

			for (int i = 0; i < (int)ComTmpList.size(); i++)
			{
				byte* tempBytes = new byte[sizeof(Type)];
				memset(tempBytes,0,sizeof(Type));
				for (int j=0; j<sizeof(Type); ++j)
				{
					tempBytes[sizeof(Type)-j-1] = ComTmpList[i][j];
				}
				PeopleList.push_back(byteToInt(tempBytes,sizeof(Type)));
				delete[] tempBytes;
			}
		}
		return 0;
	}

protected:

	void   SetParam( int Pos, byte in_Param_byte);

	void   SetParam( int Pos,const byte* in_Param_byte_Ptr,uint len);

	void   SetParam( int Pos, uint  in_Param_int);

	//子类必须初始化 
	virtual void   InitiaPack(int in_ComType, int in_ComNum);

	virtual uint   GetParamLen(uint int_index) const;

	virtual const  byte*  GetParam( int int_index ) const;

	virtual void   SetParamLen(uint int_index, int int_Value);

	//预留
	//virtual int PopParam();

protected:

	int _MessageType(byte in_Command) const;

	int MateParam( std::string in_str ) const;

	EnumFunList m_ParamMate;

	bool  m_Begin;  //标记消息的开始  统一字符'V"的ASCⅡ码 0x56
	bool  m_End;    //标记消息的结束  固定字符'C‘的ASCⅡ码 0x43
	//参数列表    4字节，对应参数N的长度
	std::vector<byte>			 m_ComListLen;  
	//参数列表    编码后的参数，具体类型根据具体指令决定
	std::vector<std::pair<int,std::vector<byte> > >  m_ComCommandList;
	byte   m_Command;			  //指令			具体的指令，用来标注此数据包的功能
	byte   m_Serial[2];			  //序号       指令的编号
	byte   m_GUID[16];            //GUID       用来提供用户验证
	//DWORD dwType;				  //消息类型		


	uint  m_Verify;               //效验码 预订

	uint  m_Obligate;              //预留空间
	uint  m_Ver;				   //版本号     标志本VNOC协议的版本
	uint  m_CmlCount;			   //参数数量    编码后的参数，具体类型根据具体指令决定
	uint  m_Len;                   //包体长度	   4个字节，用于指定包体的长度（可以辅助数据解析）

	//错误标记
	int m_Error;

private:

	void   _Initialization();
	void   _Close();
};


#endif