

#include "WhiteNoise.h"



pdsp::WhiteNoise::WhiteNoise(){

    addInput("clock", input_trig_clock);
    addInput("reseed", input_trig_seed);
    addOutput("signal", output);
    updateOutputNodes();

    seedMult = rand();
    //seed = seedMult * time(NULL);
    seed = randomInt();
    pnRegister = seed;

    if(dynamicConstruction){
            prepareToPlay(globalBufferSize, globalSampleRate);
    }
}

pdsp::Patchable& pdsp::WhiteNoise::in_clock(){
    return in("clock");
}

pdsp::Patchable& pdsp::WhiteNoise::in_reseed(){
    return in("reseed");
}

void pdsp::WhiteNoise::prepareUnit( int expectedBufferSize, double sampleRate) { }


void pdsp::WhiteNoise::releaseResources() { }


inline_f void pdsp::WhiteNoise::reSeed(){
        //seed = seedMult++ * time(NULL);
        seed = randomInt();
}



void pdsp::WhiteNoise::process(int bufferSize) noexcept  {

        int trigSeedBufferState;
        const float* trigSeedBuffer = processInput(input_trig_seed, trigSeedBufferState);

        int trigSyncBufferState;
        const float* trigSyncBuffer = processInput(input_trig_clock, trigSyncBufferState);

        //generate noise
        switch(trigSyncBufferState){
            case Unchanged: case Changed:
                process_run(bufferSize); //just noise
            break;
            case AudioRate:
                switch (trigSeedBufferState){
                    case Unchanged: case Changed:
                         process_PA(trigSyncBuffer, bufferSize);
                    break;
                        
                    case AudioRate:                    
                        process_SA_TA(trigSyncBuffer, trigSeedBuffer, bufferSize);
                    break;
                    
                    default: break;                       
                }
            break;
            
            default: break;
        }
        
}

void pdsp::WhiteNoise::process_run(const int &bufferSize) noexcept {
        
        float* outputBuffer = getOutputBufferToFill(output);
        
        for (int n = 0; n < bufferSize; ++n){
                outputBuffer[n] = PNSequenceNext(pnRegister);
        }
        
}

void pdsp::WhiteNoise::process_PA(const float* &trigSyncBuffer, const int &bufferSize) noexcept {
        
        float* outputBuffer = getOutputBufferToFill(output);
        
        for (int n = 0; n < bufferSize; ++n){
                //resetting on cycle
                if (checkTrigger(trigSyncBuffer[n])){
                        pnRegister = seed;
                }
                outputBuffer[n] = PNSequenceNext(pnRegister);
        }

}

void pdsp::WhiteNoise::process_SA_TA(const float* &trigSyncBuffer, const float* &trigSeedBuffer, const int &bufferSize) noexcept {
        
        float* outputBuffer = getOutputBufferToFill(output);

        for (int n = 0; n < bufferSize; ++n){
                //new seed on gate
                if (checkTrigger(trigSeedBuffer[n])){
                        reSeed();
                }
                //resetting on cycle
                if (checkTrigger(trigSyncBuffer[n])){
                        pnRegister = seed;
                }
                outputBuffer[n] = PNSequenceNext(pnRegister);
        }
}

