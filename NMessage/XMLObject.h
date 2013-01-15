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

    std::string GetMType() { return m_strMType;}

    std::string GetType() { return m_strType;}

    std::string GetName() { return m_strName;}

    void SetMType(
        std::string strValue
        )
    {
        m_strMType = strValue;
    }

    void SetType(
        std::string strValue
        )
    {
        m_strType = strValue;
    }

    void SetName(
        std::string strValue
        )
    {
        m_strName = strValue;
    }
private:
    std::string m_strName;
    std::string m_strMType;
    std::string m_strType;
};

class XMLObject
{
public:
    XMLObject(const char* pName, int nId);

    XMLObject(const XMLObject& obj);

    virtual ~XMLObject(){}

    std::string GetName() { return m_strName;}

    int GetId() { return m_Id;}

    XMLItem* GetItem(std::string strName);

    MsgStatus SetItem(std::string strName, XMLItem itemObject);

private:
    int m_Id;
    std::string m_strName;
    std::map<std::string, XMLItem> m_mapParamList;
};

}
}

#endif