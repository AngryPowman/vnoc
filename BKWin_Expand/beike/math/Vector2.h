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



#include <string>
#include "math.h"
#pragma once
namespace BKMath {



class Vector2 {
private:
    
    bool operator<(const Vector2&) const;
    bool operator>(const Vector2&) const;
    bool operator<=(const Vector2&) const;
    bool operator>=(const Vector2&) const;

public:
    float x;
    float y;

    Vector2();
    Vector2(float x, float y);
    Vector2(float coordinate[2]);
    Vector2(double coordinate[2]);
    Vector2(const Vector2& other);
    float& operator[](int i);
    const float& operator[](int i) const;

    
    Vector2& operator=(const Vector2& other);
    bool operator==(const Vector2& other) const;
    bool operator!=(const Vector2& other) const;
    bool isZero() const;
    bool isUnit() const;
    Vector2 operator+(const Vector2& v) const;
    Vector2 operator-(const Vector2& v) const;
    Vector2 operator*(float s) const;
    Vector2 operator*(const Vector2& v) const;
    Vector2 operator/(const Vector2& v) const;
    Vector2 operator/(float s) const;
    Vector2 operator-() const;

    inline float sum() const {
        return x + y;
    }

    inline Vector2 lerp(const Vector2& v, float alpha) const {
        return (*this) + (v - *this) * alpha; 
    }

    inline Vector2 clamp(const Vector2& low, const Vector2& high) const {
        return Vector2(
            BKMath::clamp(x, low.x, high.x),
            BKMath::clamp(y, low.y, high.y));
    }

    inline Vector2 clamp(float low, float high) const {
        return Vector2(
            (float)BKMath::clamp(x, low, high),
            (float)BKMath::clamp(y, low, high));
    }

    Vector2& operator+=(const Vector2&);
    Vector2& operator-=(const Vector2&);
    Vector2& operator*=(float);
    Vector2& operator/=(float);
    Vector2& operator*=(const Vector2&);
    Vector2& operator/=(const Vector2&);

    float length() const;
    Vector2 direction() const;
    Vector2 fastDirection() const {
        return direction();
    }

    float squaredLength() const;
    float dot(const Vector2& s) const;
    float unitize(float fTolerance = 1e-06);
    Vector2 min(const Vector2& v) const;
    Vector2 max(const Vector2& v) const;
   
    static const Vector2& zero();
    static const Vector2& one();
    static const Vector2& unitX();
    static const Vector2& unitY();

    std::string toString() const;
	size_t hashCode() const;

};

inline Vector2 operator*(double s, const Vector2& v) {
    return v * (float)s;
}

inline Vector2 operator*(float s, const Vector2& v) {
    return v * s;
}

inline Vector2 operator*(int s, const Vector2& v) {
    return v * (float)s;
}


inline Vector2::Vector2 () : x(0.0f), y(0.0f) {
}


inline Vector2::Vector2(float _x, float _y) : x(_x), y(_y) {
}


inline Vector2::Vector2 (float afCoordinate[2]) {
    x = afCoordinate[0];
    y = afCoordinate[1];
}



inline Vector2::Vector2 (double afCoordinate[2]) {
    x = (float)afCoordinate[0];
    y = (float)afCoordinate[1];
}


inline Vector2::Vector2 (const Vector2& rkVector) {
    x = rkVector.x;
    y = rkVector.y;
}




inline float& Vector2::operator[] (int i) {
    return ((float*)this)[i];
}


inline const float& Vector2::operator[] (int i) const {
    return ((float*)this)[i];
}


inline Vector2& Vector2::operator= (const Vector2& rkVector) {
    x = rkVector.x;
    y = rkVector.y;
    return *this;
}


inline bool Vector2::operator== (const Vector2& rkVector) const {
    return ( x == rkVector.x && y == rkVector.y);
}


inline bool Vector2::operator!= (const Vector2& rkVector) const {
    return ( x != rkVector.x || y != rkVector.y);
}


inline Vector2 Vector2::operator+ (const Vector2& rkVector) const {
    return Vector2(x + rkVector.x, y + rkVector.y);
}


inline Vector2 Vector2::operator- (const Vector2& rkVector) const {
    return Vector2(x - rkVector.x, y - rkVector.y);
}


inline Vector2 Vector2::operator* (float fScalar) const {
    return Vector2(fScalar*x, fScalar*y);
}



inline Vector2 Vector2::operator- () const {
    return Vector2( -x, -y);
}



inline Vector2& Vector2::operator+= (const Vector2& rkVector) {
    x += rkVector.x;
    y += rkVector.y;
    return *this;
}



inline Vector2& Vector2::operator-= (const Vector2& rkVector) {
    x -= rkVector.x;
    y -= rkVector.y;
    return *this;
}



inline Vector2& Vector2::operator*= (float fScalar) {
    x *= fScalar;
    y *= fScalar;
    return *this;
}




inline Vector2& Vector2::operator*= (const Vector2& rkVector) {
    x *= rkVector.x;
    y *= rkVector.y;
    return *this;
}



inline Vector2& Vector2::operator/= (const Vector2& rkVector) {
    x /= rkVector.x;
    y /= rkVector.y;
    return *this;
}


inline Vector2 Vector2::operator* (const Vector2& rkVector) const {
    return Vector2(x * rkVector.x, y * rkVector.y);
}



inline Vector2 Vector2::operator/ (const Vector2& rkVector) const {
    return Vector2(x / rkVector.x, y / rkVector.y);
}


inline float Vector2::squaredLength () const {
    return x*x + y*y;
}


inline float Vector2::length () const {
    return sqrtf(x*x + y*y);
}


inline Vector2 Vector2::direction () const {
    float lenSquared = x * x + y * y;

    if (lenSquared != 1.0f) {
        return *this / sqrtf(lenSquared);
    } else {
        return *this;
    }
}

inline float Vector2::dot (const Vector2& rkVector) const {
    return x*rkVector.x + y*rkVector.y;
}



inline Vector2 Vector2::min(const Vector2 &v) const {
    return Vector2(BKMath::min(v.x, x), BKMath::min(v.y, y));
}



inline Vector2 Vector2::max(const Vector2 &v) const {
    return Vector2(BKMath::max(v.x, x), BKMath::max(v.y, y));
}

inline bool Vector2::isZero() const {
    return (x == 0.0f) && (y == 0.0f);
}

inline bool Vector2::isUnit() const {
    return squaredLength() == 1.0f;
}

} 


inline BKMath::Vector2 operator*(double s, const BKMath::Vector2& v) {
    return v * (float)s;
}
inline BKMath::Vector2 operator*(int s, const BKMath::Vector2& v) {
    return v * (float)s;
}


