#ifndef  VNOC_I_MESSAGE_DATA_ARRAY
#define  VNOC_I_MESSAGE_DATA_ARRAY

#include "../MessageDef.h"

namespace VNOC
{
namespace Message
{

class MsgDataValue;

class ArrayData
{
public:
    ArrayData();
    virtual ~ArrayData();

    size_t Size();

    bool Empty();

    std::vector<MsgDataValue*>::const_iterator Begin() const;

    std::vector<MsgDataValue*>::const_iterator End() const;

    void Push(IN MsgDataValue* pValue);

private:
    std::vector<MsgDataValue*> m_arrValuePtr;
};

}// namespace Message
}// namespace VNOC

#endif