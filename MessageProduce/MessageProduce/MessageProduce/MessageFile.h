#ifndef VNOC_C_MESSAGE_FILE
#define VNOC_C_MESSAGE_FILE

#include <stdio.h>
#include <tchar.h>
#include <string>

typedef unsigned int uint32;

namespace VNOC
{
namespace Message
{

typedef enum _MessageFileType
{
    SFILETYPE_NULL,
    UNIN_F_R,
    UNIN_F_W,
    UNIN_F_A,
    UNIN_F_ATO_A,
    UNIN_F_ATO_R,
    UNIN_F_ATO_W,
}MessageFileType;

typedef enum _MessageFileSeekType
{
    UNIN_F_SK_SET,
    UNIN_F_SK_CUR,
    UNIN_F_SK_END,
}MessageFileSeekType;

class CMessageFile
{
public:
    CMessageFile();
    virtual ~CMessageFile();;

    ///> open file
    int Open(const TCHAR* lpszPath, MessageFileType fType);

    ///> write file
    ///> *first open one file
    int Write(void* vbuffe, uint32 nCount, uint32 nSize);

    ///> move file_prt
    ///> * UNIN_F_SET
    ///> * UNIN_F_CUR
    ///> * UNIN_F_END
    void Seek(int nPos, int nLen);

    ///> Read file
    ///> *first open one file
    int Read(void* vbuffe, uint32 nCount, uint32 nSize);

    ///> shut down
    ///> close the already open files
    void Close();

    int GetFileSize();

protected:

    TCHAR* returnFileType(MessageFileType fTpye);

    int returnSeekType(MessageFileType fType);

    long returnFilePrtPos();

    FILE* returnFilePrt();

private:
    FILE* m_File;
};

}
}

#endif