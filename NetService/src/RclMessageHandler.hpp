#pragma once
#include "VnocProtocol.hpp"
//#include "UserManage.hpp"
#include "Room.h"

class RclMessageHandler : public IMessageHandler
{
public:
    RclMessageHandler(VnocProtocol *protocol):protocol_(protocol)
    {
        protocol_->RegisterMessageHandler(this);
    }
    virtual VMsg getMessageType() const
    {
        return MSG_RequestClassList_Id;
    }
    virtual int operator()(const CMessage *msg, MessageContext *ctx)
    {
        std::cout << "handle RclMessageHandler" << endl;
        MSG_AnswerClassList aclMessage;
        MSG_RequestClassList rclMessage(*msg);

        //temporary room data
        std::vector<uint32> roomList;
        std::vector<std::string> roomNameList;
        std::vector<uint32> roomStateList;
        //roomList.resize(100);
        for (uint32 i = 0; i < 200; ++i)
        {
            //Room& room = roomList[i];
            //room.setRoomID(10000 + i);
            //room.setPoolObjId(i);
            //room.setIsValid(true);
            roomList.push_back(10000 + i);
            roomNameList.push_back("!!!");
            roomStateList.push_back(i);
        }

        aclMessage.SetRoomIdList(roomList);
        aclMessage.SetRoomNameList(roomNameList);
        aclMessage.SetRoomStateList(roomStateList);
        protocol_->SendVnocMessage(&aclMessage, ctx);
        return 1;
    }
private:
    VnocProtocol *protocol_;
};