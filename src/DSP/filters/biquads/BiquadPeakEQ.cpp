
#include "BiquadPeakEQ.h"



void pdsp::BiquadPeakEQ::cookCoefficients(const float &sampleRate, const float &frequency, const float &Q, const float dBgain){

    float A = pow(10.0, dBgain*0.025);

    float omega = (M_TAU * frequency) / sampleRate;
    float sinW = sinf(omega);
    float cosW = cosf(omega);
    float alpha = sinW / (Q*2.0f);

    b0 =        1 + alpha*A;
    b1 =        -2*cosW;
    b2 =        1 - alpha*A;
    float a0 =  1 + alpha/A;
    a1 =        -2*cosW;
    a2 =        1 - alpha/A;

    //then divide everything for a0
    float oneSlashA0 = 1.0f / a0;
    b0 *= oneSlashA0;
    b1 *= oneSlashA0;
    b2 *= oneSlashA0;
    a1 *= oneSlashA0;
    a2 *= oneSlashA0;
    
}
