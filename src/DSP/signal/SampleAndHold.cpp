

#include "SampleAndHold.h"



pdsp::SampleAndHold::SampleAndHold(){
        addInput("signal", input_signal);
        addInput("trig", input_trig);
        addOutput("signal", output);
        updateOutputNodes();
        
        sampled = 0.0f;
        meter.store(sampled);
}

float pdsp::SampleAndHold::meter_output() const{
    return meter.load();
}

pdsp::Patchable& pdsp::SampleAndHold::in_trig(){
    return in("trig");
}

pdsp::Patchable& pdsp::SampleAndHold::in_signal(){
    return in("signal");
}

pdsp::Patchable& pdsp::SampleAndHold::out_signal(){
    return out("signal");
}

void pdsp::SampleAndHold::prepareUnit( int expectedBufferSize, double sampleRate ){   };

void pdsp::SampleAndHold::releaseResources () {   };

void pdsp::SampleAndHold::process (int bufferSize) noexcept  {

        int trigState;
        const float* trigBuffer = processInput(input_trig, trigState);

        switch (trigState) {
        case Unchanged: case Changed:
                setControlRateOutput(output, sampled);
                meter.store(sampled);
                break;
        case AudioRate:
                int signalState;
                const float* signalBuffer = processInput(input_signal, signalState);
                float* outputBuffer = getOutputBufferToFill( output );
                
                switch (signalState) {
                case AudioRate:
                        for(int n=0; n<bufferSize; ++n){
                                if(checkTrigger(trigBuffer[n])){
                                        sampled = signalBuffer[n];
                                }
                                outputBuffer[n] = sampled;
                        }
                        break;
                default:
                        int n;
                        for(n=0; n<bufferSize; ++n){
                                if(checkTrigger(trigBuffer[n])){
                                        sampled = signalBuffer[0];
                                        break;
                                }
                                outputBuffer[n] = sampled;
                        }
                        ofx_Aeq_S_range(outputBuffer, sampled, n+1, bufferSize);
                        break;
                }
                
                meter.store(outputBuffer[0]);

                break;
        }

}
