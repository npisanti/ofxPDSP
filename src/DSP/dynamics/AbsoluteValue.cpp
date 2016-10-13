
#include "AbsoluteValue.h"


float pdsp::AbsoluteValue::formula(const float &x) noexcept {
        if(x<0.0f){
                return -x;
        }else{
                return x;
        }
}


void pdsp::AbsoluteValue::formulaAudioRate(float* &output, const float* &input, const int &bufferSize) noexcept {
        ofx_Aeq_absB(output, input, bufferSize);
}
