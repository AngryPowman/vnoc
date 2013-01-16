#ifndef  VNOC_MESSAGE2PACK
#define  VNOC_MESSAGE2PACK

#include "MessageDef.h"
#include "IReadMessage.h"
#include "XMLObject.h"

namespace VNOC
{
namespace Message
{

///> MSG转Pack
class CMessage2Pack
{
public:
    CMessage2Pack();
    ~CMessage2Pack();

    ///> size为按uint8（char）的个数算
    MsgStatus GetPackSize(IN IReadMessage* pMsg, OUT int& nSize);
    MsgStatus PackMessage(IN IReadMessage* pMsg, OUT char* pBuf);

private:
    int _GetDataSize(XMLItem& item, IReadMessage* pMsg);
    int _GetStringSize(XMLItem& item, MsgDataValue* pReadStr);
    int _GetArrSize(XMLItem& item, IReadMessage* pMsg);
};

}// namespace Message
}// namespace VNOC

#endif