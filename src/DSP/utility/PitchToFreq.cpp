
#include "PitchToFreq.h"

float pdsp::PitchToFreq::freqCalculationMultiplier = 6.875f; //440hz


float pdsp::PitchToFreq::formula(const float &x) noexcept {
    float fc;
    vect_calculateIncrement(&fc, &x, freqCalculationMultiplier , 1);
    //vect_pitchToFreq(&fc, &x, 1);
    return fc;
}

void pdsp::PitchToFreq::formulaAudioRate(float* &output, const float* &input, const int &bufferSize) noexcept {
    vect_calculateIncrement(output, input, freqCalculationMultiplier, bufferSize);
    //vect_pitchToFreq(output, input, bufferSize);
}


void pdsp::PitchToFreq::setTuning(float tuningFrequency){
    freqCalculationMultiplier = pow(2.0, -69.0/12.0) * tuningFrequency;
}


float pdsp::PitchToFreq::eval(float value){
    float fc;
    vect_calculateIncrement(&fc, &value, freqCalculationMultiplier , 1);
    //vect_pitchToFreq(&fc, &x, 1);
    return fc;
}
