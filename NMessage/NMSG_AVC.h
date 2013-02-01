#ifndef VNOC_C_MESSAGE_NMSG_AVC
#define VNOC_C_MESSAGE_NMSG_AVC
#include "MessageDef.h"
#include "CMessage.h"
#include "MsgDataValue/StringData.h"
#include "MsgDataValue/NumData.hpp"
//测试中文
///> Generate by MessageProduse. DO NOT EDIT!

namespace VNOC
{
namespace Message
{

class NMSG_AVC : public CMessage
{
public:
    NMSG_AVC()
    {
        InitializeMessage("NMSG_AVC");
    }

    virtual ~NMSG_AVC(){}

    MsgStatus SetTest(const std::vector<std::string>& Value)
    {
        ArrayData* ValueArr = new ArrayData;
        ValueArr->Push<std::string>(Value);
        return WriteArr("Test", ValueArr);
    }

    MsgStatus GetTest(std::vector<std::string>& Value)
    {
        ArrayData* pReadValueArr = NULL;
        ReadArr("Test", pReadValueArr);
        pReadValueArr->GetArr_vec<std::string>(Value);
        return MsgStatus_Ok;
    }
};

}// namespace Message
}// namespace VNOC

#endif