#pragma once
#ifndef VNOC_PROTOCOL_H
#define VNOC_PROTOCOL_H
#include "VnocMessageSocketHandler.hpp"
#include "../../Message/MsgDef.h"
#include <string>
#include <list>
#include <unordered_map>
#include <array>
using std::list;
using std::array;

class IVnocMessageProtocolHandler;

class IMessageHandler
{
public:
    //if the message is handled, return 1;
    virtual int operator()(const CMessage *msg, MessageContext *ctx)=0;
    virtual MSGTYPE getMessageType() const = 0;
};



class VnocProtocol
{
public:
    SocketHandlerFactory* getSocketHandlerFactory(){return &handlerFactory_;}
    VnocProtocol (){
        handlerFactory_.protocol = this;
    }
    void RegisterMessageHandler(IMessageHandler *MessageHandler);
    void SendVnocMessage(const CMessage *msg, MessageContext *ctx);
    list<IMessageHandler *>& getHandler(MSGTYPE msgType);

private:
    VnocMessageHandlerFactory handlerFactory_;
    array<list<IMessageHandler *>,MSGTYPE::MSGTYPE_END > handlerMap_;
};

#endif //VNOC_PROTOCOL_H
