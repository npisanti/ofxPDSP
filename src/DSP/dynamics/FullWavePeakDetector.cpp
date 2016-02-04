
#include "FullWavePeakDetector.h"


float pdsp::FullWavePeakDetector::formula(const float &x) noexcept {
        if(x<0.0f){
                return -x;
        }else{
                return x;
        }
}


void pdsp::FullWavePeakDetector::formulaAudioRate(float* &output, const float* &input, const int &bufferSize) noexcept {
        ofx_Aeq_absB(output, input, bufferSize);
}
