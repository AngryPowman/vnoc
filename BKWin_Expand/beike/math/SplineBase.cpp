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
#include "Spline.h"

namespace BKMath {

float SplineBase::getFinalInterval() const {
    if (! cyclic) {
        return 0;
    } else if (finalInterval <= 0) {
        int N = time.size();
        if (N >= 2) {
            return (time[1] - time[0] + time[N - 1] - time[N - 2]) * 0.5f;
        } else {
            return 1.0f;
        }
    } else {
        return finalInterval;
    }
}


Matrix4 SplineBase::computeBasis() {
 
    Matrix4 basis =
        Matrix4( -1,  3, -3,  1,
                  2, -5,  4, -1,
                 -1,  0,  1,  0,
                  0,  2,  0,  0) * 0.5f;

    
    basis.setColumn(0, -basis.column(0));

    
    basis.setColumn(1, basis.column(1) + basis.column(3));

    
    basis.setColumn(2, basis.column(2) - basis.column(0));

    return basis;
}


float SplineBase::duration() const {
    if (time.size() == 0) {
        return 0;
    } else {
        return time.back() - time[0] + getFinalInterval();
    }
}
    

void SplineBase::computeIndexInBounds(float s, int& i, float& u) const {
    int N = time.size();
    float t0 = time[0];
    float tn = time[N - 1];
    
    i = iFloor((N - 1) * (s - t0) / (tn - t0));
    
    
    int hi = N - 1;
    int lo = 0;
    
    while ((time[i] > s) || (time[i + 1] <= s)) {
        
        if (time[i] > s) {
            
            hi = i - 1;
        } else if (time[i + 1] <= s) {
            
            lo = i + 1;
        }
        
        i = (hi + lo) / 2;
    }
    
    
    u = (s - time[i]) / (time[i + 1] - time[i]);
}


void SplineBase::computeIndex(float s, int& i, float& u) const {
    int N = time.size();
    assert(N > 0, "No control points");
    float t0 = time[0];
    float tn = time[N - 1];
    
    if (N < 2) {
        
        i = 0;
        u = 0.0;
    } else if (cyclic) {
        float fi = getFinalInterval();
        
        
        if ((s < t0) || (s >= tn + fi)) {
            
            
            

            float d = duration();
            
            int wraps = iFloor((s - t0) / d);
            
            assert(s - d * wraps >= t0);
            assert(s - d * wraps < tn + getFinalInterval());

            computeIndex(s - d * wraps, i, u);
            i += wraps * N;
            
        } else if (s >= tn) {
            assert(s < tn + fi);
            
            i = N - 1;
            u = (s - tn) / fi;
            
        } else {
            
            computeIndexInBounds(s, i, u);
        }
        
    } else {
        
        
        if (s < t0) {
            
            
            
            float dt = time[1] - t0;
            float x = (s - t0) / dt;
            i = iFloor(x);
            u = x - i;
            
        } else if (s >= tn) {
            
            
            
            float dt = tn - time[N - 2];
            float x = N - 1 + (s - tn) / dt;
            i = iFloor(x);
            u = x - i;  
            
        } else {
    
            computeIndexInBounds(s, i, u);
            
        } 
    } 
}

}

