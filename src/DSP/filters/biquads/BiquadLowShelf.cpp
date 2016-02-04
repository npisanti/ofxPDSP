
#include "BiquadLowShelf.h"



void pdsp::BiquadLowShelf::cookCoefficients(const float &sampleRate, const float &frequency, const float &Q, const float dBgain){

    float A = pow(10.0, dBgain*0.025);

    float omega = (M_TAU * frequency) / sampleRate;
    float sinW = sinf(omega);
    float cosW = cosf(omega);
    //float alpha = sinW / (Q*2.0f);

    float beta = sqrtf(A) / Q;
    

    b0 =    A*( (A+1) - (A-1)*cosW + beta*sinW );
    b1 =  2*A*( (A-1) - (A+1)*cosW             );
    b2 =    A*( (A+1) - (A-1)*cosW - beta*sinW );
    float a0 =  (A+1) + (A-1)*cosW + beta*sinW;
    a1 =   -2*( (A-1) + (A+1)*cosW             );
    a2 =        (A+1) + (A-1)*cosW - beta*sinW;

    //then divide everything for a0
    float oneSlashA0 = 1.0f / a0;
    b0 *= oneSlashA0;
    b1 *= oneSlashA0;
    b2 *= oneSlashA0;
    a1 *= oneSlashA0;
    a2 *= oneSlashA0;
    
}
