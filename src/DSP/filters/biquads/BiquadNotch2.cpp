
#include "BiquadNotch2.h"



void pdsp::BiquadNotch2::cookCoefficients(const float &sampleRate, const float &frequency, const float &Q, const float dBgain){

    float omega = (M_TAU * frequency) / sampleRate;
    float sinW = sinf(omega);
    float cosW = cosf(omega);
    float alpha = sinW / (Q*2.0f);

    b0 = b2 = 1.0f;
    b1 = a1 = -2.0f*cosW;
    float a0 = 1.0f + alpha;
    a2 = 1.0f - alpha;

    //then divide everything for a0
    float oneSlashA0 = 1.0f / a0;
    b0 *= oneSlashA0;
    b1 *= oneSlashA0;
    b2 *= oneSlashA0;
    a1 *= oneSlashA0;
    a2 *= oneSlashA0;

}
