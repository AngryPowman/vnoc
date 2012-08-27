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


#include <limits>
#include <stdlib.h>
#include "Vector3.h"
#include "math.h"
#include "Matrix3.h"
#include "Vector2.h"
#include "Vector4.h"

 
namespace BKMath {

Vector3::Vector3(const class Vector2& v, float _z) : x(v.x), y(v.y), z(_z) {
}

Vector3& Vector3::ignore() {
    static Vector3 v;
    return v;
}

const Vector3& Vector3::zero() { static const Vector3 v(0, 0, 0); return v; }
const Vector3& Vector3::one()      { static const Vector3 v(1, 1, 1); return v; }
const Vector3& Vector3::unitX()    { static const Vector3 v(1, 0, 0); return v; }
const Vector3& Vector3::unitY()    { static const Vector3 v(0, 1, 0); return v; }
const Vector3& Vector3::unitZ()    { static const Vector3 v(0, 0, 1); return v; }
const Vector3& Vector3::inf()      { static const Vector3 v((float)BKMath::finf(), (float)BKMath::finf(), (float)BKMath::finf()); return v; }
const Vector3& Vector3::nan()      { static const Vector3 v((float)BKMath::fnan(), (float)BKMath::fnan(), (float)BKMath::fnan()); return v; }
const Vector3& Vector3::minFinite(){ static const Vector3 v(-FLT_MAX, -FLT_MAX, -FLT_MAX); return v; }
const Vector3& Vector3::maxFinite(){ static const Vector3 v(FLT_MAX, FLT_MAX, FLT_MAX); return v; }

Vector3::Axis Vector3::primaryAxis() const {
    
    Axis a = X_AXIS;

    double nx = abs(x);
    double ny = abs(y);
    double nz = abs(z);

    if (nx > ny) {
        if (nx > nz) {
            a = X_AXIS;
        } else {
            a = Z_AXIS;
        }
    } else {
        if (ny > nz) {
            a = Y_AXIS;
        } else {
            a = Z_AXIS;
        }
    }

    return a;
}

double frand() {
    return rand() / (double) RAND_MAX;
}


float Vector3::unitize(float fTolerance) {
    float fMagnitude = magnitude();

    if (fMagnitude > fTolerance) {
        float fInvMagnitude = 1.0f / fMagnitude;
        x *= fInvMagnitude;
        y *= fInvMagnitude;
        z *= fInvMagnitude;
    } else {
        fMagnitude = 0.0f;
    }

    return fMagnitude;
}


Vector3 Vector3::reflectAbout(const Vector3& normal) const {
    Vector3 out;

    Vector3 N = normal.direction();

    
    return N * 2 * this->dot(N) - *this;
}





Vector3 Vector3::reflectionDirection(const Vector3& normal) const {
    return -reflectAbout(normal).direction();
}


Vector3 Vector3::refractionDirection(
    const Vector3&  normal,
    float           iInside,
    float           iOutside) const {
    const Vector3 W = -direction();
    Vector3 N = normal.direction();

    float h1 = iOutside;
    float h2 = iInside;

    if (normal.dot(*this) > 0.0f) {
        h1 = iInside;
        h2 = iOutside;
        N  = -N;
    }

    const float hRatio = h1 / h2;
    const float WdotN = W.dot(N);

    float det = 1.0f - (float)square(hRatio) * (1.0f - (float)square(WdotN));

    if (det < 0) {
        
        return Vector3::zero();
    } else {
        return -hRatio * (W - WdotN * N) - N * sqrt(det);
    }
}

void Vector3::orthonormalize (Vector3 akVector[3]) {
   
    akVector[0].unitize();

    
	float fDot0 = akVector[0].dot(akVector[1]);
    akVector[1] -= akVector[0] * fDot0;
    akVector[1].unitize();

    
	float fDot1 = akVector[1].dot(akVector[2]);
    fDot0 = akVector[0].dot(akVector[2]);
    akVector[2] -= akVector[0] * fDot0 + akVector[1] * fDot1;
    akVector[2].unitize();
}

void Vector3::generateOrthonormalBasis (Vector3& rkU, Vector3& rkV,
                                        Vector3& rkW, bool bUnitLengthW) {
    if ( !bUnitLengthW )
        rkW.unitize();

    if ( BKMath::abs(rkW.x) >= BKMath::abs(rkW.y)
            && BKMath::abs(rkW.x) >= BKMath::abs(rkW.z) ) {
        rkU.x = -rkW.y;
        rkU.y = + rkW.x;
        rkU.z = 0.0;
    } else {
        rkU.x = 0.0;
        rkU.y = + rkW.z;
        rkU.z = -rkW.y;
    }

    rkU.unitize();
    rkV = rkW.cross(rkU);
}

} 

