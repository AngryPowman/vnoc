#include "VnocProtocol.hpp"
#include <assert.h>
void VnocProtocol::RegisterMessageHandler(IMessageHandler *MessageHandler)
{
    assert(MessageHandler != NULL);
    if (MessageHandler == NULL) {return;}
    VMsg type = MessageHandler->getMessageType();
    assert (type > MSG_TYPE_NULL);
    assert (type < MSG_TYPE_END);
    handlerMap_[type].push_back(MessageHandler);
}
void VnocProtocol::RegisterSocketHandlerFactory(SocketHandlerFactory *factory)
{
	handlerFactory_ = factory;
}
list<IMessageHandler *>& VnocProtocol::getHandler(VMsg msgType)
{
    assert (msgType > MSG_TYPE_NULL);
    assert (msgType < MSG_TYPE_END);
    return handlerMap_[msgType];
}

void VnocProtocol::SendVnocMessage(const CMessage *msg, MessageContext *ctx)
{
    ctx->connection->SendVnocMessage(msg);
}
