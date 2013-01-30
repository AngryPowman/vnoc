#ifndef VNOC_COMMON_H_
#define VNOC_COMMON_H_

//stl
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <hash_map>

using namespace std;

//c
#include<time.h>

//Socket dependencies
#if defined(_WIN32)
#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <sys/types.h>
#include <sys/socket.h>
#endif

//Datatypes
#if defined(_WIN32)	//for windows
#ifndef byte
typedef unsigned __int8    byte;
#endif
typedef __int8             int8;
typedef __int16            int16;
typedef __int32            int32;
typedef __int64            int64;
typedef unsigned __int32   uint;
typedef unsigned __int8    uint8;
typedef unsigned __int16   uint16;
typedef unsigned __int32   uint32;
typedef unsigned __int64   uint64;
#else
#ifndef byte
typedef unsigned char      byte;
#endif
typedef char               int8;
typedef short              int16;
typedef int                int32;
typedef long long          int64;
typedef unsigned int       uint;
typedef unsigned char      uint8;
typedef unsigned short     uint16;
typedef unsigned int       uint32;
typedef unsigned long long uint64;
#endif

//Lines
#if defined(_WIN32)
#define NEW_LINE "\r\n"
#else
#define NEW_LINE "\n"
#endif

#endif