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
    int nLen = MSG_HEAD_LEN + MSG_TAIL_LEN; ///> 总大小+msg id
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
            nLen += nAdd + MSG_CLASS_PARAM_COUNT;
            break;
        default:
            return MsgStatus_Err;
        }
    }
    nSize = nLen;
    return MsgStatus_Ok;
}

MsgStatus CMessage2Pack::PackMessage(IReadMessage* pMsg, CBufferMessage& pBuf)
{
    if (!pMsg)
    {
        return MsgStatus_Err;
    }
    int PackSize = 0;
    if (GetPackSize(pMsg, PackSize) != MsgStatus_Ok)
    {
        return MsgStatus_Err;
    }
    uint8* Buff = pBuf.Alloc(PackSize);
    if (Buff == NULL)
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
    int nPackPos = _PackHead(nId,pBuf);
    std::vector<int> vecParamLen;
    std::vector<std::vector<uint8> > vecParamList;
    for (auto It = pXmlObj->ParamBegin(); It != pXmlObj->ParamEnd(); It++)
    {
        switch (It->second.GetMType())
        {
        case MsgDataMType_Data:
            _PushMessageDataParam(It->second, pMsg,It->first, vecParamLen, vecParamList);
            break;
        case MsgDataMType_List:
            _PushMessageListParam(It->second, pMsg,It->first, vecParamLen, vecParamList);
            break;
        default:
            return MsgStatus_Err;
        }
    }
    if (_PackParam(pXmlObj, pBuf, nPackPos, vecParamLen, vecParamList) != MsgStatus_Ok)
    {
        return MsgStatus_Err;
    }
    _PackTail(nPackPos, pBuf);
    return MsgStatus_Ok;
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
            nSize = pReadArr->Size();
            break;
        case MsgDataType_Uint16:
            nSize = 2 * pReadArr->Size();
            break;
        case MsgDataType_Uint32:
            nSize = 4 * pReadArr->Size();
            break;
        }
    }
    return nSize;
}

MsgStatus CMessage2Pack::_PushMessageDataParam(
    XMLItem& item,
    IReadMessage* pMsg,
    std::string ParamName,
    std::vector<int>& vecParamLen,
    std::vector<std::vector<uint8> >& vecParamList
    )
{
    MsgDataValue* pReadValue = NULL;
    std::string strParam;
    uint8* pByteParam = NULL;
    uint8 uint8Param = 0;
    uint16 uint16Param = 0;
    uint32 uint32Param = 0;
    int strParamLen = 0;
    std::vector<uint8> vecParam;
    switch (item.GetType())
    {
    case MsgDataType_String:
        pMsg->Read(ParamName, pReadValue);
        pReadValue->ToStr(strParam);
        strParamLen = strParam.size();
        if (strParamLen == 0)
        {
            strParamLen = 1;
            vecParam.push_back(0);
        }
        else
        {
            pReadValue->ToStr(strParam);
            vecParam.resize(strParamLen);
            std::copy(strParam.begin(), strParam.end(), vecParam.begin());
        }
        vecParamLen.push_back(strParamLen);
        vecParamList.push_back(vecParam);
        break;
    case MsgDataType_Uint8:
        vecParamLen.push_back(sizeof(uint8));
        pMsg->Read(ParamName, pReadValue);
        pReadValue->ToUInt8(uint8Param);
        vecParam.push_back(uint8Param);
        vecParamList.push_back(vecParam);
        break;
    case MsgDataType_Uint16:
        vecParamLen.push_back(sizeof(uint16));
        pMsg->Read(ParamName, pReadValue);
        pReadValue->ToUInt16(uint16Param);
        pByteParam = (uint8*)&uint16Param;
        for (int index = 0; index < sizeof(uint16); index++)
        {
            vecParam.push_back(pByteParam[index]);
        }
        vecParamList.push_back(vecParam);
        break;
    case MsgDataType_Uint32:
        vecParamLen.push_back(sizeof(uint32));
        pMsg->Read(ParamName, pReadValue);
        pReadValue->ToUInt32(uint32Param);
        pByteParam = (uint8*)&uint32Param;
        for (int index = 0; index < sizeof(uint32); index++)
        {
            vecParam.push_back(pByteParam[index]);
        }
        vecParamList.push_back(vecParam);
        break;
    default:
        return MsgStatus_Err;
    }
    return MsgStatus_Ok;
}

