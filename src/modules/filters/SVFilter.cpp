
#include "SVFilter.h"

const float pdsp::SVFilter::LowPass = 0.0f;
const float pdsp::SVFilter::BandPass = 1.0f;
const float pdsp::SVFilter::HighPass = 2.0f;
const float pdsp::SVFilter::Notch = 3.0f;
            

void pdsp::SVFilter::patch(){
    channels(1);
    addModuleInput(  "signal", *submodules[0]);
    addModuleOutput( "signal", *submodules[0]);
    
    addModuleInput("pitch", p2f);
    addModuleInput("reso", reso);
    addModuleInput("mode", mode);    
    p2f.set(80.0f);
    mode.set(0.0f);
}

pdsp::SVFilter::~SVFilter(){
    channels(0);
}

pdsp::SVFilter::Submodule::Submodule(){
    addModuleInput( "signal", filter );
    addModuleInput( "freq", filter.in_freq() );
    addModuleInput( "reso", filter.in_reso() );
    addModuleInput( "select", fswitch.in_select() );
    addModuleOutput( "signal",  fswitch );
    fswitch.resize(4);
    filter.out_lpf() >> fswitch.input(0);
    filter.out_bpf() >> fswitch.input(1);
    filter.out_hpf() >> fswitch.input(2);
    filter.out_notch() >> fswitch.input(3);
}

void pdsp::SVFilter::channels( size_t size ){
    
    size_t oldsize = submodules.size();
    
    if( size >= oldsize ){
        submodules.resize( size );
                
        for (size_t i=oldsize; i<submodules.size(); ++i ){
            submodules[i] = new pdsp::SVFilter::Submodule();
            p2f >> submodules[i]->in("freq");
            reso >> submodules[i]->in("reso");
            mode >> submodules[i]->in("select");            
        }        
    }else{
        for( size_t i=size; i<oldsize; ++i ){
            delete submodules[i];
        }
        submodules.resize( size );
    }
}

pdsp::Patchable& pdsp::SVFilter::ch( size_t index ){
    if( index >= submodules.size() ){
        channels(index+1);
    }
    return *(submodules[index]);
}


pdsp::Patchable& pdsp::SVFilter::in_0(){
    if( submodules.size()<2 ){
        channels(2);
        addModuleInput( "0", *submodules[0] );
        addModuleInput( "1", *submodules[1] );
        addModuleOutput( "0", *submodules[0] );
        addModuleOutput( "1", *submodules[1] );
    } 
    return in("0");
}
    
pdsp::Patchable& pdsp::SVFilter::in_1(){
    if( submodules.size()<2 ){
        channels(2);
        addModuleInput( "0", *submodules[0] );
        addModuleInput( "1", *submodules[1] );
        addModuleOutput( "0", *submodules[0] );
        addModuleOutput( "1", *submodules[1] );
    } 
    return in("1");
}

pdsp::Patchable& pdsp::SVFilter::in_L(){
    if( submodules.size()<2 ){
        channels(2);
        addModuleInput( "0", *submodules[0] );
        addModuleInput( "1", *submodules[1] );
        addModuleOutput( "0", *submodules[0] );
        addModuleOutput( "1", *submodules[1] );
    } 
    return in("0");
}
    
pdsp::Patchable& pdsp::SVFilter::in_R(){
    if( submodules.size()<2 ){
        channels(2);
        addModuleInput( "0", *submodules[0] );
        addModuleInput( "1", *submodules[1] );
        addModuleOutput( "0", *submodules[0] );
        addModuleOutput( "1", *submodules[1] );
    } 
    return in("1");
}

pdsp::Patchable& pdsp::SVFilter::out_0(){
    if( submodules.size()<2 ){
        channels(2);
        addModuleInput( "0", *submodules[0] );
        addModuleInput( "1", *submodules[1] );
        addModuleOutput( "0", *submodules[0] );
        addModuleOutput( "1", *submodules[1] );
    } 
    return out("0");
}
    
pdsp::Patchable& pdsp::SVFilter::out_1(){
    if( submodules.size()<2 ){
        channels(2);
        addModuleInput( "0", *submodules[0] );
        addModuleInput( "1", *submodules[1] );
        addModuleOutput( "0", *submodules[0] );
        addModuleOutput( "1", *submodules[1] );
    } 
    return out("1");
}

pdsp::Patchable& pdsp::SVFilter::out_L(){
    if( submodules.size()<2 ){
        channels(2);
        addModuleInput( "0", *submodules[0] );
        addModuleInput( "1", *submodules[1] );
        addModuleOutput( "0", *submodules[0] );
        addModuleOutput( "1", *submodules[1] );
    } 
    return out("0");
}
    
pdsp::Patchable& pdsp::SVFilter::out_R(){
    if( submodules.size()<2 ){
        channels(2);
        addModuleInput( "0", *submodules[0] );
        addModuleInput( "1", *submodules[1] );
        addModuleOutput( "0", *submodules[0] );
        addModuleOutput( "1", *submodules[1] );
    } 
    return out("1");
}

pdsp::Patchable& pdsp::SVFilter::in_signal(){
    return in("signal");
}

pdsp::Patchable& pdsp::SVFilter::out_signal(){
    return out("signal");
}

pdsp::Patchable& pdsp::SVFilter::in_pitch(){
    return in("pitch");
}

pdsp::Patchable& pdsp::SVFilter::in_cutoff(){
    return in("pitch");
}

pdsp::Patchable& pdsp::SVFilter::in_reso(){
    return in("reso");
}

pdsp::Patchable& pdsp::SVFilter::in_mode(){
    return in("mode");
}

float pdsp::SVFilter::meter_cutoff() const {
    return p2f.meter_input();
}      
    
pdsp::Patchable& pdsp::SVFilter::operator[]( size_t index ){
    return ch( index );
}
    
