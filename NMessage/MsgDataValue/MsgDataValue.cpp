#include "MsgDataValue.h"

namespace VNOC
{
namespace Message
{

MsgDataValue::MsgDataValue()
{

}

MsgDataValue::~MsgDataValue()
{

}

MsgStatus MsgDataValue::ToStr( std::string& Value )
{
    return MsgStatus_Unk;
}

MsgStatus MsgDataValue::ToUInt8( uint8& Value )
{
    return MsgStatus_Unk;
}

MsgStatus MsgDataValue::ToUInt16( uint16& Value )
{
    return MsgStatus_Unk;
}

MsgStatus MsgDataValue::ToUInt32( uint32& Value )
{
    return MsgStatus_Unk;
}

}// namespace Message
}// namespace VNOC