MsgStatus CMessage2Pack::_PushMessageListParam(
    XMLItem& item,
    IReadMessage* pMsg,
    std::string ParamName,
    std::vector<int>& vecParamLen,
    std::vector<std::vector<uint8> >& vecParamList
    )
{
    std::vector<uint8> vecParam;
    ArrayData* pReadArrValue = NULL;
    std::string strParam;
    uint8* pByteParam = NULL;
    uint8 uint8Param = 0;
    uint16 uint16Param = 0;
    uint32 uint32Param = 0;
    pMsg->ReadArr(ParamName, pReadArrValue);
    vecParamLen.push_back(pReadArrValue->Size());
    if (pReadArrValue->Empty())
    {
        vecParamLen.push_back(1);
        vecParam.push_back(0);
        return MsgStatus_Ok;
    }

    switch(item.GetType())
    {
    case MsgDataType_String:
        for (auto It = pReadArrValue->Begin();
            It != pReadArrValue->End();
            It++
            )
        {
            static_cast<MsgDataValue*>(*It)->ToStr(strParam);
            vecParam.resize(strParam.size());
            vecParam.push_back(strParam.size());
            std::copy(strParam.begin(), strParam.end(), vecParam.begin());
            vecParamList.push_back(vecParam);
            vecParam.clear();   
        }
        break;
    case MsgDataType_Uint8:
        for (auto It = pReadArrValue->Begin();
            It != pReadArrValue->End();
            It++
            )
        {
            static_cast<MsgDataValue*>(*It)->ToUInt8(uint8Param);
            vecParam.push_back(uint8Param);
            vecParamLen.push_back(sizeof(uint8));
            vecParamList.push_back(vecParam);
            vecParam.clear();
        }
        break;
    case MsgDataType_Uint16:
        for (auto It = pReadArrValue->Begin();
            It != pReadArrValue->End();
            It++
            )
        {
            static_cast<MsgDataValue*>(*It)->ToUInt16(uint16Param);
            pByteParam = (uint8*)&uint16Param;
            for (uint32 index = 0; index < sizeof(uint16); index++)
            {
                vecParam.push_back(pByteParam[index]);
            }
            vecParamLen.push_back(sizeof(uint16));
            vecParamList.push_back(vecParam);
            vecParam.clear();
        }
        break;
    case MsgDataType_Uint32:
        for (auto It = pReadArrValue->Begin();
            It != pReadArrValue->End();
            It++
            )
        {
            static_cast<MsgDataValue*>(*It)->ToUInt32(uint32Param);
            pByteParam = (uint8*)&uint32Param;
            for (uint32 index = 0; index < sizeof(uint32); index++)
            {
                vecParam.push_back(pByteParam[index]);
            }
            vecParamLen.push_back(sizeof(uint32));
            vecParamList.push_back(vecParam);
            vecParam.clear();
        }
        break;
    default:
        return MsgStatus_Err;
    }
    return MsgStatus_Ok;
}

int CMessage2Pack::_PackHead(int PackMsgId, CBufferMessage& pBuf)
{
    int nPackSize = pBuf.GetSize();
    if (nPackSize <= MSG_HEAD_LEN)
    {
        return 0;
    }
    uint32 nPackPos = 0;
    uint8* pByte = NULL;
    uint8* Buff = pBuf.GetBuffer();
    ///>标记头
    Buff[nPackPos] = MSG_BEGIN;
    nPackPos++;
    ///>版本号 暂用一个字节存储
    Buff[nPackPos] = MSG_VER;
    nPackPos++;
    ///>消息ID uint32
    pByte = (uint8*)&PackMsgId;
    for (uint32 index = 0; index < MSG_CLASS_COMMAND; index++)
    {
        Buff[nPackPos] = pByte[index];
        nPackPos++;
    }
    ///>包体大小 uint32
    pByte = (uint8*)&nPackSize;
    for (uint32 index = 0; index < MSG_CLASS_LEN; index++)
    {
        Buff[nPackPos] = pByte[index];
        nPackPos++;
    }
    return nPackPos;
}

