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
#include "Vector2.h"
#include "Vector2.h"
#include <iostream>
#include <string>
#pragma once
namespace BKMath {

class Vector3 {
public:

    
    float x, y, z;

private:   
    bool operator<(const Vector3&) const;
    bool operator>(const Vector3&) const;
    bool operator<=(const Vector3&) const;
    bool operator>=(const Vector3&) const;

public:
    

    Vector3();

    Vector3(float _x, float _y, float _z);
    explicit Vector3(const class Vector2& v, float _z);
    explicit Vector3(float coordinate[3]);
    explicit Vector3(double coordinate[3]);
   
    const float& __fastcall operator[] (int i) const;
    float& operator[] (int i);

    enum Axis {X_AXIS=0, Y_AXIS=1, Z_AXIS=2, DETECT_AXIS=-1};

    

    Axis primaryAxis() const;

    
    Vector3& __fastcall operator= (const Vector3& rkVector);
    bool operator== (const Vector3& rkVector) const;
    bool operator!= (const Vector3& rkVector) const;
    size_t hashCode() const;
    bool fuzzyEq(const Vector3& other) const;
    bool fuzzyNe(const Vector3& other) const;

    bool isFinite() const;
    bool isZero() const;
    bool isUnit() const;
    
    
    Vector3 __fastcall operator+ (const Vector3& v) const;
    Vector3 __fastcall operator- (const Vector3& v) const;
    Vector3 __fastcall operator* (float s) const;
    inline Vector3 __fastcall operator/ (float s) const {
        return *this * (1.0f / s);
    }
    Vector3 __fastcall operator* (const Vector3& v) const;
    Vector3 __fastcall operator/ (const Vector3& v) const;
    Vector3 __fastcall operator- () const;

    
    Vector3& __fastcall operator+= (const Vector3& v);
    Vector3& __fastcall operator-= (const Vector3& v);
    Vector3& __fastcall operator*= (float s);
    inline Vector3& __fastcall operator/= (float s) {
        return (*this *= (1.0f / s));
    }
    Vector3& __fastcall operator*= (const Vector3& v);
    Vector3& __fastcall operator/= (const Vector3& v);
	float length() const;

    float magnitude() const;
    Vector3 direction() const;
    Vector3 fastDirection() const;
    Vector3 reflectAbout(const Vector3& normal) const;
    Vector3 reflectionDirection(const Vector3& normal) const;
    inline Vector3 directionOrZero() const {
        float mag = magnitude();
        if (mag < 0.0000001f) {
            return Vector3::zero();
        } else if (mag < 1.00001f && mag > 0.99999f) {
            return *this;
        } else {
            return *this * (1.0f / mag);
        }
    }
    Vector3 refractionDirection(
        const Vector3&  normal,
        float           iInside,
        float           iOutside) const;
    inline Vector3 unit() const {
        return direction();
    }

    inline Vector3 fastUnit() const {
        return fastDirection();
    }
    float squaredLength() const;

    float squaredMagnitude () const;
	
    float __fastcall dot(const Vector3& rkVector) const;
    
    float unitize(float tolerance = 1e-06);

    

    Vector3 __fastcall cross(const Vector3& rkVector) const;
    Vector3 unitCross(const Vector3& rkVector) const;

    Vector3 __fastcall min(const Vector3 &v) const;
    Vector3 __fastcall max(const Vector3 &v) const;

    inline float min() const {
        return BKMath::min(BKMath::min(x, y), z);
    }

    inline float max() const {
        return BKMath::max(BKMath::max(x, y), z);
    }

    std::string toString() const;

    inline Vector3 clamp(const Vector3& low, const Vector3& high) const {
        return Vector3(
            BKMath::clamp(x, low.x, high.x),
            BKMath::clamp(y, low.y, high.y),
            BKMath::clamp(z, low.z, high.z));
    }

    inline Vector3 clamp(float low, float high) const {
        return Vector3(
            BKMath::clamp(x, low, high),
            BKMath::clamp(y, low, high),
            BKMath::clamp(z, low, high));
    }

    

    inline Vector3 lerp(const Vector3& v, float alpha) const {
        return (*this) + (v - *this) * alpha; 
    }  

    static void orthonormalize (Vector3 akVector[3]);
    static void generateOrthonormalBasis (Vector3& rkU, Vector3& rkV,
                                          Vector3& rkW, bool bUnitLengthW = true);

    inline float sum() const {
        return x + y + z;
    }

    inline float average() const {
        return sum() / 3.0f;
    }

    
    static const Vector3& zero();
    static const Vector3& one();
    static const Vector3& unitX();
    static const Vector3& unitY();
    static const Vector3& unitZ();
    static const Vector3& inf();
    static const Vector3& nan();
    static const Vector3& minFinite();
    static const Vector3& maxFinite();

    inline void getTangents(Vector3& X, Vector3& Y) const {
        assert(BKMath::fuzzyEq(length(), 1.0f), 
                     "makeAxes requires Z to have unit length");
        
        
        X = (abs(x) < 0.9f) ? Vector3::unitX() : Vector3::unitY();
        
        
        X -= *this * this->dot(X);
        X /= X.length();
    
        Y = this->cross(X);
    }

