#include "Message2Parser.h"
#include "ParserMessageXML.h"

namespace VNOC
{
namespace Message
{

bool CMessage2Parser::IsVaild(IN const CBufferMessage& pBuf)
{
    uint8* pData = pBuf.GetBuffer();
    if (pData[0] != MSG_BEGIN)
    {
        return false;
    }

    if (pData[pBuf.GetSize() - 1] != MSG_END)
    {
        return false;
    }
    return true;
}

MsgStatus CMessage2Parser::Parser(
    OUT IWriteMessage* pMsg,
    IN const CBufferMessage& pBuf
    )
{
    if (!IsVaild(pBuf))
    {
        return MsgStatus_FormatFailure;
    }

    if (pBuf.GetSize() == 0)
    {
        return MsgStatus_Err;
    }

    if (_ParserHead(pMsg, pBuf) != MsgStatus_Ok)
    {
        return MsgStatus_FormatFailure;
    }

    if (_ParserParam(pMsg, pBuf) != MsgStatus_Ok)
    {
        return MsgStatus_Err;
    }
    return _ParserTail(pMsg, pBuf);
}

MsgStatus CMessage2Parser::_ParserHead(IWriteMessage* pMsg, const CBufferMessage& pBuf)
{
    uint8* pData = pBuf.GetBuffer();
    uint8 NumByte[4] = {0};
    if (!pData)
    {
        return MsgStatus_Err;
    }

    for (int index = 0; index < MSG_CLASS_COMMAND; index++)
    {
        NumByte[index] = pData[MSG_COMMAND_INDEX + index];
    }
    m_MsgId = _ByteToInt(NumByte);
    memset(NumByte,0,4);
    for (int index = 0; index < MSG_CLASS_LEN; index++)
    {
        NumByte[index] = pData[MSG_PACKSIZE_INDEX + index];
    }
    if (pBuf.GetSize() != _ByteToInt(NumByte))
    {
        return MsgStatus_Err;
    }
    return MsgStatus_Ok;
}

MsgStatus CMessage2Parser::_ParserTail(IWriteMessage* pMsg, const CBufferMessage& pBuf)
{
    uint8* pData = pBuf.GetBuffer();
    if (pData[pBuf.GetSize()] == MSG_END)
    {
        return MsgStatus_Err;
    }
    return MsgStatus_Ok;
}

MsgStatus CMessage2Parser::_ParserParam( IWriteMessage* pMsg, const CBufferMessage& pBuf )
{
    uint8* pData = pBuf.GetBuffer();
    int nParserIndex = MSG_HEAD_LEN;
    std::string strParam;
    int ArrSize = 0;
    std::vector<std::string> vecStrParam;
    std::vector<uint8> vecUint8Param;
    std::vector<uint16> vecUint16Param;
    std::vector<uint32> vecUint32Param;
    ArrayData* pArrData = new ArrayData;
    uint8 NumByte[4] = {0};
    XMLObject* Xml = ParserMessageXML::Instance().GetMsgObject(m_MsgId);
    if (!Xml)
    {
        return MsgStatus_Err;
    }
    for (auto It = Xml->ParamBegin(); It != Xml->ParamEnd(); It++)
    {
        switch (It->second.GetMType())
        {
        case MsgDataMType_Data:
            switch (It->second.GetType())
            {
            case MsgDataType_String:
                for (int index = 0; index < sizeof(uint32); index++)
                {
                    NumByte[index] = pData[nParserIndex];
                    nParserIndex++;
                }
                for (int index = 0; index < _ByteToInt(NumByte); index++)
                {
                    strParam.push_back(pData[nParserIndex]);
                    nParserIndex++;
                }
                pMsg->Write(It->second.GetName(), new StringData(strParam));
                strParam.clear();
                memset(NumByte, 0, 4);
                break;
            case MsgDataType_Uint8:
                pMsg->Write(It->second.GetName(), new NumData<uint8>(pData[nParserIndex]));
                nParserIndex++;
                break;
            case MsgDataType_Uint16:
                for (int index = 0; index < sizeof(uint16); index++)
                {
                    NumByte[index] = pData[nParserIndex];
                    nParserIndex++;
                }
                pMsg->Write(It->second.GetName(), new NumData<uint16>(_ByteToInt(NumByte)));
                memset(NumByte, 0, 4);
                break;
            case MsgDataType_Uint32:
                for (int index = 0; index < sizeof(uint32); index++)
                {
                    NumByte[index] = pData[nParserIndex];
                    nParserIndex++;
                }
                pMsg->Write(It->second.GetName(), new NumData<uint32>(_ByteToInt(NumByte)));
                memset(NumByte, 0, 4);
                break;
            }
            break;
        case MsgDataMType_List:
            for (int index = 0; index < sizeof(uint32); index++)
            {
                NumByte[index] = pData[nParserIndex];
                nParserIndex++;
            }
            ArrSize = _ByteToInt(NumByte);
            memset(NumByte, 0, 4);

            for (int index = 0; index < ArrSize; index++)
            {
                if (It->second.GetType() == MsgDataType_String)
                {
                    for (int index = 0; index < sizeof(uint32); index++)
                    {
                        NumByte[index] = pData[nParserIndex];
                        nParserIndex++;
                    }
                    for (int index = 0; index < _ByteToInt(NumByte); index++)
                    {
                        strParam.push_back(pData[nParserIndex]);
                        nParserIndex++;
                    }
                    vecStrParam.push_back(strParam);
                    strParam.clear();
                    memset(NumByte, 0, 4);
                }
                else if (It->second.GetType() == MsgDataType_Uint8)
                {
                    vecUint8Param.push_back(pData[nParserIndex]);
                    nParserIndex++;
                }
                else if (It->second.GetType() == MsgDataType_Uint16)
                {
                    for (int index = 0; index < sizeof(uint16); index++)
                    {
                        NumByte[index] = pData[nParserIndex];
                        nParserIndex++;
                    }
                    vecUint16Param.push_back(_ByteToInt(NumByte));
                    memset(NumByte, 0, 4);
                }
                else if (It->second.GetType() == MsgDataType_Uint32)
                {
                    for (int index = 0; index < sizeof(uint32); index++)
                    {
                        NumByte[index] = pData[nParserIndex];
                        nParserIndex++;
                    }
                    vecUint32Param.push_back(_ByteToInt(NumByte));
                    memset(NumByte, 0, 4);
                }
            }
            if (!vecStrParam.empty())
            {
                pArrData->Push(vecStrParam);
                vecStrParam.clear();
            }
            if (!vecUint8Param.empty())
            {
                pArrData->Push(vecUint8Param);
                vecUint8Param.clear();
            }
            if (!vecUint16Param.empty())
            {
                pArrData->Push(vecUint16Param);
                vecUint16Param.clear();
            }
            if (!vecUint32Param.empty())
            {
                pArrData->Push(vecUint32Param);
                vecUint32Param.clear();
            }
            pMsg->WriteArr(It->second.GetName(), pArrData);
            break;
        default:
            return MsgStatus_Err;
        }
    }
    return MsgStatus_Ok;
}

int CMessage2Parser::_ByteToInt(uint8* pData)
{
    uint8 ByteData[4] = {0};
    memcpy(ByteData, pData, 4);
    int retint = *(int*)&ByteData;
    return retint;
}

int CMessage2Parser::GetMsgType()
{
    return m_MsgId;
}

}
}