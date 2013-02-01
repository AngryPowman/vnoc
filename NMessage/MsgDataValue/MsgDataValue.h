#ifndef  VNOC_I_MESSAGE_DATA
#define  VNOC_I_MESSAGE_DATA

#include "../MessageDef.h"

namespace VNOC
{
namespace Message
{

class MsgDataValue
{
public:
    MsgDataValue();
    virtual ~MsgDataValue();

    virtual MsgStatus ToStr(OUT std::string& Value);

    virtual MsgStatus ToUInt8(OUT uint8& Value);

    virtual MsgStatus ToUInt16(OUT uint16& Value);

    virtual MsgStatus ToUInt32(OUT uint32& Value);

    virtual MsgStatus ToInt8(OUT int8& Value);

    virtual MsgStatus ToInt16(OUT int16& Value);

    virtual MsgStatus ToInt32(OUT int32& Value);
};

}// namespace Message
}// namespace VNOC

#endif