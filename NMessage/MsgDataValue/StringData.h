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
    StringData(){}
    virtual ~StringData(){};

     virtual MsgStatus ToString(
        OUT string& Value
        )
     {
         Value = m_Value;
         return MsgStatus_Ok;
     }

private:

    string      m_Value;

};



}// namespace Message
}// namespace VNOC

#endif