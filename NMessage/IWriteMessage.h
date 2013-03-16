#ifndef  VNOC_I_MESSAGE_WRITE
#define  VNOC_I_MESSAGE_WRITE

#include "MessageDef.h"
#include "MsgDataValue/MsgDataValue.h"
#include "MsgDataValue/ArrayData.h"

namespace VNOC
{
namespace Message
{

class ArrayData;

class IWriteMessage
{
public:
    IWriteMessage(){}
    virtual ~IWriteMessage(){}

public:
    virtual MsgStatus Write(
        IN const Define::MsgDataName& name,
        IN MsgDataValue* pValue) = 0;

    virtual MsgStatus WriteArr(
        IN const Define::MsgDataName& name,
        IN ArrayData* pValue) = 0;
};

}// namespace Message
}// namespace VNOC

#endif