
#include "TriggeredRandom.h"


pdsp::TriggeredRandom::TriggeredRandom(){

        sampled = randomBipolar(); 

        addInput("trig", input_trig);
        addOutput("signal", output);
        updateOutputNodes();
        
        if(dynamicConstruction){
                prepareToPlay(globalBufferSize, globalSampleRate);
        }
}

pdsp::Patchable& pdsp::TriggeredRandom::in_trig(){
    return in("trig");
}

pdsp::Patchable& pdsp::TriggeredRandom::out_signal(){
    return out("signal");
}

float pdsp::TriggeredRandom::meter_output() const{
    return meter.load();
}

void pdsp::TriggeredRandom::prepareUnit( int expectedBufferSize, double sampleRate){ }

void pdsp::TriggeredRandom::releaseResources () {}

void pdsp::TriggeredRandom::process (int bufferSize) noexcept  {
    
        int trigState;
        const float* trigBuffer = processInput(input_trig, trigState);

        
        
        switch (trigState) {
        case AudioRate: 
            {
                float* outputBuffer = getOutputBufferToFill(output);
                for(int n=0; n<bufferSize; ++n){
                        if(checkTrigger(trigBuffer[n])){
                                sampled = randomBipolar(); 
                                meter.store(sampled);
                                //std::cout<<"jitter: "<<sampled<<"\n";
                        }
                        outputBuffer[n] = sampled;
                }
    
            }
                break;
                
        default:
                setControlRateOutput(output, sampled);
                //Aeq_S(outputBuffer, sampled, bufferSize); 
                // the noise generator has to run always at audio rate, otherwise some unit won't work
                break;
        }
        
        
}
