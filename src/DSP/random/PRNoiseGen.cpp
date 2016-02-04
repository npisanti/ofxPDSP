

#include "PRNoiseGen.h"



pdsp::PRNoiseGen::PRNoiseGen(){

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

pdsp::Patchable& pdsp::PRNoiseGen::in_clock(){
    return in("clock");
}

pdsp::Patchable& pdsp::PRNoiseGen::in_reseed(){
    return in("reseed");
}

void pdsp::PRNoiseGen::prepareUnit( int expectedBufferSize, double sampleRate) { }


void pdsp::PRNoiseGen::releaseResources() { }


inline_f void pdsp::PRNoiseGen::reSeed(){
        //seed = seedMult++ * time(NULL);
        seed = randomInt();
}



void pdsp::PRNoiseGen::process(int bufferSize) noexcept  {

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

void pdsp::PRNoiseGen::process_run(const int &bufferSize) noexcept {
        
        float* outputBuffer = getOutputBufferToFill(output);
        
        for (int n = 0; n < bufferSize; ++n){
                outputBuffer[n] = PNSequenceNext(pnRegister);
        }
        
}

void pdsp::PRNoiseGen::process_PA(const float* &trigSyncBuffer, const int &bufferSize) noexcept {
        
        float* outputBuffer = getOutputBufferToFill(output);
        
        for (int n = 0; n < bufferSize; ++n){
                //resetting on cycle
                if (checkSync(trigSyncBuffer[n])){
                        pnRegister = seed;
                }
                outputBuffer[n] = PNSequenceNext(pnRegister);
        }

}

void pdsp::PRNoiseGen::process_SA_TA(const float* &trigSyncBuffer, const float* &trigSeedBuffer, const int &bufferSize) noexcept {
        
        float* outputBuffer = getOutputBufferToFill(output);

        for (int n = 0; n < bufferSize; ++n){
                //new seed on gate
                if (checkTrigger(trigSeedBuffer[n])){
                        reSeed();
                }
                //resetting on cycle
                if (checkSync(trigSyncBuffer[n])){
                        pnRegister = seed;
                }
                outputBuffer[n] = PNSequenceNext(pnRegister);
        }
}

