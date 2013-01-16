#ifndef  VNOC_C_MESSAGE_BASE
#define  VNOC_C_MESSAGE_BASE

#include "IBaseMessage.h"
#include "ParaserMessageXML.h"
#include <map>

namespace VNOC
{
namespace Message
{

class CMessage : public IMessage
{
public:
    CMessage();
    virtual ~CMessage();

    bool SetMessage(
        IN const std::string& strName,
        IN ParserMessageXML& parserXML);

    bool SetMessage(
        IN int nId,
        IN ParserMessageXML& parserXML);

    virtual MsgStatus Read(
        IN const MsgDataName& name,
        OUT MsgDataValue*& value);

    virtual MsgStatus ReadArr(
        IN  const MsgDataName& name,
        OUT ArrayData*& value);

    virtual MsgStatus Write(
        IN const MsgDataName& name,
        IN const MsgDataValue& value);

    virtual MsgStatus WriteArr(
        IN const MsgDataName& name,
        IN const ArrayData& value);

private:
    std::map<MsgDataName, MsgDataValue*> m_mapMsgData;
    std::map<MsgDataName, ArrayData*>    m_mapMsgDataArr;
    XMLObject* m_xmlObject;
};

}// namespace Message
}// namespace VNOC

#endif