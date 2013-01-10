#ifndef  VNOC_I_MESSAGE_DATA_ARRAY
#define  VNOC_I_MESSAGE_DATA_ARRAY

#include "../MessageDef.h"


namespace VNOC
{
namespace Message
{

class ArrayData
{
public:
    ArrayData(){}
    virtual ~ArrayData(){}

    virtual MsgStatus WriteList(
        IN const ArrayData ValueList
        )
    {
        if (ValueList.Empty())
        {
            return MsgStatus_Err;
        }
        std::copy(ValueList.Begin(),ValueList.End(),m_pValueArr.begin());
        return  MsgStatus_Ok;
    }

    virtual MsgStatus ReadList(
        IN ArrayData& ValueList
        )
    {
        if (m_pValueArr.empty())
        {
            return MsgStatus_Err;
        }
        auto Itr = m_pValueArr.begin();
        for (; Itr != m_pValueArr.end(); Itr++ )
        {
            ValueList.Push(*Itr);
        }
        return MsgStatus_Ok;
    }

    virtual size_t Size(){
        return m_pValueArr.size();
    }

    virtual bool Empty(){
        return m_pValueArr.empty();
    }

    virtual typename const vector::iterator Begin() const {
        return m_pValueArr.begin();
    }

    virtual typename const vector::iterator End() const {
        return m_pValueArr.end();
    }

    virtual void Push( 
        IN MsgDataValue Value
        ) 
    {
        return m_pValueArr.push_back(Value);
    }

private:
    std::vector<MsgDataValue*>                  m_pValueArr;
    const vector<MsgDataValue*>::iterator       m_pValueArrItr;
};

}// namespace Message
}// namespace VNOC

#endif