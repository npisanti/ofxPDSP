
#include "StereoConvolver.h"

pdsp::StereoConvolver::StereoConvolver() { patch(); }
pdsp::StereoConvolver::StereoConvolver(const StereoConvolver& other){ patch(); }
pdsp::StereoConvolver& pdsp::StereoConvolver::operator=(const StereoConvolver& other){ return *this; }

//pdsp::StereoConvolver::StereoConvolver(StereoConvolver&& other){ patch(); }
//pdsp::StereoConvolver::StereoConvolver& operator=( StereoConvolver&& other){ return *this; }

void pdsp::StereoConvolver::patch(){
    
    addModuleInput( "0",  channel1 );
    addModuleInput( "1",  channel2 );    
    
    addModuleOutput( "0",  channel1 );
    addModuleOutput( "1",  channel2 );    
    
}

void pdsp::StereoConvolver::loadIR ( SampleBuffer* impulseResponse){
    if(impulseResponse->channels > 1){
        channel1.loadIR(impulseResponse);
        channel2.loadIR(impulseResponse);
    }else{
        channel1.loadIR(impulseResponse, 0);
        channel2.loadIR(impulseResponse, 1);        
    }
}

pdsp::Patchable& pdsp::StereoConvolver::in_0(){
    return in("0");
}

pdsp::Patchable& pdsp::StereoConvolver::in_1(){
    return in("1");
}


pdsp::Patchable& pdsp::StereoConvolver::out_0(){
    return out("0");
}

pdsp::Patchable& pdsp::StereoConvolver::out_1(){
    return out("1");
}