    static Vector3& ignore();
};

inline BKMath::Vector3 operator*(float s, const BKMath::Vector3& v) {
    return v * s;
}

inline BKMath::Vector3 operator*(double s, const BKMath::Vector3& v) {
    return v * (float)s;
}

inline BKMath::Vector3 operator*(int s, const BKMath::Vector3& v) {
    return v * (float)s;
}


inline Vector3::Vector3() : x(0.0f), y(0.0f), z(0.0f) {
}


inline Vector3::Vector3 (float fX, float fY, float fZ) : x(fX), y(fY), z(fZ) {
}

inline Vector3::Vector3 (float V[3]) : x(V[0]), y(V[1]), z(V[2]){
}

inline Vector3::Vector3 (double V[3]) : x((float)V[0]), y((float)V[1]), z((float)V[2]){
}

inline const float& Vector3::operator[] (int i) const {
    return ((float*)this)[i];
}

inline float& Vector3::operator[] (int i) {
    return ((float*)this)[i];
}


inline Vector3& Vector3::operator= (const Vector3& rkVector) {
    x = rkVector.x;
    y = rkVector.y;
    z = rkVector.z;
    return *this;
}


inline bool Vector3::fuzzyEq(const Vector3& other) const {
    return BKMath::fuzzyEq((*this - other).squaredMagnitude(), 0);
}


inline bool Vector3::fuzzyNe(const Vector3& other) const {
    return BKMath::fuzzyNe((*this - other).squaredMagnitude(), 0);
}


inline bool Vector3::isFinite() const {
    return BKMath::isFinite(x) && BKMath::isFinite(y) && BKMath::isFinite(z);
}

inline bool Vector3::operator== (const Vector3& rkVector) const {
    return ( x == rkVector.x && y == rkVector.y && z == rkVector.z );
}

inline bool Vector3::operator!= (const Vector3& rkVector) const {
    return ( x != rkVector.x || y != rkVector.y || z != rkVector.z );
}

inline Vector3 Vector3::operator+ (const Vector3& rkVector) const {
    return Vector3(x + rkVector.x, y + rkVector.y, z + rkVector.z);
}

inline Vector3 Vector3::operator- (const Vector3& rkVector) const {
    return Vector3(x - rkVector.x, y - rkVector.y, z - rkVector.z);
}

inline Vector3 Vector3::operator* (const Vector3& rkVector) const {
    return Vector3(x * rkVector.x, y * rkVector.y, z * rkVector.z);
}

inline Vector3 Vector3::operator*(float f) const {
    return Vector3(x * f, y * f, z * f);
}

inline Vector3 Vector3::operator/ (const Vector3& rkVector) const {
    return Vector3(x / rkVector.x, y / rkVector.y, z / rkVector.z);
}

inline Vector3 Vector3::operator- () const {
    return Vector3(-x, -y, -z);
}

inline Vector3& Vector3::operator+= (const Vector3& rkVector) {
    x += rkVector.x;
    y += rkVector.y;
    z += rkVector.z;
    return *this;
}

inline Vector3& Vector3::operator-= (const Vector3& rkVector) {
    x -= rkVector.x;
    y -= rkVector.y;
    z -= rkVector.z;
    return *this;
}

inline Vector3& Vector3::operator*= (float fScalar) {
    x *= fScalar;
    y *= fScalar;
    z *= fScalar;
    return *this;
}

inline Vector3& Vector3::operator*= (const Vector3& rkVector) {
    x *= rkVector.x;
    y *= rkVector.y;
    z *= rkVector.z;
    return *this;
}

inline Vector3& Vector3::operator/= (const Vector3& rkVector) {
    x /= rkVector.x;
    y /= rkVector.y;
    z /= rkVector.z;
    return *this;
}

inline float Vector3::squaredMagnitude () const {
    return x*x + y*y + z*z;
}

inline float Vector3::squaredLength () const {
    return squaredMagnitude();
}

inline float Vector3::magnitude() const {
    return ::sqrtf(x*x + y*y + z*z);
}

inline float Vector3::length() const {
    return magnitude();
}

inline Vector3 Vector3::direction () const {
    const float lenSquared = squaredMagnitude();
    const float invSqrt = 1.0f / sqrtf(lenSquared);
    return Vector3(x * invSqrt, y * invSqrt, z * invSqrt);
}


inline Vector3 Vector3::fastDirection () const {
    float lenSquared = x * x + y * y + z * z;
    float invSqrt = rsq(lenSquared);
    return Vector3(x * invSqrt, y * invSqrt, z * invSqrt);
}

inline float Vector3::dot (const Vector3& rkVector) const {
    return x*rkVector.x + y*rkVector.y + z*rkVector.z;
}

inline Vector3 Vector3::cross (const Vector3& rkVector) const {
    return Vector3(y*rkVector.z - z*rkVector.y, z*rkVector.x - x*rkVector.z,
                   x*rkVector.y - y*rkVector.x);
}

inline Vector3 Vector3::unitCross (const Vector3& rkVector) const {
    Vector3 kCross(y*rkVector.z - z*rkVector.y, z*rkVector.x - x*rkVector.z,
                   x*rkVector.y - y*rkVector.x);
    kCross.unitize();
    return kCross;
}

inline Vector3 Vector3::min(const Vector3 &v) const {
    return Vector3(BKMath::min(v.x, x), BKMath::min(v.y, y), BKMath::min(v.z, z));
}

inline Vector3 Vector3::max(const Vector3 &v) const {
    return Vector3(BKMath::max(v.x, x), BKMath::max(v.y, y), BKMath::max(v.z, z));
}

inline bool Vector3::isZero() const {
    return BKMath::fuzzyEq(squaredMagnitude(), 0.0f);
}


inline bool Vector3::isUnit() const {
    return BKMath::fuzzyEq(squaredMagnitude(), 1.0f);
}

} 


