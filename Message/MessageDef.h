

#ifndef VNOC_MESSAGE_DEF
#define VNOC_MESSAGE_DEF


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

    MSG_ACS_COM = 0x1A,



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


#endif

