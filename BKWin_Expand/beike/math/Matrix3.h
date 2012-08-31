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


#ifndef BKMath_Matrix3_h
#define BKMath_Matrix3_h

#include "Vector3.h"
#include "Vector4.h"
#include "assert.h"

#include <cstring>
#pragma once
namespace BKMath {

#ifdef _MSC_VER
#   pragma warning (disable : 4127)
#endif
class Matrix3 {
private:

    float elt[3][3];
    bool operator<(const Matrix3&) const;
    bool operator>(const Matrix3&) const;
    bool operator<=(const Matrix3&) const;
    bool operator>=(const Matrix3&) const;

public:
    inline Matrix3() {}
    Matrix3 (const float aafEntry[3][3]);
    Matrix3 (const Matrix3& rkMatrix);
    Matrix3 (float fEntry00, float fEntry01, float fEntry02,
             float fEntry10, float fEntry11, float fEntry12,
             float fEntry20, float fEntry21, float fEntry22);

	bool fuzzyEq(const Matrix3& b) const;
    bool isRightHanded() const;
    void set(float fEntry00, float fEntry01, float fEntry02,
             float fEntry10, float fEntry11, float fEntry12,
             float fEntry20, float fEntry21, float fEntry22);
    inline float* operator[] (int iRow) {
        assert(iRow >= 0);
        assert(iRow < 3);
        return (float*)&elt[iRow][0];
    }

    inline const float* operator[] (int iRow) const {
        assert(iRow >= 0);
        assert(iRow < 3);
        return (const float*)&elt[iRow][0];
    }

    inline operator float* () {
        return (float*)&elt[0][0];
    }

    inline operator const float* () const{
        return (const float*)&elt[0][0];
    }
    
    Vector3 column(int c) const;
    const Vector3& row(int r) const;

    void setColumn(int iCol, const Vector3 &vector);
    void setRow(int iRow, const Vector3 &vector);

    
    inline Matrix3& operator= (const Matrix3& rkMatrix) {
        memcpy(elt, rkMatrix.elt, 9 * sizeof(float));
        return *this;
    }

    bool operator== (const Matrix3& rkMatrix) const;
    bool operator!= (const Matrix3& rkMatrix) const;

    
    Matrix3 operator+ (const Matrix3& rkMatrix) const;
    Matrix3 operator- (const Matrix3& rkMatrix) const;
    

    Matrix3 operator* (const Matrix3& rkMatrix) const;
    Matrix3 operator- () const;

    Matrix3& operator+= (const Matrix3& rkMatrix);
    Matrix3& operator-= (const Matrix3& rkMatrix);
    Matrix3& operator*= (const Matrix3& rkMatrix);

    

    inline Vector3 operator* (const Vector3& v) const {
        Vector3 kProd;

        for (int r = 0; r < 3; ++r) {
            kProd[r] =
                elt[r][0] * v[0] +
                elt[r][1] * v[1] +
                elt[r][2] * v[2];
        }

        return kProd;
    }


    

    friend Vector3 operator* (const Vector3& rkVector,
                              const Matrix3& rkMatrix);

    

    Matrix3 operator* (float fScalar) const;

    

    friend Matrix3 operator* (double fScalar, const Matrix3& rkMatrix);
    friend Matrix3 operator* (float fScalar, const Matrix3& rkMatrix);
    friend Matrix3 operator* (int fScalar, const Matrix3& rkMatrix);

    Matrix3& operator*= (float k);
    Matrix3& operator/= (float k);


private:
    

    static void _mul(const Matrix3& A, const Matrix3& B, Matrix3& out);
public:
    inline static void mul(const Matrix3& A, const Matrix3& B, Matrix3& out) {
        if ((&out == &A) || (&out == &B)) {
            
            out = A * B;
        } else {
            
            _mul(A, B, out);
        }
    }

private:
    static void _transpose(const Matrix3& A, Matrix3& out);
public:
    inline static void transpose(const Matrix3& A, Matrix3& out) {
        if (&A == &out) {
            out = A.transpose();
        } else {
            _transpose(A, out);
        }
    }
    bool isOrthonormal() const;

