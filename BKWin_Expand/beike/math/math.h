/**
 
  @author Morgan McGuire, http://graphics.cs.williams.edu
 
  @created 2002-07-09

  Copyright 2000-2009, Morgan McGuire.
  All rights reserved.

  @modified  2012-2-18 by zhenganming

  this code derived from G3D  graphic engine.all license see http://g3d.sourceforge.net/
  this code remove the dependcy of G3D platform,remove some advanced mechanism that generic UI not use
  ,remove the ability which object parse from *.any file and change some implement under c std lib
 */

#include <ctype.h>
#include <float.h>
#include <limits>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#undef min
#undef max
#pragma once
namespace BKMath {

inline double __fastcall drand48() {
    return ::rand() / double(RAND_MAX);
}
__inline long int lrint (double flt) {
    int intgr;

    _asm {
        fld flt
        fistp intgr
    };

    return intgr;
}

__inline long int lrintf(float flt) {
    int intgr;

    _asm {
        fld flt
        fistp intgr
    };

    return intgr;
}


#define fuzzyEpsilon (0.00001f)
double inf();
double nan();
float finf();
float fnan();

inline double pi() {
    return 3.1415926535898;
}

inline float pif() {
    return 3.1415926535898f;
}

inline double halfPi() {
    return 1.57079633;
}

inline double twoPi() {
    return 6.28318531;
}

typedef signed char     int8;
typedef unsigned char	uint8;
typedef short           int16;
typedef unsigned short  uint16;
typedef int             int32;
typedef unsigned int    uint32;

typedef __int64             int64;
typedef unsigned __int64    uint64;

typedef float           float32;
typedef double          float64;

int iAbs(int iValue);
int iCeil(double fValue);
int iClamp(int val, int low, int hi);
int16 iClamp(int16 val, int16 low, int16 hi);
double clamp(double val, double low, double hi);
float clamp(float val, float low, float hi);
inline double lerp(double a, double b, double f) {
    return a + (b - a) * f;
}

inline float lerp(float a, float b, float f) {
    return a + (b - a) * f;
}
int iWrap(int val, int hi);

int iFloor(double fValue);

int iSign(int iValue);
int iSign(double fValue);

inline int iSign(float f) {
    return iSign((double)f);
}
inline int iRound(double fValue) {
    return lrint(fValue);
}
inline int iRound(float f) {
    return lrintf(f);
}
int iRandom(int low, int hi);

double abs (double fValue);
double aCos (double fValue);
double aSin (double fValue);
double aTan (double fValue);
double aTan2 (double fY, double fX);
double sign (double fValue);
double square (double fValue);



bool isFinite(double x);



bool isNaN(double x);
bool isNaN(float x);
inline bool isNaN(int x) {
    (void)x;
    return false;
}



int iMod3(int x);



float uniformRandom(float low = 0.0f, float hi = 1.0f);



float gaussRandom(float mean = 0.0f, float stdev = 1.0f);




template <class T>
inline T pow5(T x) {
    const T y = x * x;
    return y * y * x;
}


template <class T>
inline T min(const T& x, const T& y) {
    return std::min<T>(x, y);
}

template <class T>
inline T min(const T& x, const T& y, const T& z) {
    return std::min<T>(std::min<T>(x, y), z);
}

template <class T>
inline T min(const T& x, const T& y, const T& z, const T& w) {
    return std::min<T>(std::min<T>(x, y), std::min<T>(z, w));
}

template <class T>
inline T max(const T& x, const T& y) {
    return std::max<T>(x, y);
}

template <class T>
inline T max(const T& x, const T& y, const T& z) {
    return std::max<T>(std::max<T>(x, y), z);
}

template <class T>
inline T max(const T& x, const T& y, const T& z, const T& w) {
    return std::max<T>(std::max<T>(x, y), std::max<T>(z, w));
}

int iMin(int x, int y);
int iMax(int x, int y);

double square(double x);
double sumSquares(double x, double y);
double sumSquares(double x, double y, double z);
double distance(double x, double y);
double distance(double x, double y, double z);


 
int highestBit(uint32 x);



bool fuzzyEq(double a, double b);



bool fuzzyNe(double a, double b);



bool fuzzyGt(double a, double b);



bool fuzzyGe(double a, double b);



bool fuzzyLt(double a, double b);



bool fuzzyLe(double a, double b);



inline float rsq(float x) {
    return 1.0f / sqrtf(x);
}



int ceilPow2(unsigned int in);



inline int pow2(unsigned int x) {
    return 1 << x;
}

inline double log2(double x) {
    return ::log(x) * 1.442695;
}

inline float log2(float x) {
    return ::logf(x) * 1.442695f;
}

inline double log2(int x) {
    return log2((double)x);
}




bool isPow2(int num);

bool isOdd(int num);
bool isEven(int num);

double toRadians(double deg);
double toDegrees(double rad);



inline bool any(float x) {
    return x != 0;
}



inline bool all(float x) {
    return x != 0;
}



inline float normalize(float v) {
    return v / v;
}



inline float dot(float a, float b) {
    return a * b;
}




inline float mul(float a, float b) {
    return a * b;
}



inline double exp2(double x) {
    return pow(2.0, x);
}

inline float exp2(float x) {
    return powf(2.0f, x);
}



inline double rsqrt(double x) {
    return 1.0 / sqrt(x);
}



inline float rsqrt(float x) {
    
    return 1.0 / sqrtf(x);
}



inline double sinc(double x) {
    double r = sin(x) / x;

    if (isNaN(r)) {
        return 1.0;
    } else {
        return r;
    }
}



inline float wrap(float t, float lo, float hi) {
    if ((t >= lo) && (t < hi)) {
        return t;
    }

    assert(hi > lo);

    float interval = hi - lo;

    return t - interval * iFloor((t - lo) / interval);
}


inline double wrap(double t, double lo, double hi) {
    if ((t >= lo) && (t < hi)) {
        return t;
    }

    assert(hi > lo);

    double interval = hi - lo;

    return t - interval * iFloor((t - lo) / interval);
}

inline double wrap(double t, double hi) {
    return wrap(t, 0.0, hi);
}


inline bool isFinite(double x) {
    return ! isNaN(x) && (x < BKMath::inf()) && (x > -BKMath::inf());
}

inline bool isFinite(float x) {
    return ! isNaN(x) && (x < BKMath::finf()) && (x > -BKMath::finf());
}

inline int iAbs (int iValue) {
    return ( iValue >= 0 ? iValue : -iValue );
}

inline int iCeil (double fValue) {
    return int(::ceil(fValue));
}


inline int iClamp(int val, int low, int hi) {
    assert(low <= hi);
	if (val <= low) {
		return low;
	} else if (val >= hi) {
		return hi;
	} else {
		return val;
	}
}


inline int16 iClamp(int16 val, int16 low, int16 hi) {
    assert(low <= hi);
	if (val <= low) {
		return low;
	} else if (val >= hi) {
		return hi;
	} else {
		return val;
	}
}


inline double clamp(double val, double low, double hi) {
    assert(low <= hi);
	if (val <= low) {
		return low;
	} else if (val >= hi) {
		return hi;
	} else {
		return val;
	}
}

inline float clamp(float val, float low, float hi) {
    assert(low <= hi);
	if (val <= low) {
		return low;
	} else if (val >= hi) {
		return hi;
	} else {
		return val;
	}
}

inline int iWrap(int val, int hi) {
	if (val < 0) {
		return ((val % hi) + hi) % hi;
	} else {
		return val % hi;
	}
}

inline int iFloor (double fValue) {
    return int(::floor(fValue));
}

inline int iSign (int iValue) {
    return ( iValue > 0 ? + 1 : ( iValue < 0 ? -1 : 0 ) );
}

inline int iSign (double fValue) {
    return ( fValue > 0.0 ? + 1 : ( fValue < 0.0 ? -1 : 0 ) );
}

inline double abs (double fValue) {
    return double(::fabs(fValue));
}

inline double aCos (double fValue) {
    if ( -1.0 < fValue ) {
        if ( fValue < 1.0 )
            return double(::acos(fValue));
        else
            return 0.0;
    } else {
        return pi();
    }
}

inline float acos (float fValue) {
    if ( -1.0f < fValue ) {
        if ( fValue < 1.0f ) {
            return ::acos(fValue);
        } else {
            return 0.0f;
        }
    } else {
        return pif();
    }
}

inline double aSin (double fValue) {
    if ( -1.0 < fValue ) {
        if ( fValue < 1.0 ) {
            return double(::asin(fValue));
        } else {
            return -halfPi();
        }
    } else {
        return halfPi();
    }
}

inline double aTan (double fValue) {
    return double(::atan(fValue));
}

inline double aTan2 (double fY, double fX) {
    return double(::atan2(fY, fX));
}

inline double sign (double fValue) {
    if (fValue > 0.0) {
        return 1.0;
	}

    if (fValue < 0.0) {
        return -1.0;
	}

    return 0.0;
}

inline float sign (float fValue) {
    if (fValue > 0.0f) {
        return 1.0f;
	}

    if (fValue < 0.0f) {
        return -1.0f;
	}

    return 0.0f;
}


inline float uniformRandom(float low, float hi) {
    return (hi - low) * float(::rand()) / float(RAND_MAX) + low;
}

inline double square(double x) {
    return x * x;
}

inline float square(float x) {
    return x * x;
}

inline int square(int x) {
    return x * x;
}

inline double sumSquares(double x, double y) {
    return x*x + y*y;
}

inline float sumSquares(float x, float y) {
    return x*x + y*y;
}

inline double sumSquares(double x, double y, double z) {
    return x*x + y*y + z*z;
}

inline float sumSquares(float x, float y, float z) {
    return x*x + y*y + z*z;
}

inline double distance(double x, double y) {
    return sqrt(sumSquares(x, y));
}

inline float distance(float x, float y) {
    return sqrt(sumSquares(x, y));
}

inline double distance(double x, double y, double z) {
    return sqrt(sumSquares(x, y, z));
}

inline float distance(float x, float y, float z) {
    return sqrt(sumSquares(x, y, z));
}




inline int iMin(int x, int y) {
    return (x >= y) ? y : x;
}



inline int iMax(int x, int y) {
    return (x >= y) ? x : y;
}

inline int ceilPow2(unsigned int in) {
    in -= 1;

    in |= in >> 16;
    in |= in >> 8;
    in |= in >> 4;
    in |= in >> 2;
    in |= in >> 1;

    return in + 1;
}

inline bool isPow2(int num) {
    return ((num & -num) == num);
}

inline bool isOdd(int num) {
    return (num & 1) == 1;
}

inline bool isEven(int num) {
    return (num & 1) == 0;
}

inline double toRadians(double deg) {
    return deg * pi() / 180.0;
}

inline double toDegrees(double rad) {
    return rad * 180.0 / pi();
}

inline float toRadians(float deg) {
    return deg * (float)pi() / 180.0f;
}

inline float toDegrees(float rad) {
    return rad * 180.0f / (float)pi();
}

inline float toRadians(int deg) {
    return deg * (float)pi() / 180.0f;
}

inline float toDegrees(int rad) {
    return rad * 180.0f / (float)pi();
}


inline double eps(double a, double b) {
    
    
    
    
    (void)b;
    const double aa = abs(a) + 1.0;
    if (aa == inf()) {
        return fuzzyEpsilon;
    } else {
        return fuzzyEpsilon * aa;
    }
}

inline bool fuzzyEq(double a, double b) {
    return (a == b) || (abs(a - b) <= eps(a, b));
}

inline bool fuzzyNe(double a, double b) {
    return ! fuzzyEq(a, b);
}

inline bool fuzzyGt(double a, double b) {
    return a > b + eps(a, b);
}

inline bool fuzzyGe(double a, double b) {
    return a > b - eps(a, b);
}

inline bool fuzzyLt(double a, double b) {
    return a < b - eps(a, b);
}

inline bool fuzzyLe(double a, double b) {
    return a < b + eps(a, b);
}

inline int iMod3(int x) {
    return x % 3;
}



} 



