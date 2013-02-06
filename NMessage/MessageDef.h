#ifndef  VNOC_MESSAGE_DEF
#define  VNOC_MESSAGE_DEF

#ifndef IN
#define  IN
#define  OUT
#endif

#ifndef EMPTY_TEMPTALE_DEFINE
#define  EMPTY_TEMPTALE_DEFINE template<>
#endif

// �����ʹ�С�˻���
#define BigLittleSwap16(A)        ((((ushort)(A) & 0xff00) >> 8) | \
    (((ushort)(A) & 0x00ff) << 8))

// �����ʹ�С�˻���
#define BigLittleSwap32(A)        ((((uint)(A) & 0xff000000) >> 24) | \
    (((uint)(A) & 0x00ff0000) >> 8) | \
    (((uint)(A) & 0x0000ff00) << 8) | \
    (((uint)(A) & 0x000000ff) << 24))

#include <string>
#include <vector>

namespace VNOC
{
namespace Message
{

#define MSG_BEGIN    0x56 // 'V' �����Ϣ�Ŀ�ʼ
#define MSG_END      0x43 // 'C' �����Ϣ�Ľ���
#define MSG_VER      0 // �汾��

#define MSG_HEAD_LEN 10
#define MSG_TAIL_LEN 1

#define MSG_CLASS_BEGIN                1
#define MSG_CLASS_END                  1
#define MSG_CLASS_LEN                  4
#define MSG_CLASS_VER                  1
#define MSG_CLASS_SERIAL               2
#define MSG_CLASS_GUID                 16
#define MSG_CLASS_COMMAND              4
#define MSG_CLASS_OBL                  4
#define MSG_CLASS_PARAMCONST           1
#define MSG_CLASS_VERIFY               2 // Ч���� δ��Ĭ��Ϊ2���ֽ�
#define MSG_CLASS_PARAM                4
#define MSG_CLASS_PARAM_COUNT          4

#define MSG_COMMAND_INDEX  MSG_CLASS_VER + MSG_CLASS_BEGIN
#define MSG_PACKSIZE_INDEX MSG_COMMAND_INDEX + MSG_CLASS_COMMAND

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
#define MsgDataMType_XML_List     "list"

#define MsgDataObject_XML_Name    "name"
#define MsgDataObject_XML_Id      "id"
#define MsgDataObject_XML_MType   "mtype"
#define MsgDataObject_XML_Type    "type"
#define MsgDataObject_XML_Root    "vnoc"
#define MsgDataObject_XML_Msg     "msg"

#define MsgDataType_XML_Uint32    "uint32"
#define MsgDataType_XML_Uint16    "uint16"
#define MsgDataType_XML_Uint8     "uint8"
#define MsgDataType_XML_String    "string"

typedef enum _MsgDataType
{
    MsgDataType_Null,
    MsgDataType_String,
    MsgDataType_Uint8,
    MsgDataType_Uint16,
    MsgDataType_Uint32,
}MsgDataType;

typedef enum _MsgStatus
{
   MsgStatus_Ok,
   MsgStatus_Err,
   MsgStatus_Unk,
   MsgStatus_FormatFailure,
   MsgStatus_TypeErr,
}MsgStatus;

}// namespace Message
}// namespace VNOC

#endif