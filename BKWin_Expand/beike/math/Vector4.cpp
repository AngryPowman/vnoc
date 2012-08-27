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


#include <stdlib.h>
#include <limits>
#include "Vector4.h"
#include "math.h"
#include "Matrix4.h"

namespace BKMath {

const Vector4& Vector4::inf() { 
    static const Vector4 v((float)BKMath::finf(), (float)BKMath::finf(), (float)BKMath::finf(), (float)BKMath::finf()); 
    return v; 
}


const Vector4& Vector4::zero() { 
    static const Vector4 v(0,0,0,0); 
    return v;
}

const Vector4& Vector4::nan() { 
    static Vector4 v((float)BKMath::fnan(), (float)BKMath::fnan(), (float)BKMath::fnan(), (float)BKMath::fnan()); 
    return v; 
}

Vector4::Vector4(const Vector2& v1, const Vector2& v2) {
    x = v1.x;
    y = v1.y;
    z = v2.x;
    w = v2.y;
}


Vector4::Vector4(const Vector2& v1, float fz, float fw) {
    x = v1.x;
    y = v1.y;
    z = fz;
    w = fw;
}


Vector4 Vector4::operator*(const Matrix4& M) const {
    Vector4 result;
    for (int i = 0; i < 4; ++i) {
        result[i] = 0.0f;
        for (int j = 0; j < 4; ++j) {
            result[i] += (*this)[j] * M[j][i];
        }
    }
    return result;
}


Vector4 Vector4::operator/ (float fScalar) const {
    Vector4 kQuot;

    if ( fScalar != 0.0 ) {
		float fInvScalar = 1.0f / fScalar;
        kQuot.x = fInvScalar * x;
        kQuot.y = fInvScalar * y;
        kQuot.z = fInvScalar * z;
        kQuot.w = fInvScalar * w;
        return kQuot;
    } else {
        return Vector4::inf();
    }
}

Vector4& Vector4::operator/= (float fScalar) {
    if (fScalar != 0.0f) {
		float fInvScalar = 1.0f / fScalar;
        x *= fInvScalar;
        y *= fInvScalar;
        z *= fInvScalar;
        w *= fInvScalar;
    } else {
		*this = Vector4::inf();
    }

    return *this;
}

}; 

