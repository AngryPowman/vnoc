#ifndef  VNOC_MESSAGE2PACK
#define  VNOC_MESSAGE2PACK

#include "MessageDef.h"
#include "IReadMessage.h"
#include "XMLObject.h"
#include "BufferMessage.h"

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
    MsgStatus PackMessage(IN IReadMessage* pMsg, OUT CBufferMessage& pBuf);

private:
    int _GetDataSize(XMLItem& item, IReadMessage* pMsg);
    int _GetStringSize(XMLItem& item, MsgDataValue* pReadStr);
    int _GetArrSize(XMLItem& item, IReadMessage* pMsg);
    int _PackHead(int PackMsgId, CBufferMessage& pBuf);
    int _PackTail(int nPackPos, CBufferMessage& pBuf);
    MsgStatus _PushMessageDataParam(
        XMLItem& item,
        IReadMessage* pMsg,
        std::string ParamName,
        std::vector<int>& vecParamLen,
        std::vector<std::vector<uint8> >& vecParamList);
    MsgStatus _PushMessageListParam(
        XMLItem& item,
        IReadMessage* pMsg,
        std::string ParamName,
        std::vector<int>& vecParamLen,
        std::vector<std::vector<uint8> >& vecParamList);
    MsgStatus _PackParam(
        XMLObject* pXmlObj,
        CBufferMessage& pBuf,
        int& nPackPos,
        std::vector<int>& vecParamLen,
        std::vector<std::vector<uint8> >& vecParamList);
};

}// namespace Message
}// namespace VNOC

#endif