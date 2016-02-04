
#include "Saturator1.h"

float pdsp::Saturator1::formula(const float &x) noexcept {
    float xCube = x*x*x;
    float a = x + (0.16489087f * xCube) + (0.00985468f * xCube*x*x);
    return (a / sqrtf(1.0f + a * a));
}
        
void pdsp::Saturator1::formulaAudioRate(float* &output, const float* &input, const int &bufferSize) noexcept {
   
    int n=0;
    int maxSimd = ROUND_DOWN(bufferSize, 8);

#ifdef OFX_SIMD_USE_SIMD
    for (; n<maxSimd; n+=8) {

        // load 8 data elements
        ofx::f128 x0 = ofx::m_load(input + n);
        ofx::f128 x4 = ofx::m_load(input + n +4);

        // do the computations
        ofx::f128 xCube0 = ofx::m_mul(x0, x0); //x^2
        ofx::f128 xCube4 = ofx::m_mul(x4, x4); //x^2

        xCube0 = ofx::m_mul(xCube0, x0); // x^3
        xCube4 = ofx::m_mul(xCube4, x4); // x^3

        ofx::f128 b0 = ofx::m_mul1(xCube0, 0.16489087f);
        ofx::f128 b4 = ofx::m_mul1(xCube4, 0.16489087f);

        ofx::f128 c0 = ofx::m_mul(xCube0, x0);
        ofx::f128 c4 = ofx::m_mul(xCube4, x4);

        c0 = ofx::m_mul(c0, x0); //x^5
        c4 = ofx::m_mul(c4, x4); //x^5

        c0 = ofx::m_mul1(c0, 0.0098546f);
        c4 = ofx::m_mul1(c0, 0.0098546f);

        ofx::f128 a0 = ofx::m_add(b0, c0);
        ofx::f128 a4 = ofx::m_add(b4, c4);

        a0 = ofx::m_add(a0, x0);
        a4 = ofx::m_add(a4, x4);

        x0 = ofx::m_mul(a0, a0);
        x4 = ofx::m_mul(a4, a4);

        x0 = ofx::m_add1(x0, 1.0f);
        x4 = ofx::m_add1(x4, 1.0f);

        x0 = ofx::m_rcp_square_root(x0);
        x4 = ofx::m_rcp_square_root(x4);

        x0 = ofx::m_mul(a0, x0);
        x4 = ofx::m_mul(a4, x4);

        // store the results
        ofx::m_store(output +n , x0);
        ofx::m_store(output +n +4, x4);

    }
#endif

    for (; n<bufferSize; ++n) {
            float x = input[n];
            float xCube = x*x*x;
            float a = x + (0.16489087f * xCube) + (0.00985468f * xCube*x*x);
            output[n] = (a / sqrtf(1.0f + a * a));
    }
    
}
     
