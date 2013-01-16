#include "ArrayData.h"

namespace VNOC
{
namespace Message
{

ArrayData::ArrayData()
{

}

ArrayData::~ArrayData()
{

}

size_t ArrayData::Size()
{
    return m_arrValuePtr.size();
}

bool ArrayData::Empty()
{
    return m_arrValuePtr.empty();
}

std::vector<MsgDataValue*>::const_iterator ArrayData::Begin() const
{
    return m_arrValuePtr.begin();
}

std::vector<MsgDataValue*>::const_iterator ArrayData::End() const
{
    return m_arrValuePtr.end();
}

void ArrayData::Push( MsgDataValue* pValue )
{
    return m_arrValuePtr.push_back(pValue);
}

}// namespace Message
}// namespace VNOC
