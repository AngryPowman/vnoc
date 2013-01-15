#ifndef VNOC_C_MESSAG_XMLOBJECT
#define VNOC_C_MESSAG_XMLOBJECT

#include <map>
#include <string>
#include <vector>
#include "MessageDef.h"

namespace VNOC
{
namespace Message
{

class XMLItem 
{
public:
    XMLItem(){}
    virtual ~XMLItem(){}

    MsgDataMType GetMType();

    MsgDataType GetType();

    std::string GetName();

    void SetMType(
        IN MsgDataMType strValue);

    void SetType(
        IN MsgDataType strValue);

    void SetName(
        IN const std::string& strValue);
private:
    std::string m_strName;
    MsgDataMType m_strMType;
    MsgDataType m_strType;
};

class XMLObject
{
public:
    XMLObject(const char* pName, int nId);

    XMLObject(const XMLObject& obj);

    virtual ~XMLObject();

    std::string GetName();

    int GetId();

    XMLItem* GetItem(
        IN const std::string& strName);

    MsgStatus SetItem(
        IN const std::string& strName,
        IN XMLItem itemObject);
private:
    int m_Id;
    std::string m_strName;
    std::map<std::string, XMLItem> m_mapParamList;
};

}// namespace Message
}// namespace VNOC

#endif