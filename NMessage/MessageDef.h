#ifndef  VNOC_MESSAGE_DEF
#define  VNOC_MESSAGE_DEF

namespace VNOC
{
namespace Message
{

    typedef int     MsgDataName;
    typedef int     MsgDataValue;
    typedef enum _MsgStatus
    {
        MsgStatus_Ok,
        MsgStatus_Err,
        MsgStatus_Unk,
    }MsgStatus;

}// namespace Message
}// namespace VNOC

#endif