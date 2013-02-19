#ifndef VNOC_MESSAGEPARSER
#define VNOC_MESSAGEPARSER

#include "XMLObject.h"
#include "MessageDef.h"
#include "BufferMessage.h"
#include "IWriteMessage.h"

namespace VNOC
{
namespace Message
{

class CMessage2Parser
{
public:
    CMessage2Parser() : m_MsgId(0) {}
    virtual ~CMessage2Parser() {}

    MsgStatus Parser(OUT IWriteMessage* pMsg, IN const CBufferMessage& pBuf);
    bool IsVaild(IN const CBufferMessage& pBuf);
    static int GetMsgType(IN const CBufferMessage& pBuf);

private:
    MsgStatus _ParserHead(IWriteMessage* pMsg, const CBufferMessage& pBuf);
    MsgStatus _ParserTail(IWriteMessage* pMsg, const CBufferMessage& pBuf);
    MsgStatus _ParserParam(IWriteMessage* pMsg, const CBufferMessage& pBuf);
    int _ByteToInt(Define::uint8* pData);

private:
    int m_MsgId;
};

}
}

#endif