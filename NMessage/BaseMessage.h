#ifndef  VNOC_C_MESSAGE_BASE
#define  VNOC_C_MESSAGE_BASE

#include "IBaseMessage.h"
#include <map>

namespace VNOC
{
namespace Message
{

class BaseMessage : public IMessage
{
public:
    BaseMessage();
    virtual ~BaseMessage();

    virtual MsgStatus Read(
        IN const MsgDataName& name,
        OUT MsgDataValue& value);
    virtual MsgStatus Write(
        IN const MsgDataName& name,
        IN const MsgDataValue& value);

private:
    std::map<MsgDataName, MsgDataValue> m_mapMsgData;
};

}// namespace Message
}// namespace VNOC

#endif
