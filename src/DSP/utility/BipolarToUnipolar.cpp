
#include "BipolarToUnipolar.h"

float pdsp::BipolarToUnipolar::formula(const float &x) noexcept {
    return (x*0.5f + 0.5f); 
}

void pdsp::BipolarToUnipolar::formulaAudioRate(float* &output, const float* &input, const int &bufferSize) noexcept {
    ofx_Aeq_BmulS(output, input, 0.5f, bufferSize);
    ofx_Aeq_BaddS(output, output, 0.5f, bufferSize);
}

