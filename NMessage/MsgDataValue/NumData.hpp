#ifndef VNOC_C_MESSAGE_UINT
#define VNOC_C_MESSAGE_UINT

#include "NumData_t.hpp"

namespace VNOC
{
namespace Message
{

typedef NumData<Define::int8>    Int8Data;
typedef NumData<Define::int16>   Int16Data;
typedef NumData<Define::int32>   Int32Data;
typedef NumData<Define::uint8>   UInt8Data;
typedef NumData<Define::uint16>  UInt16Data;
typedef NumData<Define::uint32>  UInt32Data;

EMPTY_TEMPTALE_DEFINE
MsgStatus UInt8Data::ToUInt8(Define::uint8& Value)
{
    Value = m_Value;
    return MsgStatus_Ok;
}

EMPTY_TEMPTALE_DEFINE
MsgStatus UInt16Data::ToUInt16(Define::uint16& Value)
{
    Value = m_Value;
    return MsgStatus_Ok;
}

EMPTY_TEMPTALE_DEFINE
MsgStatus UInt32Data::ToUInt32(Define::uint32& Value)
{
    Value = m_Value;
    return MsgStatus_Ok;
}

EMPTY_TEMPTALE_DEFINE
MsgStatus Int8Data::ToInt8(Define::int8& Value)
{
    Value = m_Value;
    return MsgStatus_Ok;
}

EMPTY_TEMPTALE_DEFINE
MsgStatus Int16Data::ToInt16(Define::int16& Value)
{
    Value = m_Value;
    return MsgStatus_Ok;
}

EMPTY_TEMPTALE_DEFINE
MsgStatus Int32Data::ToInt32(Define::int32& Value)
{
    Value = m_Value;
    return MsgStatus_Ok;
}

}// namespace Message
}// namespace VNOC

#endif