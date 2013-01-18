#ifndef  VNOC_C_MESSAGE_BUFFER
#define  VNOC_C_MESSAGE_BUFFER

#include "CMessage.h"

namespace VNOC
{
namespace Message
{

class CBufferMessage
{
public:
    CBufferMessage();
    virtual ~CBufferMessage();

    uint8* Alloc(IN uint32 nSize);

    uint8* GetBuffer() const;

    uint32 GetSize() const;

    const CBufferMessage& Copy(IN const void* Src, uint32 nSize);

    const CBufferMessage& Copy(IN const CBufferMessage& Src);

    void SetValue(uint8 Value);

    void Attach(IN void* lpBuff, IN uint32 nSize);

    void Detach();

    void Clear();

private:
    void _Free();
private:
    DISALLOW_COPY_AND_ASSIGN(CBufferMessage);
    uint32 m_Size;
    uint8* m_pBuffer;
    bool m_attachtag;
};

}// namespace Message
}// namespace VNOC

#endif