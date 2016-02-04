
// incrementCalculator.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_MATH_INCREMENTCALCULATOR_H_INCLUDED
#define PDSP_MATH_INCREMENTCALCULATOR_H_INCLUDED

#include "../functions.h"


namespace pdsp{
    
    struct calc_inc_kernel
    {
    public:
        enum { BLOCK_SIZE = 8 }; // this defines how many elements of the array are processed by a single call to block()
        calc_inc_kernel(float incCalc) : incCalc(incCalc){};
        
        inline void operator()(float* y, const float* x) const {
            (*y) = powf(2.0, (*x)*divideByTwelve_f ) * incCalc ;
            //(*y) =  (*x)*divideByTwelve_f ;
        }
        
        inline void block(float* y, const float* x) const {
            // load 4 data elements at a time
            ofx::f128 X1 = ofx::m_load(x);
            ofx::f128 X2 = ofx::m_load(x+4);

            // do the computations
            ofx::f128 result1 = ofx::m_mul(X1, divideByTwelve);
            ofx::f128 result2 = ofx::m_mul(X2, divideByTwelve);
            
            //now elevate 2 to the results
            result1 = ofx::m_pow2(result1);
            result2 = ofx::m_pow2(result2);
            result1 = ofx::m_mul1(result1, incCalc);
            result2 = ofx::m_mul1(result2, incCalc);
            
            // store the results
   
            ofx::m_store(y, result1);
            ofx::m_store(y+4, result2);
            
        }
        
    protected:
        float incCalc;
    public:
        static ofx::f128 divideByTwelve;
        static const float divideByTwelve_f;
        
    };
    
    
    // inc multiplier is  pow(2.0, -69.0/12.0) * referenceFrequency / sampleRate, you can calculate it before
    inline_f void vect_calculateIncrement (float* incBuffer, const float* pitchBuffer, const float incMultiplier, const int len){
        
        ofx::transform_n(incBuffer, pitchBuffer, len, calc_inc_kernel(incMultiplier));
       
        
        /*for (int i=0; i<len; ++i) {
            incBuffer[i] = powf(2.0f, incBuffer[i]);
        }
        Aeq_AxS(incBuffer, incMultiplier, len);
        
        */
   
    }
    
}


#endif  // PDSP_MATH_INCREMENTCALCULATOR_H_INCLUDED
