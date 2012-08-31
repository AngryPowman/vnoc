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

#include <assert.h>
#include <vector>
#include "Matrix4.h"
#include "Vector4.h"
#include "math.h"
#pragma once
namespace BKMath {

class SplineBase {
public:

    std::vector<float>            time;
    bool                    cyclic;
    float                   finalInterval; 
    SplineBase() : cyclic(true), finalInterval(-1) {}
    virtual ~SplineBase() {}
    float getFinalInterval() const;
    float duration() const;
    static Matrix4 computeBasis();
    
protected:
    void computeIndexInBounds(float s, int& i, float& u) const;
public:
    void computeIndex(float s, int& i, float& u) const;
};
/*
Catmull-Rom分段样条曲线实现,参考:
1,real time rendering 3rd edition,第13章
2,http://www.gamedev.net/reference/articles/article1497.asp 

------------------------------------------------------*/

template<typename Control>
class Spline : public SplineBase {
protected:
    Control                 zero;
public:
    std::vector<Control>          control;
    Spline() {
        static Control x;     
        zero = *(&x) * 0.0f;
    }
    void append(float t, const Control& c) {
        assert((time.size() == 0) || (t > time.back()), 
                     "Control points must have monotonically increasing times.");
        time.push_back(t);
        control.push_back(c);
        assert(control.size() == time.size());
    }
    void append(const Control& c) {
        switch (time.size()) {
        case 0:
            append(0, c);
            break;

        case 1:
            if (time[0] == 0) {
                append(1, c);
            } else {
                append(time[0], c);
            }
            break;

        default:
            append(2 * time[time.size() - 1] - time[time.size() - 2], c);
        }
        assert(control.size() == time.size());
    }

    void clear() {
        control.clear();
        time.clear();
    }   

    int size() const {
        assert(time.size() == control.size());
        return control.size();
    }

    void getControl(int i, float& t, Control& c) const {
        int N = control.size();
        if (N == 0) {
            c = zero;
            t = 0;
        } else if (cyclic) {
            c = control[iWrap(i, N)];

            if (i < 0) {

                int wraps = (N + 1 - i) / N;                    
                int j = (i + wraps * N) % N;
                t = time[j] - wraps * duration();

            } else if (i < N) {

                t = time[i];

            } else {

                int wraps = i / N;                    
                int j = i % N;
                t = time[j] + wraps * duration();
            }

        } else if (i < 0) {
            
            if (N >= 2) {
                
                float dt = time[1] - time[0];
                
                
                c = control[1] * float(i) + control[0] * float(1 - i);
                correct(c);
                t = dt * i + time[0];

            } else { 
                c = control[0];
                t = time[0] + i;
            }

        } else if (i >= N) {
            if (N >= 2) {
                float dt = time[N - 1] - time[N - 2]; 
                c = control[N - 1] * float(i - N + 2) + control[N - 2] * -float(i - N + 1);
                correct(c);
                t = time[N - 1] + dt * (i - N + 1);

            } else {
                
                c = control.back();
                
                t = time[0] + i;
            }
        } else {
            
            c = control[i];
            t = time[i];
        }
    }

protected:
    void getControls(int i, float* T, Control* A, int N) const {
        for (int j = 0; j < N; ++j) {
            getControl(i + j, T[j], A[j]);
        }
        ensureShortestPath(A, N);
    }
    virtual void ensureShortestPath(Control* A, int N) const { (void)A; (void) N;}
    virtual void correct(Control& A) const { (void)A; }
public:
    Control evaluate(float s) const {
        assert(control.size() == time.size(), "Corrupt spline: wrong number of control points."); 
        int i = 0;   
        float u = 0;
        computeIndex(s, i, u);
        Control p[4];
        float   t[4];
        getControls(i - 1, t, p, 4);
        float dt0 = t[1] - t[0];
        float dt1 = t[2] - t[1];
        float dt2 = t[3] - t[2];
        static const Matrix4 basis = computeBasis(); 
        Vector4 uvec((float)(u*u*u), (float)(u*u), (float)u, 1.0f);
        const Vector4& weights = uvec * basis;
        Control sum;
        const Control& p0 = p[0];
        const Control& p1 = p[1];
        const Control& p2 = p[2];
        const Control& p3 = p[3];  
        float x = (dt0 + dt1) * 0.5f;
        float n0 = x / dt0;
        float n1 = x / dt1;
        float n2 = x / dt2;
        const Control& dp0 = p1 + (p0*-1.0f);
        const Control& dp1 = p2 + (p1*-1.0f);
        const Control& dp2 = p3 + (p2*-1.0f);
        const Control& dp1n1 = dp1 * n1;
        const Control& tan1 = dp0 * n0 + dp1n1;
        const Control& tan2 = dp1n1 + dp2 * n2;
        sum = 
            tan1 * weights[0] +
             p1  * weights[1] +
             p2  * weights[2] +
            tan2 * weights[3]; 
        correct(sum);
        return sum;
    }
};

}


