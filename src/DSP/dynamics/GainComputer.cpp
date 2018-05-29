
#include "GainComputer.h"


pdsp::GainComputer::GainComputer(){
    addInput("signal", input);
    addOutput("signal", output);
    addInput("threshold", in_thresh);
    addInput("ratio", input_ratio);
    addInput("knee", input_knee);
    updateOutputNodes();
    
    in_thresh.setDefaultValue(-20.0f);
    
    input_ratio.setDefaultValue(4.0f);
    CS = 1.0f-(1.0f/4.0f);
    
    input_knee.setDefaultValue(0.0f);
    
    if(dynamicConstruction){
        prepareToPlay(globalBufferSize, globalSampleRate);
    }    
}

pdsp::Patchable& pdsp::GainComputer::set(float threshold, float ratio, float knee){
        in_thresh.setDefaultValue(threshold); 
        input_ratio.setDefaultValue(ratio);
        input_knee.setDefaultValue(knee);
        return *this;
}

float pdsp::GainComputer::meter_reduction() const {
    return meter.load();
}

pdsp::Patchable& pdsp::GainComputer::in_signal(){
    return in("signal");
}

pdsp::Patchable& pdsp::GainComputer::in_threshold(){
    return in("threshold");
}

pdsp::Patchable& pdsp::GainComputer::in_ratio(){
    return in("ratio");
}

pdsp::Patchable& pdsp::GainComputer::in_knee(){
    return in("knee");
}

pdsp::Patchable& pdsp::GainComputer::out_signal(){
    return in("signal");
}

void pdsp::GainComputer::prepareUnit( int expectedBufferSize, double sampleRate ) {}

void pdsp::GainComputer::releaseResources () {}


void pdsp::GainComputer::process (int bufferSize) noexcept {
  
        int inputState;
        const float* inputBuffer = processInput(input, inputState);

        if(inputState==AudioRate){
                
                float threshold = processAndGetSingleValue(in_thresh, 0);
                float knee = processAndGetSingleValue(input_knee, 0);
                
                int ratioChanged;
                const float * ratioBuffer = processInput(input_ratio, ratioChanged);
                
                if(ratioChanged){
                    if(ratioBuffer[0] > 40.0f){
                        CS = 1.0f;
                    }else{
                        CS = 1.0f-(1.0f/ratioBuffer[0]);
                    }
                }

                float* outputBuffer = getOutputBufferToFill(output);
                
                if(knee>0.0f){
                        vect_calculateGainReductionDBWithKnee(outputBuffer, inputBuffer, threshold, CS, knee, bufferSize); 
                }else{
                        vect_calculateGainReductionDB(outputBuffer, inputBuffer, threshold, CS, bufferSize); 
                }
                
                
                float min = 0.0f; 
                for(int n=0; n<bufferSize; ++n){
                    min = (min<outputBuffer[n]) ? min : outputBuffer[n];
                }
                meter.store( min );
                
        }else{
            setOutputToZero(output);
            meter.store(0.0f);
        }  
}
