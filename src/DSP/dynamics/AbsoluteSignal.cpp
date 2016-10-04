
#include "AbsoluteSignal.h"


float pdsp::AbsoluteSignal::formula(const float &x) noexcept {
        if(x<0.0f){
                return -x;
        }else{
                return x;
        }
}


void pdsp::AbsoluteSignal::formulaAudioRate(float* &output, const float* &input, const int &bufferSize) noexcept {
        ofx_Aeq_absB(output, input, bufferSize);
}
