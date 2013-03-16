#include <iostream>
#include "Dispose.hpp"

#define MAX_ENUM_LEN 46

bool DisposeMessage(
    const char* lpszXMLPtah,
    const char* lpTargetPath,
    const char* lpszTamplatePath);

int main(int argc, char **argv)
{
    int oCommand = 0;
    char* lpszTamplatePath = NULL;
    if (argc > 0)
    {
        if ((argv[1][0] == '-') && (argv[1][1] == 'h'))
        {
                std::cout<<">>>>>>>>>> command [-p] pathXml targetpath"<<std::endl;
        }
        if ((argv[1][0] == '-') && (argv[1][1] == 't'))
        {
            lpszTamplatePath = argv[2];
        }
        if ((argv[1][0] == '-') && (argv[1][1] == 'p'))
        {
            if (!argv[2] || !argv[3])
            {
                std::cout<<">>>>>>>>>> Path Err"<<std::endl;
                return 0;
            }
            DisposeMessage(argv[2], argv[3],lpszTamplatePath);
        }
    }
    return 0;
}

bool DisposeMessage(
    const char* lpszXMLPtah,
    const char* lpTargetPath,
    const char* lpszTamplatePath
    )
{
    VNOC::Message::CMessageFile file;
    std::string strTamplatePath;
    if (!lpszXMLPtah)
    {
        return false;
    }
    if (!lpszTamplatePath)
    {
        strTamplatePath = "NMsgTamplate.msg";
    }
    else
    {
        strTamplatePath = lpszTamplatePath;
    }
    if (file.Open(strTamplatePath.c_str(), VNOC::Message::UNIN_F_R) == -1)
    {
        return false;
    }
    std::string strTamplate;
    std::vector<std::string> vecUnion;
    std::vector<unsigned int> vecUnionId;
    char* lpszTamplate = new char[file.GetFileSize() + 1];
    memset(lpszTamplate, 0, file.GetFileSize() + 1);
    file.Read(lpszTamplate, 1, file.GetFileSize());
    strTamplate = lpszTamplate;
    if (VNOC::Message::ParserMessageXML::Instance().LoadFile(lpszXMLPtah) == VNOC::Message::MsgStatus_Ok)
    {
        for (auto Itr = VNOC::Message::ParserMessageXML::Instance().ObjectBegin();
            Itr != VNOC::Message::ParserMessageXML::Instance().ObjectEnd();
            Itr++)
        {
            std::string strParamPort;
            std::string strGetParam, strSetParam;
            VNOC::Message::XMLObject* obj = &(Itr->second);
            std::string strSrc;
            bool TopLine = false;
            VNOC::Message::DisposeName(strTamplate, strSrc, MESSAGE_TAMPLATE_NAME_TAG, Itr->second.GetName(), true);
            VNOC::Message::DisposeName(strSrc, strSrc, MESSAGE_TAMPLATE_ID_TAG, Itr->second.GetName() + ("_Id"), true);
            for (auto ItrItem = obj->ParamBegin(); ItrItem != obj->ParamEnd(); ItrItem++)
            {
                std::string strTmp;
                VNOC::Message::DisposePort(&ItrItem->second, strTmp);
                if (TopLine != false)
                {
                    strParamPort += "\n";
                    strParamPort += "\n";
                    strParamPort += "        ";
                }
                strParamPort += strTmp;
                VNOC::Message::DisposeParam(&ItrItem->second, "Get", strTmp);
                if (TopLine != false)
                {
                    strGetParam += "\n";
                    strGetParam += "\n";
                }
                strGetParam += strTmp;
                VNOC::Message::DisposeParam(&ItrItem->second, "Set", strTmp);
                if (TopLine != false)
                {
                    strSetParam += "\n";
                    strSetParam += "\n";
                }
                strSetParam += strTmp;
                TopLine = true;
            }
            strParamPort += "\n";
            VNOC::Message::DisposeName(strSrc, strSrc, MESSAGE_TAMPLATE_REGISTERPORT_TAG, strParamPort, true);
            VNOC::Message::DisposeName(strSrc, strSrc, MESSAGE_TAMPLATE_PARAM_TAG_SET, strSetParam, false);
            VNOC::Message::DisposeName(strSrc, strSrc, MESSAGE_TAMPLATE_PARAM_TAG_GET, strGetParam, false);
            std::string strTargetPath = lpTargetPath;
            if (!strTargetPath.empty())
            {
                strTargetPath += "/";
                strTargetPath += obj->GetName();
                strTargetPath += ".h";
                vecUnion.push_back(obj->GetName());
                vecUnionId.push_back(obj->GetId());
                if (VNOC::Message::ProduceFile(strTargetPath.c_str(), strSrc))
                {
                    std::cout<<">>>>>>>>>> Produse ------->    "<<obj->GetName()<<std::endl;
                }
            }
        }
    }

    std::string strUnionPath = lpTargetPath;
    std::string strUnionDefPath = lpTargetPath;
    std::string strUnionFunDefPath = lpTargetPath;
    strUnionPath += "/MessageUnion.h";
    std::string strUninBuff;
    strUninBuff = "#ifndef VNOC_D_MESSSAGE_UNION\n";
    strUninBuff += "#define VNOC_D_MESSSAGE_UNION\n\n";
    for (auto It = vecUnion.begin(); It != vecUnion.end(); It++)
    {
        strUninBuff += "#include \"" + (*It) + ".h \"\n";
    }
    strUninBuff += "#include \"MessageUnionDef.h \"\n";
    strUninBuff += "\n#endif";
    if (VNOC::Message::ProduceFile(strUnionPath.c_str(), strUninBuff))
    {
        std::cout<<">>>>>>>>>> Produse ------->    MessageUnion.h"<<std::endl;
    }

    strUnionDefPath += "/MessageUnionDef.h";
    std::string strUninBuffDef;
    strUninBuffDef = "#ifndef VNOC_D_MESSSAGE_UNION_DEF\n";
    strUninBuffDef += "#define VNOC_D_MESSSAGE_UNION_DEF\n\n";
    strUninBuffDef += "namespace VNOC\n{\n";
    strUninBuffDef += "namespace Message\n{\n\n";
    strUninBuffDef += "typedef enum _VMsg\n{\n";
    strUninBuffDef += "    MSG_TYPE_NULL = 0,\n";
    int index = 0;
    for (auto It = vecUnion.begin(); It != vecUnion.end(); It++, index++)
    {
        char szMsgId[21] = {0};
        itoa(vecUnionId[index], szMsgId, 10);
        strUninBuffDef += "    ";
        strUninBuffDef += (*It);
        strUninBuffDef += "_Id";
        for (unsigned int index = 0;
            index < MAX_ENUM_LEN - ((*It).length() + 6 + strlen(szMsgId));
            index++)
        {
            strUninBuffDef += " ";
        }
        strUninBuffDef += "= ";
        strUninBuffDef += szMsgId;
        strUninBuffDef += ",\n";
        memset(szMsgId, 0, 21);
    }
    strUninBuffDef += "    MSG_TYPE_END,\n}VMsg;\n\n";
    strUninBuffDef += "}\n";
    strUninBuffDef += "}\n";
    strUninBuffDef += "\n#endif";
    if (VNOC::Message::ProduceFile(strUnionDefPath.c_str(), strUninBuffDef))
    {
        std::cout<<">>>>>>>>>> Produse ------->    MessageUnionDef.h"<<std::endl;
    }

    strUnionFunDefPath += "/MessageUnionFunDef.h";
    std::string strUninFun;
    strUninFun = "#ifndef VNOC_D_MESSSAGE_UNIONFUN_DEF\n";
    strUninFun += "#define VNOC_D_MESSSAGE_UNIONFUN_DEF\n\n";
    strUninFun += "#include \"CMessage.h\"\n";
    strUninFun += "#include \"MessageUnion.h\"\n\n";
    strUninFun += "namespace VNOC\n{\n";
    strUninFun += "namespace Message\n{\n\n";
    strUninFun += "void RegistrePort(CMessage* pMsg, int nId)\n{\n";
    strUninFun += "    CMessage* pMsgConver = NULL;\n";
    strUninFun += "    switch(nId)\n    {\n";
    for (auto It = vecUnion.begin(); It != vecUnion.end(); It++)
    {
        strUninFun += "    case ";
        strUninFun += (*It);
        strUninFun += "_Id:\n";
        strUninFun += "        pMsgConver = new ";
        strUninFun += (*It);
        strUninFun += ";\n";
        strUninFun += "        pMsg->CopyPort(*pMsgConver);\n";
        strUninFun += "        break;\n";
    }
    strUninFun += "    }\n";
    strUninFun += "    if (pMsg != NULL)\n    {\n";
    strUninFun += "        delete pMsgConver;\n    }\n";
    strUninFun += "}\n\n";
    strUninFun += "}\n";
    strUninFun += "}\n";
    strUninFun += "\n#endif";
    if (VNOC::Message::ProduceFile(strUnionFunDefPath.c_str(), strUninFun))
    {
        std::cout<<">>>>>>>>>> Produse ------->    MessageUnionFunDef.h"<<std::endl;
    }

    if (lpszTamplate != NULL)
    {
        delete [] lpszTamplate;
        lpszTamplate = NULL;
    }
    return true;
}