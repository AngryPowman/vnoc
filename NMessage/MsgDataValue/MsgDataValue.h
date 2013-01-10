#ifndef  VNOC_I_MESSAGE_DATA
#define  VNOC_I_MESSAGE_DATA

#include "../MessageDef.h"

namespace VNOC
{
namespace Message
{

typedef     int     MessageName;
typedef     int     MessageType;

class MsgDataValue
{
public:
    MsgDataValue(){}
    virtual ~MsgDataValue(){}

    virtual MsgStatus ToStr(){ return MsgStatus_Unk;}

    virtual MsgStatus ToUInt8(){ return MsgStatus_Unk;}

    virtual MsgStatus ToUInt16(){ return MsgStatus_Unk;}

    virtual MsgStatus ToUInt32(){ return MsgStatus_Unk;}

    virtual MsgStatus ToInt32(){ return MsgStatus_Unk;}
};

}// namespace Message
}// namespace VNOC

#endif