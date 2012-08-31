

#include "assert.h"
#include "Matrix3.h"
#include "Vector3.h"
#pragma once
namespace BKMath {



class Matrix4 {
private:

    float elt[4][4];

    

    float subDeterminant(int excludeRow, int excludeCol) const;

    
    bool operator<(const Matrix4&) const;
    bool operator>(const Matrix4&) const;
    bool operator<=(const Matrix4&) const;
    bool operator>=(const Matrix4&) const;

public:

    Matrix4(
        float r1c1, float r1c2, float r1c3, float r1c4,
        float r2c1, float r2c2, float r2c3, float r2c4,
        float r3c1, float r3c2, float r3c3, float r3c4,
        float r4c1, float r4c2, float r4c3, float r4c4);

    

    Matrix4(const float* init);
    
	

    Matrix4(const class Matrix3& upper3x3, const class Vector3& lastCol = Vector3::zero());


    Matrix4(const double* init);

    Matrix4();

    
    
    static const Matrix4& identity();
    static const Matrix4& zero();

    

    void getPerspectiveProjectionParameters
    (float& left,
     float& right,
     float& bottom,  
     float& top,
     float& nearval, 
     float& farval,
     float updirection = -1.0f) const;
        
    inline float* operator[](int r) {
        assert(r >= 0);
        assert(r < 4);
        return (float*)&elt[r];
    }

    inline const float* operator[](int r) const {
        assert(r >= 0);
        assert(r < 4);
        return (const float*)&elt[r];
    } 

    inline operator float* () {
        return (float*)&elt[0][0];
    }

    inline operator const float* () const {
        return (const float*)&elt[0][0];
    }

    Matrix4 operator*(const Matrix4& other) const;
    Matrix4 operator+(const Matrix4& other) const {
        Matrix4 result;
        for (int r = 0; r < 4; ++r) {
            for (int c = 0; c < 4; ++c) {
                result.elt[r][c] = elt[r][c] + other.elt[r][c];
            }
        }
        return result;
    }

    class Matrix3 upper3x3() const;

    

    class Vector3 homoMul(const class Vector3& v, float w) const;

    

    static Matrix4 orthogonalProjection(
        float            left,
        float            right,
        float            bottom,
        float            top,
        float            nearval,
        float            farval,
        float            upDirection = -1.0f);


    

    static Matrix4 perspectiveProjection(
        float            left,
        float            right,
        float            bottom,
        float            top,
        float            nearval,
        float            farval,
        float            upDirection = -1.0f);

    void setRow(int r, const class Vector4& v);
    void setColumn(int c, const Vector4& v);

    const Vector4& row(int r) const;
    Vector4 column(int c) const;

    Matrix4 operator*(const float s) const;
    Vector4 operator*(const Vector4& vector) const;

    Matrix4 transpose() const;

    bool operator!=(const Matrix4& other) const;
    bool operator==(const Matrix4& other) const;

    float determinant() const;
    Matrix4 inverse() const;

    

    Matrix4 adjoint() const;
    Matrix4 cofactor() const;

    std::string toString() const;

    

    inline static Matrix4 scale(const Vector3& v) {
        return Matrix4(v.x, 0, 0, 0,
                       0, v.y, 0, 0,
                       0, 0, v.z, 0,
                       0, 0, 0, 1);
    }
    
    

    inline static Matrix4 scale(float x, float y, float z) {
        return scale(Vector3(x, y, z));
    }

    

    inline static Matrix4 scale(float s) {
        return scale(s,s,s);
    }

    

    inline static Matrix4 translation(const Vector3& v) {
        return Matrix4(Matrix3::identity(), v);
    }

    inline static Matrix4 translation(float x, float y, float z) {
        return Matrix4(Matrix3::identity(), Vector3(x, y, z));
    }

     

    inline static Matrix4 yawDegrees(float deg) {
        return Matrix4(Matrix3::fromAxisAngle(Vector3::unitY(), toRadians(deg)));
    }

    inline static Matrix4 pitchDegrees(float deg) {
        return Matrix4(Matrix3::fromAxisAngle(Vector3::unitX(), toRadians(deg)));
    }

    inline static Matrix4 rollDegrees(float deg) {
        return Matrix4(Matrix3::fromAxisAngle(Vector3::unitZ(), toRadians(deg)));
    }
};



}

