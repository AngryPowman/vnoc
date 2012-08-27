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
#include <string>
#include "Vector3.h"
#pragma once
namespace BKMath {
	class Vector2;    
	class Vector3;
	class Vector4;
class Vector4 {
private:
    bool operator<(const Vector4&) const;
    bool operator>(const Vector4&) const;
    bool operator<=(const Vector4&) const;
    bool operator>=(const Vector4&) const;

public:
  
    Vector4();
    Vector4(float fX, float fY, float fZ, float fW);
    Vector4(float afCoordinate[4]);
    Vector4(const Vector4& rkVector);
    Vector4(const Vector2& v1, const Vector2& v2);
    Vector4(const Vector2& v1, float fz, float fw);

    
    float x, y, z, w;   
    float& operator[] (int i);
    const float& operator[] (int i) const;    
    Vector4& operator= (const Vector4& rkVector);
    bool operator== (const Vector4& rkVector) const;
    bool operator!= (const Vector4& rkVector) const;
    static const Vector4& zero();

    inline void set(float _x, float _y, float _z, float _w) {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    }

    inline void set(const Vector3& v, float _w) {
        x = v.x;
        y = v.y;
        z = v.z;
        w = _w;
    }

    inline void set(const Vector2& v, float _z, float _w) {
        x = v.x;
        y = v.y;
        z = _z;
        w = _w;
    }

    size_t hashCode() const;
    bool fuzzyEq(const Vector4& other) const;
    bool fuzzyNe(const Vector4& other) const;

    static const Vector4& inf();
    static const Vector4& nan();

    

    float length() const;
    float squaredLength() const;

    inline float sum() const {
        return x + y + z + w;
    }

    bool isFinite() const;

    bool isZero() const;

    bool isUnit() const;
 
    Vector4 operator+ (const Vector4& rkVector) const;
    Vector4 operator- (const Vector4& rkVector) const;
    
    inline Vector4 operator*(const Vector4& rkVector) const {
        return Vector4(x * rkVector.x, y * rkVector.y, z * rkVector.z, w * rkVector.w);
    }

    inline Vector4 operator/(const Vector4& rkVector) const {
        return Vector4(x / rkVector.x, y / rkVector.y, z / rkVector.z, w / rkVector.w);
    }

    Vector4 operator*(const class Matrix4& M) const;

    Vector4 operator* (float fScalar) const;
    Vector4 operator/ (float fScalar) const;
    Vector4 operator- () const;
    friend Vector4 operator* (float, const Vector4& rkVector);

    
    Vector4& operator+= (const Vector4& rkVector);
    Vector4& operator-= (const Vector4& rkVector);
    Vector4& operator*= (float fScalar);
    Vector4& operator/= (float fScalar);

    inline Vector4 clamp(const Vector4& low, const Vector4& high) const {
        return Vector4(
            BKMath::clamp(x, low.x, high.x),
            BKMath::clamp(y, low.y, high.y),
            BKMath::clamp(z, low.z, high.z),
            BKMath::clamp(w, low.w, high.w));
    }

    inline Vector4 clamp(float low, float high) const {
        return Vector4(
            BKMath::clamp(x, low, high),
            BKMath::clamp(y, low, high),
            BKMath::clamp(z, low, high),
            BKMath::clamp(w, low, high));
    }

    float dot (const Vector4& rkVector) const;

    Vector4 min(const Vector4& v) const;
    Vector4 max(const Vector4& v) const;
    std::string toString() const;
    Vector4 lerp(const Vector4& v, float alpha) const;

};


inline Vector4::Vector4() {
    x = y = z = w = 0;
}


inline Vector4::Vector4 (float fX, float fY, float fZ, float fW) {
    x = fX;
    y = fY;
    z = fZ;
    w = fW;
}

inline Vector4::Vector4 (float afCoordinate[4]) {
    x = afCoordinate[0];
    y = afCoordinate[1];
    z = afCoordinate[2];
    w = afCoordinate[3];
}

inline Vector4::Vector4(const Vector4& rkVector) {
    x = rkVector.x;
    y = rkVector.y;
    z = rkVector.z;
    w = rkVector.w;
}


inline float& Vector4::operator[] (int i) {
    return ((float*)this)[i];
}

inline const float& Vector4::operator[] (int i) const {
    return ((float*)this)[i];
}

inline Vector4& Vector4::operator= (const Vector4& rkVector) {
    x = rkVector.x;
    y = rkVector.y;
    z = rkVector.z;
    w = rkVector.w;
    return *this;
}

inline bool Vector4::operator== (const Vector4& rkVector) const {
    return ( (x == rkVector.x) && (y == rkVector.y) && (z == rkVector.z) && (w == rkVector.w));
}

inline bool Vector4::operator!= (const Vector4& rkVector) const {
    return ( x != rkVector.x || y != rkVector.y || z != rkVector.z || w != rkVector.w);
}

inline Vector4 Vector4::operator+ (const Vector4& rkVector) const {
    return Vector4(x + rkVector.x, y + rkVector.y, z + rkVector.z, w + rkVector.w);
}

inline Vector4 Vector4::operator- (const Vector4& rkVector) const {
    return Vector4(x - rkVector.x, y - rkVector.y, z - rkVector.z, w - rkVector.w);
}

inline Vector4 Vector4::operator* (float fScalar) const {
    return Vector4(fScalar*x, fScalar*y, fScalar*z, fScalar*w);
}

inline Vector4 Vector4::operator- () const {
    return Vector4( -x, -y, -z, -w);
}

inline Vector4& Vector4::operator+= (const Vector4& rkVector) {
    x += rkVector.x;
    y += rkVector.y;
    z += rkVector.z;
    w += rkVector.w;
    return *this;
}

inline Vector4& Vector4::operator-= (const Vector4& rkVector) {
    x -= rkVector.x;
    y -= rkVector.y;
    z -= rkVector.z;
    w -= rkVector.w;
    return *this;
}


inline Vector4 Vector4::lerp(const Vector4& v, float alpha) const {
    return (*this) + (v - *this) * alpha; 
}


inline Vector4& Vector4::operator*= (float fScalar) {
    x *= fScalar;
    y *= fScalar;
    z *= fScalar;
    w *= fScalar;
    return *this;
}


inline float Vector4::dot(const Vector4& rkVector) const {
    return x*rkVector.x + y*rkVector.y + z*rkVector.z + w*rkVector.w;
}

inline Vector4 Vector4::min(const Vector4 &v) const {
    return Vector4(BKMath::min(v.x, x), BKMath::min(v.y, y), BKMath::min(v.z, z), BKMath::min(v.w, w));
}

inline Vector4 Vector4::max(const Vector4 &v) const {
    return Vector4(BKMath::max(v.x, x), BKMath::max(v.y, y), BKMath::max(v.z, z), BKMath::max(v.w, w));
}

inline bool Vector4::isZero() const {
    return (x == 0.0f) && (y == 0.0f) && (z == 0.0f) && (w == 0.0f);
}


inline bool Vector4::isFinite() const {
    return BKMath::isFinite(x) && BKMath::isFinite(y) && BKMath::isFinite(z) && BKMath::isFinite(w);
}


inline bool Vector4::isUnit() const {
    return squaredLength() == 1.0;
}


inline float Vector4::length() const {
    return sqrtf(squaredLength());
}


inline float Vector4::squaredLength() const {
    return x * x + y * y + z * z + w * w;
}

}
inline BKMath::Vector4 operator* (float s, const BKMath::Vector4& v) {
    return v * s;
}


