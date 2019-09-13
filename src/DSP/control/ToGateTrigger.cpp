
#include "ToGateTrigger.h"


pdsp::ToGateTrigger::ToGateTrigger(){
    addInput("signal", input);
    addInput("threshold", in_thresh);
    addOutput("trig", output);
    updateOutputNodes();
    
    gate = 0;
    in_thresh.setDefaultValue(0.5f);
    set(0.5f);
    
    if(dynamicConstruction){
        prepareToPlay(globalBufferSize, globalSampleRate);
    }    
}

pdsp::Patchable& pdsp::ToGateTrigger::set(float threshold, float hysteresis){
    this->hysteresis = hysteresis;
    updateThresholds(threshold);
    return *this;
}

pdsp::Patchable& pdsp::ToGateTrigger::in_signal(){
    return in("signal");
}

pdsp::Patchable& pdsp::ToGateTrigger::in_threshold(){
    return in("threshold");
}

pdsp::Patchable& pdsp::ToGateTrigger::out_trig(){
    return out("trig");
}

float pdsp::ToGateTrigger::meter_gate() const{
    return static_cast<float> (gateMeter.load());
}
    
    
void pdsp::ToGateTrigger::setHystersis(float hysteresis){
    this->hysteresis = hysteresis;
    updateThresholds(highThreshold);
}

void pdsp::ToGateTrigger::updateThresholds(float threshold){
    if(threshold>0.0f){
        highThreshold = threshold;
        lowThreshold = threshold - hysteresis;
        if(lowThreshold<0.0f){ lowThreshold = 0.0f; }
        
        thresholdMultiply = 1.0f / (1.0f - highThreshold);
    }
}

void pdsp::ToGateTrigger::prepareUnit( int expectedBufferSize, double sampleRate ){
    gate = 0;
}
void pdsp::ToGateTrigger::releaseResources () {}



void pdsp::ToGateTrigger::process (int bufferSize) noexcept {
    
    int thresholdChanged;
    const float* thresholdBuffer = processInput(in_thresh, thresholdChanged);
    
    if(thresholdChanged == 0){ // likely branch optimization
    }else{
        updateThresholds(thresholdBuffer[0]);
    }
    
    int inputBufferState;
    const float* inputBuffer = processInput(input, inputBufferState);
    
    switch(inputBufferState){
        case Unchanged:  // NO CHANGE---------------------------------------------
            setOutputToZero(output);
        break;
        case Changed:  // CONTROL RATE---------------------------------------------
            switch(gate){
                case 0:
                    if(inputBuffer[0] > highThreshold){
                        float* outputBuffer = getOutputBufferToFill(output);
                        ofx_Aeq_Zero(outputBuffer, bufferSize);
                        this->gate = 1;
                        outputBuffer[0] = (inputBuffer[0] - highThreshold) * thresholdMultiply;
                    }
                break;
                
                case 1:
                    if(inputBuffer[0] <= lowThreshold){
                        float* outputBuffer = getOutputBufferToFill(output);
                        ofx_Aeq_Zero(outputBuffer, bufferSize);
                        this->gate = 0;
                        outputBuffer[0] = PDSP_TRIGGER_OFF;
                    }
                break;
                
                default: break;
            }
            
        break;
        case AudioRate: // AUDIO RATE---------------------------------------------
            {
            float* outputBuffer = nullptr;
 
            for (int n=0; n<bufferSize; ++n){
                
                switch(gate){
                    case 0:
                        if(inputBuffer[n] <= highThreshold){ // likely branch optimization

                        }else{
                            if(outputBuffer == nullptr){
                                outputBuffer = getOutputBufferToFill(output);
                                ofx_Aeq_Zero(outputBuffer, bufferSize);
                            }
                            gate = 1;
                            outputBuffer[n] = (inputBuffer[0] - highThreshold) * thresholdMultiply;
                            
                        }
                    break;
                    
                    case 1:
                        if(inputBuffer[n] > lowThreshold){ // likely branch optimization

                        }else{
                            if(outputBuffer == nullptr){
                                outputBuffer = getOutputBufferToFill(output);
                                ofx_Aeq_Zero(outputBuffer, bufferSize);
                            }
                            gate = 0;
                            outputBuffer[n] = PDSP_TRIGGER_OFF;
                        }
                    break;
                    
                    default: break;
                }
                        
            }
            
            if(outputBuffer == nullptr){ setOutputToZero(output); }
            
        
            }
        break;
        
        default: break;
    }
    gateMeter.store(gate);
}
