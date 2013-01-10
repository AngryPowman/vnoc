#ifndef  VNOC_I_MESSAGE_DATA_ARRAY
#define  VNOC_I_MESSAGE_DATA_ARRAY

#include "MsgDataValue.h"


namespace VNOC
{
namespace Message
{

template<class ValueType>
class ArrayData:public MsgDataValue
{
public:

    virtual MsgStatus WriteList(
        IN const ArrayData<ValueType> ValueList
        )
    { return MsgStatus_Unk;}

    virtual MsgStatus ReadList(
        IN ArrayData<ValueType>& ValueList
        )
    { return MsgStatus_Unk;}

private:

    vector<ValueType>      m_ValueArr;

};



}// namespace Message
}// namespace VNOC

#endif