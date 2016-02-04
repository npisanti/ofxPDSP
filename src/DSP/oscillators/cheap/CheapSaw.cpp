
#include "CheapSaw.h"



void pdsp::CheapSaw::oscillate( float* outputBuffer, const float* phaseBuffer, int bufferSize) noexcept {
    genSaw(outputBuffer, phaseBuffer, bufferSize);
}
