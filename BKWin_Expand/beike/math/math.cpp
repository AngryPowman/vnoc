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


#include "math.h"
#include <cstdlib>
#include <cstring>


namespace BKMath {

float gaussRandom(float mean, float stdev) {
    float w, x1, x2;
    do {
        x1 = uniformRandom(-1.0, 1.0);
        x2 = uniformRandom(-1.0, 1.0);

        w = float(square(x1) + square(x2));
    } while (w > 1.0f);
    return x2 * (float)square(stdev) * sqrtf((-2.0f * logf(w) ) / w) + mean; 
}
double inf() {
    return std::numeric_limits<double>::infinity();
}

bool isNaN(float x) {
    static const float n = nan();
    return memcmp(&x, &n, sizeof(float)) == 0;
}

bool isNaN(double x) {
    static const double n = nan();
    return memcmp(&x, &n, sizeof(double)) == 0;
}
float finf() {
    return std::numeric_limits<float>::infinity();
}
double nan() {
    return std::numeric_limits<double>::quiet_NaN();
}

float fnan() {
    return std::numeric_limits<float>::quiet_NaN();
}


int highestBit(uint32 x) {
    int base = 0;

    if (x & 0xffff0000)	{
        base = 16;
        x >>= 16;
    }
    if (x & 0x0000ff00) {
        base += 8;
        x >>= 8;
    }
    if (x & 0x000000f0) {
        base += 4;
        x >>= 4;
    }
    
    static const int lut[] = {-1,0,1,1,2,2,2,2,3,3,3,3,3,3,3,3};
    return base + lut[x];
}


int iRandom(int low, int high) {
    int r = iFloor(low + (high - low + 1) * (double)rand() / RAND_MAX);
    if (r > high) {
        return high;
    } else {
        return r;
    }
}


}

