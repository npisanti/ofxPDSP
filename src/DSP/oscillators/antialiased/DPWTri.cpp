

#include "DPWTri.h"

pdsp::DPWTri::DPWTri(){
        z_1 = 0.0;
        addInput("inc", input_inc);
        updateOutputNodes();
};

pdsp::Patchable& pdsp::DPWTri::in_inc(){
    return in("inc");
}

void pdsp::DPWTri::prepareOscillator( double sampleRate) {
        z_1 = 0.0;
};

void pdsp::DPWTri::releaseOscillator() {

};

void pdsp::DPWTri::oscillate( float* outputBuffer, const float* phaseBuffer, int bufferSize) noexcept {
        
        int incBufferState;
        const float* incBuffer = processInput(input_inc, incBufferState);

        if (incBufferState == AudioRate){ //phase increment is always changing

                //TODO make with SIMD

                for (int n = 0; n < bufferSize; ++n){

                        //create a modulo with double frequency
                        outputBuffer[n] = phaseBuffer[n] * 2.0f;
                        if (outputBuffer[n] >= 1.0f){
                                outputBuffer[n] -= 1.0f;
                        }

                        //create saw
                        outputBuffer[n] = (outputBuffer[n] * 2.0f) - 1.0f;
                        outputBuffer[n] *= outputBuffer[n];
                        
                        //create parabol
                        outputBuffer[n] = 1.0f - outputBuffer[n];

                        //square modulo
                        if (phaseBuffer[n] < 0.5f){
                                outputBuffer[n] *= -1.0f;
                        }

                        //differentiator
                        outputBuffer[n] = outputBuffer[n] - z_1;
                        z_1 = outputBuffer[n];
                        outputBuffer[n] /= (8.0f * incBuffer[n] * (1.0f - incBuffer[n]));
                }
                
        }else{ //phase increment is always the same

                float c = 1 / (8.0f * incBuffer[0] * (1.0f - incBuffer[0]));

                for (int n = 0; n < bufferSize; ++n){

                        //create a modulo with double frequency
                        outputBuffer[n] = phaseBuffer[n]*2.0f;
                        if (outputBuffer[n] >= 1.0f){
                                outputBuffer[n] -= 1.0f;
                        }

                //create saw
                outputBuffer[n] = (outputBuffer[n] * 2.0f) - 1.0f;


                outputBuffer[n] *= outputBuffer[n];

                //create parabol
                outputBuffer[n] = 1.0f - outputBuffer[n];

                //square modulo
                if (phaseBuffer[n] < 0.5f){
                        outputBuffer[n] *= -1.0f;
                }

                //differentiator
                outputBuffer[n] = outputBuffer[n] - z_1;
                z_1 = outputBuffer[n];
                outputBuffer[n] *= c;

                }

        }

}
