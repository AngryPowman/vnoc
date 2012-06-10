#include "VnocProtocol.hpp"
#include <assert.h>
void VnocProtocol::RegisterMessageHandler(IMessageHandler *MessageHandler)
{
    assert(MessageHandler != NULL);
    if (MessageHandler == NULL) {return;}
    MSGTYPE type = MessageHandler->getMessageType();
    assert (type != MSGTYPE_NULL);
    assert (type < MSGTYPE_END);
    handlerMap_[type].push_back(MessageHandler);
}

list<IMessageHandler *>& VnocProtocol::getHandler(MSGTYPE msgType)
{
    assert (msgType != MSGTYPE_NULL);
    assert (msgType < MSGTYPE_END);
    return handlerMap_[msgType];
}

void VnocProtocol::SendVnocMessage(const CMessage *msg, MessageContext *ctx)
{
    ctx->connection->SendVnocMessage(msg);
}
