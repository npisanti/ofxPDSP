

#include "LinToDB.h"

float pdsp::LinToDB::formula(const float &x) noexcept {
    if( x <= -90.0f ){  //we compute the if only at control rate
        return 0.0f;    //silence at -90db and less
    }else{
        return 20.0f*log10f(x); 
    }
}

void pdsp::LinToDB::formulaAudioRate(float* &output, const float* &input, const int &bufferSize) noexcept {
   
    ofx_Aeq_logSB(output, 10.0f, input, bufferSize);
    ofx_Aeq_BmulS(output, output, 20.0f, bufferSize);
}

float pdsp::LinToDB::eval(float value){
    return 20.0f*log10f(value); 
}
