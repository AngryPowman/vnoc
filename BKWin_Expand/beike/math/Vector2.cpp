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
#include "Vector2.h"

namespace BKMath {

const Vector2& Vector2::one() { 
    static const Vector2 v(1, 1); return v; 
}


const Vector2& Vector2::zero() {
    static Vector2 v(0, 0);
    return v;
}

const Vector2& Vector2::unitX() {
    static Vector2 v(1, 0);
    return v;
}

const Vector2& Vector2::unitY() {
    static Vector2 v(0, 1);
    return v;
}

size_t Vector2::hashCode() const {
    unsigned int xhash = (*(int*)(void*)(&x));
    unsigned int yhash = (*(int*)(void*)(&y));

    return xhash + (yhash * 37);
}


Vector2 Vector2::operator/ (float k) const {
    return *this * (1.0f / k);
}

Vector2& Vector2::operator/= (float k) {
    this->x /= k;
    this->y /= k;
    return *this;
}

float Vector2::unitize (float fTolerance) {
	float fLength = length();

    if (fLength > fTolerance) {
		float fInvLength = 1.0f / fLength;
        x *= fInvLength;
        y *= fInvLength;
    } else {
        fLength = 0.0;
    }

    return fLength;
}


std::string Vector2::toString() const {
	std::string temp = "(";
	int dec,sign;
	temp = temp + _fcvt(x,5,&dec,&sign) + "," + _fcvt(y,5,&dec,&sign) +")";
    return temp;
}
} 

