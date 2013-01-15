#ifndef VNOC_C_MESSAGE_PARSERXML
#define VNOC_C_MESSAGE_PARSERXML

#include <string>
#include <vector>
#include <map>
#include <tchar.h>
#include "../TinyXML/tinyxml.h"
#include "MessageDef.h"
#include "XMLObject.h"

namespace VNOC
{
namespace Message
{

class ParserMessageXML
{
public:
    ParserMessageXML();
    virtual ~ParserMessageXML();

    MsgStatus LoadFile(const char* strPath);

    XMLObject* GetMsgObject(const std::string& strName);
    XMLObject* GetMsgObject(int nId);
private:
    bool _Parser();
private:
    TiXmlDocument m_xmlTiny;
    std::map<std::string, int> m_MsgIdList;
    std::map<int, XMLObject> m_MsgObjectList;
};

}// namespace Message
}// namespace VNOC

#endif