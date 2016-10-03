
#include "IRVerb.h"

pdsp::IRVerb::IRVerb() { patch(); }
pdsp::IRVerb::IRVerb(const IRVerb& other){ patch(); }
pdsp::IRVerb& pdsp::IRVerb::operator=(const IRVerb& other){ return *this; }


void pdsp::IRVerb::patch(){
    
    stereoConnected = false;
    monoConnected = false;
    
    addModuleInput( "mono",  input_mono );
    addModuleInput( "L",  input_L );
    addModuleInput( "R",  input_R );

    addModuleOutput("L", reverbL);
    addModuleOutput("R", reverbR);

}

pdsp::Patchable& pdsp::IRVerb::in_mono(){
    checkMono();
    return in("mono");
}


pdsp::Patchable& pdsp::IRVerb::in_0(){
    checkStereo();
    return in("L");
}

pdsp::Patchable& pdsp::IRVerb::in_1(){
    checkStereo();
    return in("R");
}

pdsp::Patchable& pdsp::IRVerb::in_L(){
    checkStereo();
    return in("L");
}

pdsp::Patchable& pdsp::IRVerb::in_R(){
    checkStereo();
    return in("R");
}


pdsp::Patchable& pdsp::IRVerb::out_0(){
    return out("L");
}

pdsp::Patchable& pdsp::IRVerb::out_1(){
    return out("R");
}

pdsp::Patchable& pdsp::IRVerb::out_L(){
    return out("L");
}

pdsp::Patchable& pdsp::IRVerb::out_R(){
    return out("R");
}


void pdsp::IRVerb::checkMono() {
    if(!monoConnected){
        input_mono >> reverbL;
        input_mono >> reverbR;
        monoConnected = true;
    }    
}

void pdsp::IRVerb::checkStereo() {
    if(!stereoConnected){
        input_L >> reverbL;
        input_R >> reverbR;
        stereoConnected = true;
    }       
}

void pdsp::IRVerb::loadIR ( std::string path ) {
    
    impulse.load( path );
    
    if (impulse.channels == 1 ){
        reverbL.loadIR( impulse );
        reverbR.loadIR( impulse );
    }else if (impulse.channels>1){
        reverbL.loadIR( impulse, 0 );
        reverbR.loadIR( impulse, 1 );        
    }
    
}

void pdsp::IRVerb::prepareToPlay(int expectedBufferSize, double sampleRate){
    // if we have not used any in_ mono activate the default connection (mono)
    if(!monoConnected && !stereoConnected) checkMono();
}
    
void pdsp::IRVerb::releaseResources() { }

