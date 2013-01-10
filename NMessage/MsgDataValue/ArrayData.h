#ifndef  VNOC_I_MESSAGE_DATA_ARRAY
#define  VNOC_I_MESSAGE_DATA_ARRAY

#include "../MessageDef.h"


namespace VNOC
{
namespace Message
{

template<class ValueType>
class ArrayData :public MsgDataValue
{
public:
    ArrayData(){}
    virtual ~ArrayData(){};

    virtual MsgStatus WriteList(
        IN const ArrayData<ValueType> ValueList
        );

    virtual MsgStatus ReadList(
        IN ArrayData<ValueType>& ValueList
        );

    virtual size_t Size(){
        return m_ValueArr.size();
    }

    virtual bool Empty(){
        return m_ValueArr.empty();
    }

    virtual const vector<ValueType>::iterator Begin() const {
        return m_ValueArr.begin();
    }

    virtual const vector<ValueType>::iterator End() const {
        return m_ValueArr.end();
    }

    virtual void Push( 
        IN ValueType Value
        ) 
    {
        return m_ValueArr.push_back(Value);
    }

private:
    vector<ValueType>      m_ValueArr;

};



}// namespace Message
}// namespace VNOC

#endif