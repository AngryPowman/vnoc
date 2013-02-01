#ifndef  VNOC_C_MESSAGE_BASE
#define  VNOC_C_MESSAGE_BASE

#include "IBaseMessage.h"
#include <map>
#include "XMLObject.h"

namespace VNOC
{
namespace Message
{

class CMessage : public IMessage
{
public:
    CMessage(const std::string& strName);
    CMessage(int nId);
    virtual ~CMessage();

    virtual MsgStatus Read(
        IN const MsgDataName& name,
        OUT MsgDataValue*& value);

    virtual MsgStatus ReadArr(
        IN  const MsgDataName& name,
        OUT ArrayData*& value);

    virtual int MsgId();

    virtual MsgStatus Write(
        IN const MsgDataName& name,
        IN MsgDataValue* pValue);

    virtual MsgStatus WriteArr(
        IN const MsgDataName& name,
        IN ArrayData* pValue);

    virtual bool IsValid();

protected:
    CMessage();

    void InitializeMessage(const std::string& strName);

    void InitializeMessage(int nId);

private:
    void _InitDataMap();
    void _ReleaseMap();

    std::map<MsgDataName, MsgDataValue*> m_mapMsgData;
    std::map<MsgDataName, ArrayData*>    m_mapMsgDataArr;
    XMLObject* m_xmlObject;
};

}// namespace Message
}// namespace VNOC

#endif