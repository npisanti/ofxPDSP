
#include "IRVerb.h"

pdsp::IRVerb::IRVerb() { patch(); }
pdsp::IRVerb::IRVerb(const IRVerb& other){ patch(); }
pdsp::IRVerb& pdsp::IRVerb::operator=(const IRVerb& other){ return *this; }


void pdsp::IRVerb::patch(){
    
    addModuleInput( "L",  reverbL );
    addModuleInput( "R",  reverbR );

    addModuleOutput("L", reverbL);
    addModuleOutput("R", reverbR);

}


pdsp::Patchable& pdsp::IRVerb::ch( std::size_t index ){
    wrapChannelIndex( index, 2, "pdsp::IRVerb" );
    
    switch( index ){
        case 0: return reverbL; break;
        case 1: return reverbR; break;
    }
    
    return reverbL;
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

// ---------------------- legacy ------------------------------------

pdsp::Patchable& pdsp::IRVerb::in_mono(){
    return in("L");
}

pdsp::Patchable& pdsp::IRVerb::in_0(){
    return in("L");
}

pdsp::Patchable& pdsp::IRVerb::in_1(){
    return in("R");
}

pdsp::Patchable& pdsp::IRVerb::in_L(){
    return in("L");
}

pdsp::Patchable& pdsp::IRVerb::in_R(){
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


