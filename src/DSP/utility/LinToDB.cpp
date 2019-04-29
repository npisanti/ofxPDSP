

#include "LinToDB.h"
#include <cfloat>

float pdsp::LinToDB::formula(const float &x) noexcept {
    if( x == 0.0f ){  //we compute the if only at control rate
        return 0.0f;
    }else{
        return 20.0f*log10f(x); 
    }
}

void pdsp::LinToDB::formulaAudioRate(float* &output, const float* &input, const int &bufferSize) noexcept {
   
    ofx_Aeq_logSB(output, 10.0f, input, bufferSize);
    ofx_Aeq_BmulS(output, output, 20.0f, bufferSize);
    
    // this could be optimized with SIMD
    for( int i=0; i<bufferSize; ++i ){
        if( input[i]==0.0f ){
            output[i] = 0.0f;
        }
    }
    
}

float pdsp::LinToDB::eval(float value){
    return 20.0f*log10f(value); 
}
