#pragma once
#ifndef UTIL_H_
#define UTIL_H_
#include <stdlib.h>
#include <memory>
using namespace std;

class smart_buf_imp{
public:
    smart_buf_imp(char *raw){
        this->buf_ = raw;
    }
    smart_buf_imp():buf_(NULL){};
    ~smart_buf_imp(){
        delete[] buf_;
    }

    operator char *(){
        return buf_;
    }

private:
    mutable char *buf_;
    smart_buf_imp(const smart_buf_imp &other){}
    smart_buf_imp &operator =(const smart_buf_imp &other){}
};
class smart_buf{
public:
    smart_buf(char *raw):imp_(new smart_buf_imp(raw)){}

    operator char *(){
        return (char *)(*imp_);
    }
    operator unsigned char *(){
        return (unsigned char*)(char *)*this;
    }
private:
    shared_ptr<smart_buf_imp> imp_;
};
#endif //UTIL_H_
