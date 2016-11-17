
#include "Saturator2.h"

float pdsp::Saturator2::formula(const float &x) noexcept {
    return std::tanh(x);
}
        
void pdsp::Saturator2::formulaAudioRate(float* &output, const float* &input, const int &bufferSize) noexcept {
    for (int n=0; n<bufferSize; ++n) {
            output[n] = std::tanh(input[n]);
    }
}
     
