#include "MessageFile.h"

namespace VNOC
{
namespace Message
{

int CMessageFile::Open(const TCHAR* lpszPath, MessageFileType fType)
{
    if (!lpszPath)
    {
        return -1;
    }
    if (_tfopen_s(&m_File, lpszPath, returnFileType(fType)) != 0)
    {
        return -1;
    }
    if (m_File == NULL)
    {
        return -1;
    }
    return 0;
}

void CMessageFile::Close()
{
    if (m_File != NULL)
    {
        fclose(m_File);
        m_File = NULL;
    }
}

int CMessageFile::Read(void* vbuffe, uint32 nCount, uint32 nSize)
{
    if (!vbuffe || !m_File)
    {
        return -1;
    }
    if (fread(vbuffe, nSize, nCount, m_File) ==  0)
    {
        return -1;
    }
    return 0;
}

int CMessageFile::Write(void* vbuffe, uint32 nCount, uint32 nSize)
{
    if (!vbuffe || !m_File)
    {
        return -1;
    }
    if (fwrite(vbuffe, nSize, nCount, m_File) ==  0)
    {
        return -1;
    }
    return 0;
}

void CMessageFile::Seek(int nPos,int nLen)
{
    if (m_File)
    {
        fseek(m_File, nLen, nPos);
    }
}

long CMessageFile::returnFilePrtPos()
{
    if (!m_File)
    {
        return -1;
    }
    return (ftell(m_File));
}

TCHAR* CMessageFile::returnFileType(MessageFileType fTpye)
{
    switch (fTpye)
    {
        case UNIN_F_R: return (_T("r"));
        case UNIN_F_W: return (_T("w"));
        case UNIN_F_A: return  (_T("a"));
        case UNIN_F_ATO_A: return (_T("a+"));
        case UNIN_F_ATO_R: return (_T("r+"));
        case UNIN_F_ATO_W: return (_T("w+"));
    }
    return 0;
}

int CMessageFile::returnSeekType(MessageFileType fType)
{
    switch (fType)
    {
        case UNIN_F_SK_SET: return SEEK_SET;
        case UNIN_F_SK_CUR: return SEEK_CUR;
        case UNIN_F_SK_END: return SEEK_END;
    }
    return -1;
}

FILE* CMessageFile::returnFilePrt()
{
    return m_File;
}

int CMessageFile::GetFileSize()
{
    if (returnFilePrt() == NULL)
    {
        return -1;
    }
    int   FileSize = 0;
    int   OldPos = returnFilePrtPos();  //保存当前的位置。
    Seek(UNIN_F_SK_END,0);
    FileSize = ftell(returnFilePrt());
    Seek(UNIN_F_SK_SET,OldPos);
    return FileSize;
}

CMessageFile::CMessageFile() : m_File(0)
{
}

CMessageFile::~CMessageFile()
{
    Close();
}

}
}