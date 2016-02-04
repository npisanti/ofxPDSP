
#include "CheapTri.h"


void pdsp::CheapTri::oscillate( float* outputBuffer, const float* phaseBuffer, int bufferSize) noexcept{
        //oscillator DSP
        genTriangle(outputBuffer, phaseBuffer, bufferSize);
}
