#ifndef VNOC_PRODUCE_DISPOSE
#define VNOC_PRODUCE_DISPOSE

#include "../../../NMessage/ParaserMessageXML.h"
#include "MessageFile.h"
#include <string>
#include <vector>

#define MESSAGE_TAMPLATE_NAME_TAG "?Name"
#define MESSAGE_TAMPLATE_PARAM_TAG_SET "?SetParam"
#define MESSAGE_TAMPLATE_PARAM_TAG_GET "?GetParam"

namespace VNOC
{
namespace Message
{

void ConvertTypeToStr(int IdType, std::string& strType)
{
    switch (IdType)
    {
    case VNOC::Message::MsgDataType_String:
        strType = "StringData";
        break;
    case VNOC::Message::MsgDataType_Uint8:
        strType = "UInt8Data";
        break;
    case VNOC::Message::MsgDataType_Uint16:
        strType = "UInt16Data";
        break;
    case VNOC::Message::MsgDataType_Uint32:
        strType = "UInt32Data";
        break;
    default:
        strType = "void";
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
    if (Item->GetMType() == VNOC::Message::MsgDataMType_Data)
    {
        std::string strParamType;
        ConvertTypeToStr(Item->GetType(), strParamType);
        strParam += strParamType;
    }
    else if (Item->GetMType() == VNOC::Message::MsgDataMType_List)
    {
        strParam += "ArrayData";
    }
    if (strPattern == "Get")
    {
        strParam += "*& ";
    }
    else
    {
        strParam += "& ";
    }
    strParam += "Value";
    strParam += ")";
    strParam += "\n";
    strParam += "    ";
    strParam += "{";
    strParam += "\n";
    strParam += "        ";
    strParam += "return ";
    if (strPattern == "Get")
    {
        if (Item->GetMType() == VNOC::Message::MsgDataMType_Data)
        {
            strParam += "Read(";
        }
        else if (Item->GetMType() == VNOC::Message::MsgDataMType_List)
        {
            strParam += "ReadArr(";
        }
    }
    else if (strPattern == "Set")
    {
        if (Item->GetMType() == VNOC::Message::MsgDataMType_Data)
        {
            strParam += "Write(";
        }
        else if (Item->GetMType() == VNOC::Message::MsgDataMType_List)
        {
            strParam += "WriteArr(";
        }
    }
    strParam += "\"";
    strParam += Item->GetName();
    strParam += "\"";
    strParam += ", ";
    if (strPattern == "Set")
    {
        strParam += "&";
    }
    strParam += "Value";
    strParam += ");";
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