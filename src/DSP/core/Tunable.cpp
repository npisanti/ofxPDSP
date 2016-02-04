
#include "Tunable.h"


float pdsp::Tunable::freqCalculationMultiplier = 6.875f;


void pdsp::setReferenceFrequency(float frequency) {
    Tunable::freqCalculationMultiplier = pow(2.0, -69.0/12.0) * frequency;
}

float pdsp::p2f (float frequency){
    float toReturn;
    vect_pitchToFreq(&toReturn, &frequency, 1);
    return toReturn;
}
