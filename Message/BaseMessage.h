
#ifndef VNOC_MSG_CMESSAGE
#define VNOC_MSG_CMESSAGE
/*
#define  MSG_ERROR_BDMISS  1
#define  MSG_ERROR_COMMAND 2
#define  MSG_ERROR_PARAM   3
#define  MSG_ERROR_LEN     4*/

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm> 

#define MSG_BEGIN    0x56   // 'V' 标记消息的开始
#define MSG_END		 0x43	// 'C' 标记消息的结束
#define MSG_VER      0		//版本号

#define  MSG_CLASS_BEGIN  1
#define  MSG_CLASS_END    1
#define  MSG_CLASS_LEN    4
#define	 MSG_CLASS_VER    1
#define  MSG_CLASS_SERIAL 2
#define  MSG_CLASS_GUID   16
#define  MSG_CLASS_COMMAND 1
#define  MSG_CLASS_OBL    4
#define  MSG_CLASS_PARAMCONST 1
#define  MSG_CLASS_VERIFY 2   //效验码 未定默认为2个字节
#define  MSG_CLASS_PARAM  4

#define  VER_INDEX    MSG_CLASS_VER
#define  SER_INDEX    (MSG_CLASS_VER + MSG_CLASS_SERIAL)
#define  LEN_INDEX    (MSG_CLASS_LEN + MSG_CLASS_VER + MSG_CLASS_SERIAL)
#define  GUID_INDEX   (MSG_CLASS_LEN + MSG_CLASS_VER + MSG_CLASS_SERIAL + MSG_CLASS_GUID)
#define  COM_INDEX    (MSG_CLASS_LEN + MSG_CLASS_VER + MSG_CLASS_SERIAL + MSG_CLASS_COMMAND + MSG_CLASS_GUID)
#define  OBL_INDEX    (MSG_CLASS_LEN + MSG_CLASS_VER + MSG_CLASS_SERIAL + MSG_CLASS_GUID + MSG_CLASS_COMMAND + MSG_CLASS_OBL)
#define  PAC_INDEX    (MSG_CLASS_LEN + MSG_CLASS_VER + MSG_CLASS_SERIAL + MSG_CLASS_GUID + MSG_CLASS_COMMAND + MSG_CLASS_OBL + MSG_CLASS_PARAMCONST)


enum  MSGTYPE
{
	MSGTYPE_NULL,

	MSG_AVC_TYPE,
	MSG_RVC_TYPE,

	MSG_ALI_TYPE,
	MSG_RLI_TYPE,

	MSG_RPS_TYPE,
	MSG_APS_TYPE,

	MSG_ACI_TYPE,
	MSG_RCI_TYPE,

	MSG_ACL_TYPE,
	MSG_RCL_TYPE,

	MSGTYPE_END
};

enum  MSGCONMMAND
{
	MSGCONMMAND_NULL,

	MSG_RPG_COM = 0x12,
	MSG_APG_COM = 0x13,

	MSG_RVC_COM = 0x14,
	MSG_AVC_COM = 0x15,

	MSG_RLI_COM = 0x16,
	MSG_ALI_COM = 0x17,

	MSG_RPS_COM = 0x18,
	MSG_APS_COM = 0x19,

	MSG_RCI_COM = 0x20,
	MSG_ACI_COM = 0x21,

	MSG_RCL_COM = 0x1E,
	MSG_ACL_COM = 0x1F,

};

typedef  unsigned char byte;	
typedef  unsigned int  uint;
typedef unsigned short ushort;

typedef std::map<std::string,int> EnumFunList;
typedef std::map<std::string,int>::iterator EnumFunList_Iterator;

typedef std::vector<byte> ByteArr;

// 短整型大小端互换
#define BigLittleSwap16(A)        ((((ushort)(A) & 0xff00) >> 8) | \
	(((ushort)(A) & 0x00ff) << 8))

// 长整型大小端互换
#define BigLittleSwap32(A)        ((((uint)(A) & 0xff000000) >> 24) | \
	(((uint)(A) & 0x00ff0000) >> 8) | \
	(((uint)(A) & 0x0000ff00) << 8) | \
	(((uint)(A) & 0x000000ff) << 24))


uint byteToInt(const byte* in_byte,size_t len);

void IntTobyte(int in_int,byte* out_byte);


#define OBJECT_PARAM_LIST  m_ParamMate

#define Mate_Param  MateParam

#define BEGIN_PARAM_LIST  int __Param_List_Begin = 0;

#define ADD_PARAM_LIST(PARAM_NAME)	  OBJECT_PARAM_LIST[PARAM_NAME] = __Param_List_Begin++;

#define INIT_PARAM_OBJEDT(COMMAND_TAG) CMessage::InitiaPack(COMMAND_TAG,__Param_List_Begin);

#define END_PARAM_LIST 

#define SetParam_t_ptr(x,y,z)  {SetParam(Mate_Param(x),y,z);}

#define SetParam_t_tamp(x,y,z)  {SetParam<y>(Mate_Param(x),z);}

#define SetParam_t(x,y)  {SetParam(Mate_Param(x),y);}

#define GetParamLen_t_r(x) {return  CMessage::GetParamLen(Mate_Param(x));}

//#define GetParam_t(x)  {GetParam(x);}

#define GetParam_t_r(x)  { return GetParam(Mate_Param(x)); }

#define GetParam_t_tamp_r(x,y,z)  { return GetParam<y>(Mate_Param(x),z); }

#define GetParam_t_byte_r_int(x,len)  { return byteToInt(GetParam(Mate_Param(x)),len); }

#define GetParam_t_int_r(x,len)  { return BigLittleSwap32(byteToInt(GetParam(Mate_Param(x)),len)); }

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
	void   SetParam( uint Pos, std::vector<Type> PeopleList)
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
		byte* tmpParam = new byte[PeopleList.size() * sizeof(Type)];
		memset(tmpParam,0,PeopleList.size() * sizeof(Type));
		int  tmpInt = 0;
		for (int index = 0,ParamPos = 0; index <  (int)PeopleList.size();index++)
		{
			IntTobyte(PeopleList[index],tmpByte);
			for (int i = 0; i < sizeof(Type); i++,ParamPos++)
			{
				tmpParam[ParamPos] = tmpByte[i];
			}
			memset(tmpByte,0,4);
		}
		SetParam(Pos,tmpParam,PeopleList.size() * sizeof(Type));
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
				for (int j = 0; j < sizeof(Type); i++)
				{
					ComTmpList[index].push_back(m_ComCommandList[int_index].second[i]);
				}
			}

			for (int i = 0; i < (int)ComTmpList.size(); i++)
			{
				PeopleList.push_back(byteToInt((byte*)ComTmpList[i].data(),sizeof(Type)));
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

	bool  m_Begin;				  //标记消息的开始  统一字符'V"的ASCⅡ码 0x56
	bool  m_End;				  //标记消息的结束  固定字符'C‘的ASCⅡ码 0x43
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