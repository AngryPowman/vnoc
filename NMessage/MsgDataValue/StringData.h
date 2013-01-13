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
    virtual ~StringData(){}

    virtual MsgStatus ToString(
        OUT std::string& Value
        )
    {
         Value = m_Value;
         return MsgStatus_Ok;
    }
private:
    std::string      m_Value;
};

}// namespace Message
}// namespace VNOC

#endif