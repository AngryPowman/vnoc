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
    virtual MSGTYPE getMessageType() const
    {
        return MSG_RCL_TYPE;
    }
    virtual int operator()(const CMessage *msg, MessageContext *ctx)
    {
        std::cout << "handle RclMessageHandler" << endl;
        MSG_ACL aclMessage;
        const MSG_RCL * rclMessage = dynamic_cast<const MSG_RCL *>(msg);

        //temporary room data
        std::vector<int32> roomList;
        //roomList.resize(100);
        for (uint32 i = 0; i < roomList.size(); ++i)
        {
            //Room& room = roomList[i];
            //room.setRoomID(10000 + i);
            //room.setPoolObjId(i);
            //room.setIsValid(true);
            roomList.push_back(10000 + i);
        }

        aclMessage.SetRoomList(roomList);
        
        protocol_->SendVnocMessage(&aclMessage, ctx);
        return 1;
    }
private:
    VnocProtocol *protocol_;
};