    Matrix3 transpose () const;
    bool inverse (Matrix3& rkInverse, float fTolerance = 1e-06) const;
    Matrix3 inverse (float fTolerance = 1e-06) const;
    float determinant () const;
    void singularValueDecomposition (Matrix3& rkL, Vector3& rkS,
                                     Matrix3& rkR) const;
    void singularValueComposition (const Matrix3& rkL,
                                   const Vector3& rkS, const Matrix3& rkR);
    void orthonormalize();
    void qDUDecomposition (Matrix3& rkQ, Vector3& rkD,
                           Vector3& rkU) const;
    void polarDecomposition(Matrix3 &R, Matrix3 &S) const;

	float spectralNorm () const;

    float squaredFrobeniusNorm() const;

    float frobeniusNorm() const;

    float l1Norm() const;

    float lInfNorm() const;

    float diffOneNorm(const Matrix3 &y) const;

    

    void toAxisAngle(Vector3& rkAxis, float& rfRadians) const;

    static Matrix3 fromDiagonal(const Vector3& d) {
        return Matrix3(d.x, 0, 0, 
                       0, d.y, 0,
                       0, 0, d.z);
    }

    static Matrix3 fromAxisAngle(const Vector3& rkAxis, float fRadians);

    

    bool toEulerAnglesXYZ (float& rfYAngle, float& rfPAngle,
                           float& rfRAngle) const;
    bool toEulerAnglesXZY (float& rfYAngle, float& rfPAngle,
                           float& rfRAngle) const;
    bool toEulerAnglesYXZ (float& rfYAngle, float& rfPAngle,
                           float& rfRAngle) const;
    bool toEulerAnglesYZX (float& rfYAngle, float& rfPAngle,
                           float& rfRAngle) const;
    bool toEulerAnglesZXY (float& rfYAngle, float& rfPAngle,
                           float& rfRAngle) const;
    bool toEulerAnglesZYX (float& rfYAngle, float& rfPAngle,
                           float& rfRAngle) const;
    static Matrix3 fromEulerAnglesXYZ (float fYAngle, float fPAngle, float fRAngle);
    static Matrix3 fromEulerAnglesXZY (float fYAngle, float fPAngle, float fRAngle);
    static Matrix3 fromEulerAnglesYXZ (float fYAngle, float fPAngle, float fRAngle);
    static Matrix3 fromEulerAnglesYZX (float fYAngle, float fPAngle, float fRAngle);
    static Matrix3 fromEulerAnglesZXY (float fYAngle, float fPAngle, float fRAngle);
    static Matrix3 fromEulerAnglesZYX (float fYAngle, float fPAngle, float fRAngle);

    

    void eigenSolveSymmetric (float afEigenvalue[3],
                              Vector3 akEigenvector[3]) const;

    static void tensorProduct (const Vector3& rkU, const Vector3& rkV,
                               Matrix3& rkProduct);

    static const float EPSILON; 
 
    static const Matrix3& zero();
    static const Matrix3& identity(); 

protected:

    
    void tridiagonal (float afDiag[3], float afSubDiag[3]);
    bool qLAlgorithm (float afDiag[3], float afSubDiag[3]);

    
    static const float ms_fSvdEpsilon;
    static const int ms_iSvdMaxIterations;
    static void bidiagonalize (Matrix3& kA, Matrix3& kL,
                               Matrix3& kR);
    static void golubKahanStep (Matrix3& kA, Matrix3& kL,
                                Matrix3& kR);

    
    static float maxCubicRoot (float afCoeff[3]);

};

inline Vector3 operator* (const Vector3& rkPoint, const Matrix3& rkMatrix) {
    Vector3 kProd;

    for (int r = 0; r < 3; ++r) {
        kProd[r] =
            rkPoint[0] * rkMatrix.elt[0][r] +
            rkPoint[1] * rkMatrix.elt[1][r] +
            rkPoint[2] * rkMatrix.elt[2][r];
    }

    return kProd;
}


} 

#endif


