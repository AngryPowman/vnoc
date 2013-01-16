#include "Message2Pack.h"
#include "ParaserMessageXML.h"

namespace VNOC
{
namespace Message
{

CMessage2Pack::CMessage2Pack()
{

}

CMessage2Pack::~CMessage2Pack()
{

}

/************************************************************************

Pack的内存结构为：

头部： | 总大小：uint32 | msg id：uint32 |
接下来为所有data（按xml定义的顺序）
uint： | uint8 | uint16 | uint32 |
string： | 长度 : uint32 | 字符串 |
array： | 元素个数 : uint32 | 数据s（按前面所述）

************************************************************************/

MsgStatus CMessage2Pack::GetPackSize( IReadMessage* pMsg, int& nSize )
{
    if (!pMsg)
    {
        return MsgStatus_Err;
    }
    int nId = pMsg->MsgId();
    if (nId == 0)
    {
        return MsgStatus_Err;
    }
    XMLObject* pXmlObj = ParserMessageXML::Instance().GetMsgObject(nId);
    if (!pXmlObj)
    {
        return MsgStatus_Err;
    }
    int nLen = 8; ///> 总大小+msg id
    int nAdd = 0;
    MsgDataValue* pReadStr = NULL;
    auto it = pXmlObj->ParamBegin();
    for (; it != pXmlObj->ParamEnd(); ++it)
    {
        switch (it->second.GetMType())
        {
        case MsgDataMType_Data:
            nAdd = _GetDataSize(it->second, pMsg);
            if (nAdd == 0)
            {
                return MsgStatus_Err;
            }
            nLen += nAdd;
            break;
        case MsgDataMType_List:
            nAdd = _GetArrSize(it->second, pMsg);
            if (nAdd == 0)
            {
                return MsgStatus_Err;
            }
            nLen += nAdd;
            break;
        default:
            return MsgStatus_Err;
        }
    }
    nSize = nLen;
    return MsgStatus_Ok;
}

MsgStatus CMessage2Pack::PackMessage( IReadMessage* pMsg, char* pBuf)
{
    return MsgStatus_Err;
}

int CMessage2Pack::_GetDataSize( XMLItem& item, IReadMessage* pMsg )
{
    MsgDataValue* pReadStr = NULL;
    switch (item.GetType())
    {
    case MsgDataType_String:
        if (pMsg->Read(item.GetName(), pReadStr) == MsgStatus_Ok)
            return _GetStringSize(item, pReadStr);
        break;
    case MsgDataType_Uint8:
        return 1;
        break;
    case MsgDataType_Uint16:
        return 2;
        break;
    case MsgDataType_Uint32:
        return 4;
        break;
    }
    return 0;
}

int CMessage2Pack::_GetStringSize( XMLItem& item, MsgDataValue* pReadStr )
{
    std::string strData;
    if (pReadStr && pReadStr->ToStr(strData) == MsgStatus_Ok)
    {
        return 4 + strData.size();
    }
    return 0;
}

int CMessage2Pack::_GetArrSize( XMLItem& item, IReadMessage* pMsg )
{
    int nSize = 0;
    int nAdd = 0;
    ArrayData* pReadArr = NULL;
    if (pMsg->ReadArr(item.GetName(), pReadArr) == MsgStatus_Ok)
    {
        switch (item.GetType())
        {
        case MsgDataType_String:
            for (std::vector<MsgDataValue*>::const_iterator it = pReadArr->Begin();
                it != pReadArr->End();
                ++it)
            {
                nAdd = _GetStringSize(item, static_cast<MsgDataValue*>(*it));
                if (nAdd == 0)
                {
                    return 0;
                }
                nSize += nAdd;
            }
            break;
        case MsgDataType_Uint8:
            nSize = 5 * pReadArr->Size();
            break;
        case MsgDataType_Uint16:
            nSize = 6 * pReadArr->Size();
            break;
        case MsgDataType_Uint32:
            nSize = 8 * pReadArr->Size();
            break;
        }
    }
    return nSize;
}

}// namespace Message
}// namespace VNOC
