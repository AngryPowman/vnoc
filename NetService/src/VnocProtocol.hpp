#pragma once
#ifndef VNOC_PROTOCOL_H
#define VNOC_PROTOCOL_H

#include "../../NMessage/MessageUnion.h"
#include <string>
#include <list>
#include <unordered_map>
#include <array>
#include "SocketHandler.hpp"

using std::list;
using std::array;
using namespace VNOC::Message;

class IVnocMessageProtocolHandler
{
public:
    virtual void SendVnocMessage(const CMessage *msg) = 0;
};

struct MessageContext
{
    IVnocMessageProtocolHandler *connection; 
    std::string userName;
};

class IMessageHandler
{
public:
    //if the message is handled, return 1;
    virtual int operator()(const CMessage *msg, MessageContext *ctx)=0;
    virtual VMsg getMessageType() const = 0;
};

class VnocProtocol
{
public:
    SocketHandlerFactory* getSocketHandlerFactory(){return handlerFactory_;}
    VnocProtocol (){
    }
    void RegisterMessageHandler(IMessageHandler *MessageHandler);
	void RegisterSocketHandlerFactory(SocketHandlerFactory *factory);
    void SendVnocMessage(const CMessage *msg, MessageContext *ctx);
    list<IMessageHandler *>& getHandler(VMsg msgType);

private:
    SocketHandlerFactory *handlerFactory_;
    array<list<IMessageHandler *>, VMsg::MSG_TYPE_END > handlerMap_;
};

#endif //VNOC_PROTOCOL_H
