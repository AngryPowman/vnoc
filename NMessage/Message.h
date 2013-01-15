#ifndef  VNOC_C_MESSAGE_MSG
#define  VNOC_C_MESSAGE_MSG

#include "CMessage.h"
#include "ParaserMessageXML.h"
#include <string>

namespace VNOC
{
namespace Message
{

class Message:public BaseMessage
{
public:
    Message();
    Message(
        IN const char* strPath);
    virtual ~Message();

    MsgStatus LoadXML(
        IN const char* strPath);

    bool SetMessage(
        IN const std::string& strName);

    bool SetMessage(
        IN int nId);

    MsgStatus Write(
        IN const std::string& strName,
        IN const MsgDataValue& strValue);

    MsgStatus Write(
        IN const std::string& strName,
        IN const ArrayData& strValue);

    MsgStatus Read(
        IN const std::string& strName,
        IN MsgDataValue*& strValue);

    MsgStatus Read(
        IN const std::string& strName,
        IN ArrayData*& strValue);
private:
    XMLObject* m_XML;
    ParserMessageXML m_ParserXML;
};

}// namespace Message
}// namespace VNOC

#endif