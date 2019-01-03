
#include "OnTrigger.h"

pdsp::OnTrigger::OnTrigger(){
    addInput("trig", input_trig);
    addOutput("signal", output);
    updateOutputNodes();
    
    sampled = 0.0f;
    meter.store(sampled);
    
    code = []( float trig ) noexcept { return trig; };
}

float pdsp::OnTrigger::meter_output() const{
    return meter.load();
}

pdsp::Patchable& pdsp::OnTrigger::in_trig(){
    return in("trig");
}

pdsp::Patchable& pdsp::OnTrigger::out_signal(){
    return out("signal");
}

void pdsp::OnTrigger::prepareUnit( int expectedBufferSize, double sampleRate ){   };

void pdsp::OnTrigger::releaseResources () {   };

void pdsp::OnTrigger::process (int bufferSize) noexcept  {

    int trigState;
    const float* trigBuffer = processInput(input_trig, trigState);

    switch (trigState) {
    case Unchanged: 
    case Changed: 
        setControlRateOutput(output, sampled);
    break;
            
    case AudioRate:
            float* outputBuffer = getOutputBufferToFill( output );
            for(int n=0; n<bufferSize; ++n){
                if(checkTrigger(trigBuffer[n])){
                    sampled = code( trigBuffer[n] );
                }
                outputBuffer[n] = sampled;
            }
            meter.store(outputBuffer[bufferSize-1]);
    break;
    }
}
