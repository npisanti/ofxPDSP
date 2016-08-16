
#include "PitchToFreq.h"

float pdsp::PitchToFreq::freqCalculationMultiplier = 6.875f; //440hz
float pdsp::PitchToFreq::tuning = 440.0f;

pdsp::PitchToFreq::PitchToFreq() {
    setTuning( 440.0f );
}

float pdsp::PitchToFreq::formula(const float &x) noexcept {
    float fc;
    vect_calculateIncrement(&fc, &x, freqCalculationMultiplier , 1);
    return fc;
}

void pdsp::PitchToFreq::formulaAudioRate(float* &output, const float* &input, const int &bufferSize) noexcept {
    vect_calculateIncrement(output, input, freqCalculationMultiplier, bufferSize);
}


void pdsp::PitchToFreq::setTuning(float tuningFrequency){
    tuning = tuningFrequency;
    freqCalculationMultiplier = pow(2.0, -69.0/12.0) * tuningFrequency;
}


float pdsp::PitchToFreq::eval(float value){
    //return powf(2.0f, value) * freqCalculationMultiplier;
    float fc;
    vect_calculateIncrement(&fc, &value, freqCalculationMultiplier , 1);
    return fc;
}

float pdsp::PitchToFreq::getTuningFrequency() {
    return tuning;
}
