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

    size_t Size(){
        return m_pValueArr.size();
    }

    bool Empty(){
        return m_pValueArr.empty();
    }

    typename const std::vector<MsgDataValue*>::iterator Begin() const {
        return m_pValueArr.begin();
    }

    typename const std::vector<MsgDataValue*>::iterator End() const {
        return m_pValueArr.end();
    }

    void Push( 
        IN MsgDataValue Value
        ) 
    {
        return m_pValueArr.push_back(&Value);
    }

private:
    std::vector<MsgDataValue*>                  m_pValueArr;
};

}// namespace Message
}// namespace VNOC

#endif