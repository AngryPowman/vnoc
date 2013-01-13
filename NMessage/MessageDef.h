#ifndef  VNOC_MESSAGE_DEF
#define  VNOC_MESSAGE_DEF

#define  IN
#define  OUT

#include <string>
#include <vector>

namespace VNOC
{
namespace Message
{
typedef std::string         MsgDataName;
typedef int                 MsgDataNameInt;
typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
typedef char                int8;
typedef short               int16;
typedef int                 int32;

typedef enum _MsgStatus
{
   MsgStatus_Ok,
   MsgStatus_Err,
   MsgStatus_Unk,
}MsgStatus;

}// namespace Message
}// namespace VNOC

#endif