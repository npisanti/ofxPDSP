#include "FreqToMs.h"

float pdsp::FreqToMs::formula(const float &x) noexcept {
    return (1000.0f/ x); 
}

void pdsp::FreqToMs::formulaAudioRate(float* &output, const float* &input, const int &bufferSize) noexcept {
    ofx_Aeq_1divB(output, input, bufferSize);
    ofx_Aeq_BmulS(output, output, 1000.0f, bufferSize);
}

float eval(float value){
    return (1000.0f/ value); 
}
