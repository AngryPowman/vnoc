#ifndef VNOC_C_MESSAGE_PARSERXML
#define VNOC_C_MESSAGE_PARSERXML

#include <string>
#include <vector>
#include <map>
#include <tchar.h>
#include "../TinyXML/tinyxml.h"
#include "MessageDef.h"

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
private:
    void _Parser();
private:
    TiXmlDocument m_xmlTiny;
    std::string m_RootName;
    std::map<std::string,std::vector<std::string> > m_MsgObjectList;
    std::vector<std::string> m_NameList;
    std::vector<std::string> m_IdList;
    std::vector<std::string> m_ItmeNameList;
    std::vector<std::string> m_ItmeTypeOList;
    std::vector<std::string> m_ItmeTypeTList;
};

}// namespace Message
}// namespace VNOC

#endif