#ifndef  VNOC_MESSAGE_DEF
#define  VNOC_MESSAGE_DEF

#define  IN
#define  OUT

#include <string>
#include <vector>

namespace VNOC
{
namespace Message
{

#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&);             \
    void operator=(const TypeName&);
#endif

#ifndef DISALLOW_ASSIGN
#define DISALLOW_ASSIGN(TypeName) \
    void operator=(const TypeName&);
#endif

typedef std::string         MsgDataName;
typedef int                 MsgDataNameInt;
typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
typedef char                int8;
typedef short               int16;
typedef int                 int32;

typedef enum _VMsg
{

    //服务器节点
    MSG_RequestQueryServer              = 1100, //获取一个服务器节点（请求）
    MSG_AnswerQueryServer               = 1101, //获取一个服务器节点（应答）

    //账号处理
    MSG_RequestRegister                 = 2100, //注册帐号（请求）
    MSG_AnswerRegister                  = 2101, //注册帐号（应答）

    //验证码
    MSG_RequestVerificationCode         = 3101, //获取验证码（请求）
    MSG_AnswerVerificationCode          = 3102, //获取验证码（应答）

    //登陆
    MSG_RequestLogin                    = 4101, //登录（请求）
    MSG_AnswerLogin                     = 4102, //登录（应答）

    //个人
    MSG_RequestProfileSync              = 5101, //个人信息同步（通知）
    MSG_AnswerProfileSync               = 5102, //个人信息同步（通知确认）

    //班级
    MSG_RequestClassSync                = 6101, //班级信息同步（通知）
    MSG_AnswerClassSync                 = 6102, //班级信息同步（通知确认）
    MSG_RequestClassSyncResult          = 6103, //班级同步结果（通知）
    MSG_AnswerClassSyncResult           = 6104, //班级同步结果（通知确认）
    MSG_RequestClassList                = 6105, //获取班级列表（请求）
    MSG_AnswerClassList                 = 6106, //获取班级列表（应答）
    MSG_RequestClassInfo                = 6107, //获取班级信息（请求）
    MSG_AnswerClassInfo                 = 6108, //获取班级信息（应答）
}VMsg;

typedef enum _MsgDataMType
{
    MsgDataMType_Null,
    MsgDataMType_Data,
    MsgDataMType_List,
}MsgDataMType;

#define MsgDataMType_XML_Data     "data"
#define MsgDataMType_XML_List     "List"

#define MsgDataObject_XML_Name    "name"
#define MsgDataObject_XML_Id      "id"
#define MsgDataObject_XML_MType   "mtype"
#define MsgDataObject_XML_Type    "type"
#define MsgDataObject_XML_Root    "vnoc"
#define MsgDataObject_XML_Msg     "msg"

#define MsgDataType_XML_Dword     "dword"
#define MsgDataType_XML_Byte      "byte"
#define MsgDataType_XML_String    "string"

typedef enum _MsgDataType
{
    MsgDataType_Null,
    MsgDataType_String,
    MsgDataType_Dword,
    MsgDataType_Byte,
}MsgDataType;

typedef enum _MsgStatus
{
   MsgStatus_Ok,
   MsgStatus_Err,
   MsgStatus_Unk,
}MsgStatus;

}// namespace Message
}// namespace VNOC

#endif