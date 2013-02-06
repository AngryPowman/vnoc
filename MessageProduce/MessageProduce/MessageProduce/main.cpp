#include <iostream>
#include "Dispose.hpp"

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
            std::string strGetParam, strSetParam;
            VNOC::Message::XMLObject* obj = &(Itr->second);
            std::string strSrc;
            bool TopLine = false;
            VNOC::Message::DisposeName(strTamplate, strSrc, MESSAGE_TAMPLATE_NAME_TAG, Itr->second.GetName(), true);
            for (auto ItrItem = obj->ParamBegin(); ItrItem != obj->ParamEnd(); ItrItem++)
            {
                std::string strTmp;
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
            VNOC::Message::DisposeName(strSrc, strSrc, MESSAGE_TAMPLATE_PARAM_TAG_SET, strSetParam, false);
            VNOC::Message::DisposeName(strSrc, strSrc, MESSAGE_TAMPLATE_PARAM_TAG_GET, strGetParam, false);
            std::string strTargetPath = lpTargetPath;
            if (!strTargetPath.empty())
            {
                strTargetPath += "/";
                strTargetPath += obj->GetName();
                strTargetPath += ".h";
                vecUnion.push_back(obj->GetName() + ".h");
                if (VNOC::Message::ProduceFile(strTargetPath.c_str(), strSrc))
                {
                    std::cout<<">>>>>>>>>> Produse ------->    "<<obj->GetName()<<std::endl;
                }
            }
        }
    }

    std::string strUnionPath = lpTargetPath;
    strUnionPath += "/MessageUnion.h";
    std::string strUninBuff;
    strUninBuff = "#ifndef VNOC_D_MESSSAGE_UNION\n";
    strUninBuff += "#define VNOC_D_MESSSAGE_UNION\n\n";
    for (auto It = vecUnion.begin(); It != vecUnion.end(); It++)
    {
        strUninBuff += "#include \"" + (*It) + "\"\n";
    }
    strUninBuff += "\n#endif";
    if (VNOC::Message::ProduceFile(strUnionPath.c_str(), strUninBuff))
    {
        std::cout<<">>>>>>>>>> Produse ------->    MessageUnion.h"<<std::endl;
    }
    if (lpszTamplate != NULL)
    {
        delete [] lpszTamplate;
        lpszTamplate = NULL;
    }
    return true;
}