int CMessage2Pack::_PackTail(int nPackPos,CBufferMessage& pBuf)
{
    if (nPackPos == 0)
    {
        return 0;
    }
    uint8* Buff = pBuf.GetBuffer();
    if (pBuf.GetSize() < (uint32)(nPackPos + MSG_TAIL_LEN))
    {
        return 0;
    }
    Buff[nPackPos] = MSG_END;
    return nPackPos++;
}

MsgStatus CMessage2Pack::_PackParam(
    XMLObject* pXmlObj,
    CBufferMessage& pBuf,
    int& nPackPos,
    std::vector<int>& vecParamLen,
    std::vector<std::vector<uint8> >& vecParamList
    )
{
    if (pXmlObj == NULL)
    {
        return MsgStatus_Err;
    }
    uint8* Buff = pBuf.GetBuffer();
    if (Buff == NULL)
    {
        return MsgStatus_Err;
    }
    int nPackParamIndex = 0;
    uint8* pByte = NULL;
    uint32 ArrCount = 0;
    for (auto It = pXmlObj->ParamBegin(); It != pXmlObj->ParamEnd(); It++)
    {
        switch (It->second.GetMType())
        {
        case MsgDataMType_Data:
            if (It->second.GetType() == MsgDataType_String)
            {
                pByte = (uint8*)&vecParamLen[nPackParamIndex];
                for (int index = 0; index < sizeof(uint32); index++)
                {
                    Buff[nPackPos] = pByte[index];
                    nPackPos++;
                }
                for (uint32 index = 0; index < vecParamList[nPackParamIndex].size(); index++)
                {
                    Buff[nPackPos] = vecParamList[nPackParamIndex][index];
                    nPackPos++;
                }
            }
            else if (It->second.GetType() == MsgDataType_Uint8)
            {
                Buff[nPackPos] = vecParamList[nPackParamIndex][0];
                nPackPos++;
            }
            else
            {
                for (int index = 0;
                    index < vecParamLen[nPackParamIndex];
                    index++
                    )
                {
                    Buff[nPackPos] = vecParamList[nPackParamIndex][index];
                    nPackPos++;
                }
            }
            nPackParamIndex++;
            break;
        case MsgDataMType_List:
            ///>元素个数
            ArrCount = vecParamLen[nPackParamIndex];
            if (ArrCount < 0)
            {
                ArrCount = 0;
            }
            pByte = (uint8*)&ArrCount;
            for (int index = 0; index < sizeof(uint32); index++)
            {
                Buff[nPackPos] = pByte[index];
                nPackPos++;
            }
            for (uint32 ArrIndex = 0; ArrIndex < ArrCount; ArrIndex++)
            {
                if (It->second.GetType() == MsgDataType_String)
                {
                    pByte = (uint8*)&vecParamLen[nPackParamIndex + 1];
                    for (int index = 0; index < sizeof(uint32); index++)
                    {
                        Buff[nPackPos] = pByte[index];
                        nPackPos++;
                    }
                    for (uint32 index = 0; index < vecParamList[nPackParamIndex].size(); index++)
                    {
                        Buff[nPackPos] = vecParamList[nPackParamIndex][index];
                        nPackPos++;
                    }
                }
                else
                {
                    for (int index = 0;
                        index < vecParamLen[nPackParamIndex + 1];
                        index++
                        )
                    {
                        Buff[nPackPos] = vecParamList[nPackParamIndex][index];
                        nPackPos++;
                    }
                }
                nPackParamIndex++;
            }
            break;
        default:
            return MsgStatus_Err;
        }
    }
    return MsgStatus_Ok;
}

}// namespace Message
}// namespace VNOC