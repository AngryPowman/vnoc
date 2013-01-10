
// #include "ArrayData.h"
// 
// template<class ValueType>
// MsgStatus VNOC::Message::ArrayData<ValueType>::WriteList( IN const ArrayData<ValueType> ValueList )
// {
//     if (ValueList.Empty())
//     {
//         return MsgStatus_Err;
//     }
//     std::copy(ValueList.Begin(),ValueList.End(),m_ValueArr.begin());
//     return  MsgStatus_Ok;
// }
// 
// template<class ValueType>
// MsgStatus VNOC::Message::ArrayData<ValueType>::ReadList( IN ArrayData<ValueType>& ValueList )
// {
//     if (m_ValueArr.empty())
//     {
//         return MsgStatus_Err;
//     }
//     auto Itr = m_ValueArr.begin();
//     for (; Itr != m_ValueArr.end(); Itr++ )
//     {
//         ValueList.Push(*Itr);
//     }
//     return MsgStatus_Ok;
// }
