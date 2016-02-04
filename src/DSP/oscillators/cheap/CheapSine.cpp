

#include "CheapSine.h"

const float pdsp::CheapSine::cheapSineB = 4.0f / M_PI;
const float pdsp::CheapSine::cheapSineC = -4.0f / (M_PI * M_PI);
const float pdsp::CheapSine::cheapSineP = 0.225f;


void pdsp::CheapSine::oscillate(float* outputBuffer, const float* phaseBuffer, int bufferSize) noexcept{
        
        int n=0;

#ifdef VHID_USE_SIMD
        static f128 sineB = m_set1(cheapSineB);
        static f128 sineC = m_set1(cheapSineC);

        int maxSimd = ROUND_DOWN(bufferSize, 8);

        for (; n<maxSimd; n+=8) {

                f128 result1 = m_load(phaseBuffer + n);
                f128 result2 = m_load(phaseBuffer + n + 4);

                result1 = m_mul1(result1, 2.0f);
                result2 = m_mul1(result2, 2.0f);
                result1 = m_sub1(result1, 1.0f);
                result2 = m_sub1(result2, 1.0f);
                result1 = m_mul1(result1, M_PI);
                result2 = m_mul1(result2, M_PI);

                f128 result1b = m_mul(result1, sineB);
                f128 result2b = m_mul(result2, sineB);
                f128 result1c = m_abs(result1);
                f128 result2c = m_abs(result2);
                result1c = m_mul(result1c, sineC);
                result2c = m_mul(result2c, sineC);
                result1c = m_mul(result1c, result1);
                result2c = m_mul(result2c, result2);
                result1 = m_add(result1b, result1c);
                result2 = m_add(result2b, result2c);

                m_store( outputBuffer + n , result1);
                m_store( outputBuffer + n +4, result2);
        }
#endif

        for (; n<bufferSize; ++n) {
                outputBuffer[n] = (phaseBuffer[n] * 2.0f) - 1.0f;
                outputBuffer[n] *= M_PI;
                outputBuffer[n] = cheapSineB * outputBuffer[n] + cheapSineC * outputBuffer[n] * std::abs(outputBuffer[n]);
        }
}

/*

template<> void CheapSineT<true>::oscillate(float* outputBuffer, const float* phaseBuffer, int bufferSize) noexcept{

        int n=0;

        #ifdef VHID_USE_SIMD

        static f128 sineB = m_set1(cheapSineB);
        static f128 sineC = m_set1(cheapSineC);
        static f128 sineP = m_set1(cheapSineC);

        int maxSimd = ROUND_DOWN(bufferSize, 8);

        for (; n<maxSimd; n+=8) {

                f128 result1 = m_load(phaseBuffer + n);
                f128 result2 = m_load(phaseBuffer + n + 4);

                result1 = m_mul1(result1, 2.0f);
                result2 = m_mul1(result2, 2.0f);
                result1 = m_sub1(result1, 1.0f);
                result2 = m_sub1(result2, 1.0f);
                result1 = m_mul1(result1, M_PI);
                result2 = m_mul1(result2, M_PI);

                f128 result1b = m_mul(result1, sineB);
                f128 result2b = m_mul(result2, sineB);
                f128 result1c = m_abs(result1);
                f128 result2c = m_abs(result2);
                result1c = m_mul(result1c, sineC);
                result2c = m_mul(result2c, sineC);
                result1c = m_mul(result1c, result1);
                result2c = m_mul(result2c, result2);
                result1 = m_add(result1b, result1c);
                result2 = m_add(result2b, result2c);

                result1b = m_abs(result1);
                result2b = m_abs(result2);
                result1b = m_mul(result1b, result1);
                result2b = m_mul(result2b, result2);
                result1b = m_sub(result1b, result1);
                result2b = m_sub(result2b, result2);
                result1b = m_mul(result1b, sineP);
                result2b = m_mul(result2b, sineP);
                result1b = m_add(result1b, result1);
                result2b = m_add(result2b, result2);

                m_store( outputBuffer + n , result1b);
                m_store( outputBuffer + n +4, result2b);

        }
        #endif

        for (; n<bufferSize; ++n) {
                outputBuffer[n] = (phaseBuffer[n] * 2.0) - 1.0;
                outputBuffer[n] *= M_PI;

                outputBuffer[n] = cheapSineB * outputBuffer[n] + cheapSineC * outputBuffer[n] * std::abs(outputBuffer[n]);
                //higher precision
                outputBuffer[n] = cheapSineP * ( (outputBuffer[n] * std::abs(outputBuffer[n])) - outputBuffer[n]) + outputBuffer[n];
        }

}
*/
