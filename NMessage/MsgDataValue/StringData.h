#ifndef  VNOC_I_MESSAGE_DATA_STRING
#define  VNOC_I_MESSAGE_DATA_STRING

#include "MsgDataValue.h"


namespace VNOC
{
namespace Message
{

class StringData:public MsgDataValue
{
public:

     virtual MsgStatus ToString(
        OUT string&  Value
        )
     { return MsgStatus_Ok;}

private:

    string      m_Value;

};



}// namespace Message
}// namespace VNOC

#endif