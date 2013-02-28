#ifndef VNOC_PRODUCE_DISPOSE
#define VNOC_PRODUCE_DISPOSE

#include "../../../NMessage/ParserMessageXML.h"
#include "MessageFile.h"
#include <string>
#include <vector>

#define MESSAGE_TAMPLATE_NAME_TAG "?Name"
#define MESSAGE_TAMPLATE_ID_TAG "?Id"
#define MESSAGE_TAMPLATE_REGISTERPORT_TAG "?RegisterPort"
#define MESSAGE_TAMPLATE_REGISTERPORT "RegisterPort"
#define MESSAGE_TAMPLATE_PARAM_TAG_SET "?SetParam"
#define MESSAGE_TAMPLATE_PARAM_TAG_GET "?GetParam"

using namespace VNOC::Message::Define;

namespace VNOC
{
namespace Message
{

void ConvertTypeToStr(int IdType, std::string& strType)
{
    switch (IdType)
    {
    case VNOC::Message::MsgDataType_String:
        strType = "std::string";
        break;
    case VNOC::Message::MsgDataType_Uint8:
        strType = "uint8";
        break;
    case VNOC::Message::MsgDataType_Uint16:
        strType = "uint16";
        break;
    case VNOC::Message::MsgDataType_Uint32:
        strType = "uint32";
        break;
    default:
        strType = "void";
    }
}

void ConvertTypeToMsgDataValue(int IdType, std::string& strType)
{
    switch (IdType)
    {
    case VNOC::Message::MsgDataType_String:
        strType = "StringData";
        break;
    case VNOC::Message::MsgDataType_Uint8:
        strType = "NumData<Define::uint8>";
        break;
    case VNOC::Message::MsgDataType_Uint16:
        strType = "NumData<Define::uint16>";
        break;
    case VNOC::Message::MsgDataType_Uint32:
        strType = "NumData<Define::uint32>";
        break;
    default:
        strType = "void";
    }
}

void ConvertToStyle(int IdType, std::string& strType)
{
    switch (IdType)
    {
    case VNOC::Message::MsgDataType_String:
        strType = "ToStr";
        break;
    case VNOC::Message::MsgDataType_Uint8:
        strType = "ToUInt8";
        break;
    case VNOC::Message::MsgDataType_Uint16:
        strType = "ToUInt16";
        break;
    case VNOC::Message::MsgDataType_Uint32:
        strType = "ToUInt32";
        break;
    default:
        strType = "void";
    }
}

void ConvertTypeToIdName(int IdType, std::string& strType)
{
    switch (IdType)
    {
    case VNOC::Message::MsgDataType_String:
        strType = "MsgDataType_String";
        break;
    case VNOC::Message::MsgDataType_Uint8:
        strType = "MsgDataType_Uint8";
        break;
    case VNOC::Message::MsgDataType_Uint16:
        strType = "MsgDataType_Uint16";
        break;
    case VNOC::Message::MsgDataType_Uint32:
        strType = "MsgDataType_Uint32";
        break;
    default:
        strType = "0";
    }
}

void ConvertMTypeToIdName(int IdType, std::string& strType)
{
    switch (IdType)
    {
    case VNOC::Message::MsgDataMType_List:
        strType = "MsgDataMType_List";
        break;
    case VNOC::Message::MsgDataMType_Data:
        strType = "MsgDataMType_Data";
        break;
    default:
        strType = "0";
    }
}

void DisposeName(
    const std::string& strTamplate,
    std::string& strSrc,
    const std::string& strName,
    const std::string& strValue,
    bool Isall
    )
{
    int TagPos = strTamplate.find(strName);
    if (TagPos == std::string::npos)
    {
        return;
    }
    int NameLen = strName.length();
    std::string strFirst, strSecond;
    strFirst.resize(TagPos);
    strSecond.resize(strTamplate.length() - (TagPos + NameLen));
    std::copy(strTamplate.begin(), strTamplate.begin() + TagPos, strFirst.begin());
    std::copy(strTamplate.begin() + (TagPos + NameLen), strTamplate.end(), strSecond.begin());
    strSrc.clear();
    strSrc += strFirst;
    strSrc += strValue;
    strSrc += strSecond;
    //µÝ¹éÌæ»»
    if (Isall == true)
    {
        DisposeName(strSrc, strSrc, strName, strValue, Isall);
    }
}

void DisposePort(
    VNOC::Message::XMLItem* Item,
    std::string& strParam
    )
{
    std::string strType, strMType;
    ConvertTypeToIdName(Item->GetType(), strType);
    ConvertMTypeToIdName(Item->GetMType() , strMType);
    strParam += MESSAGE_TAMPLATE_REGISTERPORT;
    strParam += "(\"";
    strParam += Item->GetName();
    strParam += "\", ";
    strParam += strMType;
    strParam += ", ";
    strParam += strType;
    strParam += ");";
};

void DisposeParam(
    VNOC::Message::XMLItem* Item,
    const std::string& strPattern,
    std::string& strParam
    )
{
    strParam = "    ";
    strParam += "MsgStatus ";
    strParam += strPattern;
    strParam += Item->GetName();
    strParam += "(";
    if (strPattern == "Set")
    {
        strParam += "const ";
    }
    std::string strParamType;
    if (Item->GetMType() == VNOC::Message::MsgDataMType_Data)
    {
        ConvertTypeToStr(Item->GetType(), strParamType);
        if (strParamType != "std::string")
        {
            strParam += "Define::" + strParamType;
        }
        else
        {
            strParam += strParamType;
        }
    }
    else if (Item->GetMType() == VNOC::Message::MsgDataMType_List)
    {
        ConvertTypeToStr(Item->GetType(), strParamType);
        strParam += "std::vector<";
        if (strParamType != "std::string")
        {
            strParam += "Define::" + strParamType;
        }
        else
        {
            strParam += strParamType;
        }
        strParam += ">";
    }
    strParam += "& ";
    strParam += "Value";
    if (strPattern == "Set")
    {
        strParam += ")";
    }
    else
    {
        strParam += ") const";
    }
    strParam += "\n";
    strParam += "    ";
    strParam += "{";
    strParam += "\n";
    strParam += "        ";
    if (strPattern == "Get")
    {
        std::string strParamToStyle;
        if (Item->GetMType() == VNOC::Message::MsgDataMType_Data)
        {
            strParam += "MsgDataValue* pReadValue = NULL;";
            strParam += "\n        ";
            strParam += "Read(";
        }
        else if (Item->GetMType() == VNOC::Message::MsgDataMType_List)
        {
            strParam += "ArrayData* pReadValueArr = NULL;";
            strParam += "\n        ";
            strParam += "ReadArr(";
        }
        strParam += "\"";
        strParam += Item->GetName();
        strParam += "\"";
        strParam += ", ";
        if (Item->GetMType() == VNOC::Message::MsgDataMType_Data)
        {
            ConvertToStyle(Item->GetType(),strParamToStyle);
            strParam += "pReadValue);\n";
            strParam += "        ";
            strParam += "return ";
            strParam += "pReadValue->";
            strParam += strParamToStyle;
            strParam += "(Value);";
        }
        else if (Item->GetMType() == VNOC::Message::MsgDataMType_List)
        {
            ConvertTypeToStr(Item->GetType(), strParamType);
            strParam += "pReadValueArr);\n        ";
            strParam += "pReadValueArr->GetArr_vec<";
            if (strParamType != "std::string")
            {
                strParam += "Define::" + strParamType;
            }
            else
            {
                strParam += strParamType;
            }
            strParam += ">(Value);\n";
            strParam += "        ";
            strParam += "return MsgStatus_Ok;";
        }
    }
    else if (strPattern == "Set")
    {
        if (Item->GetMType() == VNOC::Message::MsgDataMType_Data)
        {
            strParam += "return ";
            strParam += "Write(";
        }
        else if (Item->GetMType() == VNOC::Message::MsgDataMType_List)
        {
            ConvertTypeToStr(Item->GetType(), strParamType);
            strParam += "ArrayData* ValueArr = new ArrayData;";
            strParam += "\n        ";
            strParam += "ValueArr->Push<";
            if (strParamType != "std::string")
            {
                strParam += "Define::" + strParamType;
            }
            else
            {
                strParam += strParamType;
            }
            strParam += ">(Value);";
            strParam += "\n        ";
            strParam += "return ";
            strParam += "WriteArr(";
        }
        strParam += "\"";
        strParam += Item->GetName();
        strParam += "\"";
        strParam += ", ";
        if (Item->GetMType() == VNOC::Message::MsgDataMType_Data)
        {
            std::string strMsgDataValueType;
            strParam += "new ";
            ConvertTypeToMsgDataValue(Item->GetType(), strMsgDataValueType);
            strParam += strMsgDataValueType;
            strParam += "(Value)";
        }
        else if (Item->GetMType() == VNOC::Message::MsgDataMType_List)
        {
            strParam += "ValueArr";
        }
        strParam += ");";
    }
    strParam += "\n";
    strParam += "    ";
    strParam += "}";
}

bool ProduceFile(const char* lpTargetPath, const std::string& strBuffer)
{
    if (!lpTargetPath)
    {
        return false;
    }
    VNOC::Message::CMessageFile file;
    if (file.Open(lpTargetPath, VNOC::Message::UNIN_F_ATO_W))
    {
        return false;
    }
    //UTF8 ±ê¼ÇÍ· 0xEFBBBF
    uint8 UTF8Head[3] = {0};
    UTF8Head[0] = 0xEF;
    UTF8Head[1] = 0xBB;
    UTF8Head[2] = 0xBF;
    if (file.Write((void*)UTF8Head, 1, 3) < 0)
    {
        return false;
    }
    if (file.Write((void*)strBuffer.c_str(), 1, strBuffer.length()) < 0)
    {
        return false;
    }
    return true;
}

}
}

#endif