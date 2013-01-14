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
    ParserMessageXML(){}
    virtual ~ParserMessageXML(){}

    MsgStatus LoadFile( const char* strPath );

    void SetObject(std::string strName,std::string strValue);

    XMLObject* GetOjbect(std::string strName);
private:
    void _Parser();
private:
    TiXmlDocument m_xmlTiny;
    std::string m_RootName;
    std::map<std::string,std::string > m_MsgIdList;
    std::map<std::string,XMLObject > m_MsgObjectList;
    std::vector<std::string> m_NameList;
    std::vector<std::string> m_IdList;
};

}// namespace Message
}// namespace VNOC

#endif