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

    //�������ڵ�
    MSG_RequestQueryServer              = 1100, //��ȡһ���������ڵ㣨����
    MSG_AnswerQueryServer               = 1101, //��ȡһ���������ڵ㣨Ӧ��

    //�˺Ŵ���
    MSG_RequestRegister                 = 2100, //ע���ʺţ�����
    MSG_AnswerRegister                  = 2101, //ע���ʺţ�Ӧ��

    //��֤��
    MSG_RequestVerificationCode         = 3101, //��ȡ��֤�루����
    MSG_AnswerVerificationCode          = 3102, //��ȡ��֤�루Ӧ��

    //��½
    MSG_RequestLogin                    = 4101, //��¼������
    MSG_AnswerLogin                     = 4102, //��¼��Ӧ��

    //����
    MSG_RequestProfileSync              = 5101, //������Ϣͬ����֪ͨ��
    MSG_AnswerProfileSync               = 5102, //������Ϣͬ����֪ͨȷ�ϣ�

    //�༶
    MSG_RequestClassSync                = 6101, //�༶��Ϣͬ����֪ͨ��
    MSG_AnswerClassSync                 = 6102, //�༶��Ϣͬ����֪ͨȷ�ϣ�
    MSG_RequestClassSyncResult          = 6103, //�༶ͬ�������֪ͨ��
    MSG_AnswerClassSyncResult           = 6104, //�༶ͬ�������֪ͨȷ�ϣ�
    MSG_RequestClassList                = 6105, //��ȡ�༶�б�����
    MSG_AnswerClassList                 = 6106, //��ȡ�༶�б�Ӧ��
    MSG_RequestClassInfo                = 6107, //��ȡ�༶��Ϣ������
    MSG_AnswerClassInfo                 = 6108, //��ȡ�༶��Ϣ��Ӧ��
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