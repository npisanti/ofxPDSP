

#include "DBtoLin.h"

float pdsp::DBtoLin::silenceThreshold = -48.0f;

void pdsp::DBtoLin::setSilenceThreshold( float silenceDB ){
    silenceThreshold = silenceDB;
}


float pdsp::DBtoLin::formula(const float &x) noexcept {
    if(x > silenceThreshold ){ 
        return pow(10.0f, x*0.05f);
    }else{
        return 0.0f;
    }
}


void pdsp::DBtoLin::formulaAudioRate(float* &output, const float* &input, const int &bufferSize) noexcept {
    ofx_Aeq_BmulS(output, input, 0.05f, bufferSize);  // input / 20
    ofx_Aeq_powSB(output, 10.0f, output, bufferSize);
}


float pdsp::DBtoLin::eval(float value){
    return pow(10.0f, value*0.05f);
}
