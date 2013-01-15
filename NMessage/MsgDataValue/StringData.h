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

    StringData(std::string Value) { SetValue(Value);}

    virtual MsgStatus ToStr(std::string& Value);

    void SetValue(
        IN std::string Value);
private:
    std::string      m_Value;
};

}// namespace Message
}// namespace VNOC

#endif