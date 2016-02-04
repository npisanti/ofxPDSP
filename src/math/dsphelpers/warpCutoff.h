
// warpCutoff.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_MATH_WARPCUTOFF_H_INCLUDED
#define PDSP_MATH_WARPCUTOFF_H_INCLUDED


#include "../functions.h"


namespace pdsp{
    
    struct warp_cutoff_kernel
    {
    public:
        enum { BLOCK_SIZE = 8 }; // this defines how many elements of the array are processed by a single call to block()
        warp_cutoff_kernel(float halfT, float twoSlashT) : halfT(halfT), twoSlashT(twoSlashT) {};
        
        inline void operator()(float* dest, const float* src) const {
            float wd = M_TAU * (*src); // at the moment there is fc inside outputBuffer
            (*dest) = twoSlashT * tanf(wd * halfT);
            
        }
        
        inline void block(float* dest, const float* src) const {
            // load 4 data elements at a time
            ofx::f128 result1 = ofx::m_load(src);
            ofx::f128 result2 = ofx::m_load(src+4);

            // do the computations
            result1 = ofx::m_mul1(result1, M_TAU);
            result2 = ofx::m_mul1(result2, M_TAU);
            result1 = ofx::m_mul1(result1, halfT);
            result2 = ofx::m_mul1(result2, halfT);
            result1 = ofx::m_tan(result1);
            result2 = ofx::m_tan(result2);
            result1 = ofx::m_mul1(result1, twoSlashT);
            result2 = ofx::m_mul1(result2, twoSlashT);
            
            // store the results
            ofx::m_store(dest, result1);
            ofx::m_store(dest+4, result2);
            
        }
        
    protected:
        float halfT;
        float twoSlashT;

    };
    
    // inc multiplier is  pow(2.0, -69.0/12.0) * referenceFrequency / sampleRate, you can calculate it before
    inline_f void vect_warpCutoff (float* dest, const float* freqBuffer, const float halfT, const float twoSlashT, const int len){
        //for (int n=0; n<len; ++n){
        //    float wd = M_TAU * freqBuffer[n];
        //    dest[n] = twoSlashT * tanf(wd * halfT);
        //}
        ofx::transform_n(dest, freqBuffer, len, warp_cutoff_kernel(halfT, twoSlashT));
    }
    
}



#endif  // PDSP_MATH_WARPCUTOFF_H_INCLUDED
