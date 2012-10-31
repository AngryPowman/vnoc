#pragma once
#ifndef UTIL_H_
#define UTIL_H_
#include <stdlib.h>

class smart_buf{
public:
    smart_buf(char *raw){
        this->buf_ = raw;
    }
    smart_buf():buf_(NULL){};
    ~smart_buf(){
        delete[] buf_;
    }
    smart_buf(smart_buf &&other)
    {
        if (&other != this){
            this->buf_ = other.buf_;
            other.buf_ = NULL;
        }
    }
    operator char *(){
        return buf_;
    }
    operator unsigned char *(){
        return (unsigned char*)buf_;
    }
private:
    char *buf_;
};


#endif //UTIL_H_
