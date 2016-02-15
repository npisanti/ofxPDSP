
#include "OneBarTimeMs.h"


pdsp::OneBarTimeMs::OneBarTimeMs(){
    addOutput("signal", output);
    updateOutputNodes();

    if(dynamicConstruction){
        prepareToPlay(globalBufferSize, globalSampleRate);
    }    
}


pdsp::Patchable& pdsp::OneBarTimeMs::out_signal(){
    return out("signal");
}

void pdsp::OneBarTimeMs::prepareUnit( int expectedBufferSize, double sampleRate ) {}

void pdsp::OneBarTimeMs::releaseResources () {}

void pdsp::OneBarTimeMs::process (int bufferSize) noexcept {
    
    setControlRateOutput(output, Clockable::getOneBarTimeMs() );

}

