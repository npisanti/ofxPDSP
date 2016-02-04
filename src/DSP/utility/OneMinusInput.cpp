#include "OneMinusInput.h"

float pdsp::OneMinusInput::formula(const float &x) noexcept {
    return (1.0f - x); 
}

void pdsp::OneMinusInput::formulaAudioRate(float* &output, const float* &input, const int &bufferSize) noexcept {
    ofx_Aeq_SsubB(output, 1.0f, input, bufferSize);
}

