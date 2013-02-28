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
        IN const Define::MsgDataName& name,
        OUT MsgDataValue*& value) const;

    virtual MsgStatus ReadArr(
        IN  const Define::MsgDataName& name,
        OUT ArrayData*& value) const;

    virtual int MsgId() const;

    virtual MsgStatus Write(
        IN const Define::MsgDataName& name,
        IN MsgDataValue* pValue);

    virtual MsgStatus WriteArr(
        IN const Define::MsgDataName& name,
        IN ArrayData* pValue);

    virtual bool IsValid() const;

    CMessage& Copy(IReadMessage& lhs, int MessageId = 0);

    CMessage& Copy(IReadMessage& lhs,const std::string MessageName);

protected:
    CMessage();

    void InitializeMessage(const std::string& strName);

    void InitializeMessage(int nId);

private:
    void _InitDataMap();
    void _ReleaseMap();

    std::map<Define::MsgDataName, MsgDataValue*> m_mapMsgData;
    std::map<Define::MsgDataName, ArrayData*>    m_mapMsgDataArr;
    XMLObject* m_xmlObject;
};

}// namespace Message
}// namespace VNOC

#endif