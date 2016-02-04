
#include "CheapPulse.h"


pdsp::CheapPulse::CheapPulse(){
    input_shape.setDefaultValue( 0.5f );
}

pdsp::Patchable& pdsp::CheapPulse::set(float pulseWidth){
    if(pulseWidth>1.0f){
        input_shape.setDefaultValue(1.0f);
    }else if(pulseWidth<0.0f){
        input_shape.setDefaultValue(0.0f);
    }else{
        input_shape.setDefaultValue(pulseWidth);
    }
    return *this;
}


void pdsp::CheapPulse::oscillateShapeCR(float* outputBuffer, const float* phaseBuffer, const float shape, int bufferSize) noexcept {
        genPulse(outputBuffer, phaseBuffer, shape, bufferSize);
};


void pdsp::CheapPulse::oscillateShapeAR(float* outputBuffer, const float* phaseBuffer, const float* shapeBuffer, int bufferSize) noexcept {
        genPulsePWM(outputBuffer, phaseBuffer, shapeBuffer, bufferSize);
};
