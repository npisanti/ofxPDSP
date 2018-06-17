
#include "VAFilter.h"

const float pdsp::VAFilter::LowPass24 = 0.0f;
const float pdsp::VAFilter::LowPass12 = 1.0f;
const float pdsp::VAFilter::HighPass24 = 2.0f;
const float pdsp::VAFilter::HighPass12 = 3.0f;
const float pdsp::VAFilter::BandPass24 = 4.0f;
const float pdsp::VAFilter::BandPass12 = 5.0f;
            

void pdsp::VAFilter::patch(){
    channels(1);
    addModuleInput(  "signal", *submodules[0]);
    addModuleOutput( "signal", *submodules[0]);    
    
    addModuleInput("pitch", p2f);
    addModuleInput("reso", reso);
    addModuleInput("mode", mode);    
    p2f.set(80.0f);
    mode.set(0.0f);
}

pdsp::VAFilter::~VAFilter(){
    channels(0);
}

pdsp::VAFilter::Submodule::Submodule(){
    addModuleInput( "signal", filter );
    addModuleInput( "freq", filter.in_freq() );
    addModuleInput( "reso", filter.in_reso() );
    addModuleInput( "select", fswitch.in_select() );
    addModuleOutput( "signal",  fswitch );
    fswitch.resize(6);
    filter.out_lpf4() >> fswitch.input(0);
    filter.out_lpf2() >> fswitch.input(1);
    filter.out_hpf4() >> fswitch.input(2);
    filter.out_hpf2() >> fswitch.input(3);
    filter.out_bpf4() >> fswitch.input(4);
    filter.out_bpf2() >> fswitch.input(5);
}

void pdsp::VAFilter::channels( int size ){
    
    int oldsize = submodules.size();
    
    if( size >= oldsize ){
        submodules.resize( size );
                
        for (size_t i=oldsize; i<submodules.size(); ++i ){
            submodules[i] = new pdsp::VAFilter::Submodule();
            p2f >> submodules[i]->in("freq");
            reso >> submodules[i]->in("reso");
            mode >> submodules[i]->in("select");            
        }        
    }else{
        for (int i=size; i<oldsize; ++i ){
            delete submodules[i];
        }
        submodules.resize( size );
    }
}

pdsp::Patchable& pdsp::VAFilter::operator[]( const int & ch ){
    if( ch >= int(submodules.size()) ){
        channels(ch+1);
    }
    return *(submodules[ch]);
}



pdsp::Patchable& pdsp::VAFilter::in_0(){
    if( submodules.size()<2 ){
        channels(2);
        addModuleInput( "0", *submodules[0] );
        addModuleInput( "1", *submodules[1] );
        addModuleOutput( "0", *submodules[0] );
        addModuleOutput( "1", *submodules[1] );
    } 
    return in("0");
}
    
pdsp::Patchable& pdsp::VAFilter::in_1(){
    if( submodules.size()<2 ){
        channels(2);
        addModuleInput( "0", *submodules[0] );
        addModuleInput( "1", *submodules[1] );
        addModuleOutput( "0", *submodules[0] );
        addModuleOutput( "1", *submodules[1] );
    } 
    return in("1");
}

pdsp::Patchable& pdsp::VAFilter::in_L(){
    if( submodules.size()<2 ){
        channels(2);
        addModuleInput( "0", *submodules[0] );
        addModuleInput( "1", *submodules[1] );
        addModuleOutput( "0", *submodules[0] );
        addModuleOutput( "1", *submodules[1] );
    } 
    return in("0");
}
    
pdsp::Patchable& pdsp::VAFilter::in_R(){
    if( submodules.size()<2 ){
        channels(2);
        addModuleInput( "0", *submodules[0] );
        addModuleInput( "1", *submodules[1] );
        addModuleOutput( "0", *submodules[0] );
        addModuleOutput( "1", *submodules[1] );
    } 
    return in("1");
}

pdsp::Patchable& pdsp::VAFilter::out_0(){
    if( submodules.size()<2 ){
        channels(2);
        addModuleInput( "0", *submodules[0] );
        addModuleInput( "1", *submodules[1] );
        addModuleOutput( "0", *submodules[0] );
        addModuleOutput( "1", *submodules[1] );
    } 
    return out("0");
}
    
pdsp::Patchable& pdsp::VAFilter::out_1(){
    if( submodules.size()<2 ){
        channels(2);
        addModuleInput( "0", *submodules[0] );
        addModuleInput( "1", *submodules[1] );
        addModuleOutput( "0", *submodules[0] );
        addModuleOutput( "1", *submodules[1] );
    } 
    return out("1");
}

pdsp::Patchable& pdsp::VAFilter::out_L(){
    if( submodules.size()<2 ){
        channels(2);
        addModuleInput( "0", *submodules[0] );
        addModuleInput( "1", *submodules[1] );
        addModuleOutput( "0", *submodules[0] );
        addModuleOutput( "1", *submodules[1] );
    } 
    return out("0");
}
    
pdsp::Patchable& pdsp::VAFilter::out_R(){
    if( submodules.size()<2 ){
        channels(2);
        addModuleInput( "0", *submodules[0] );
        addModuleInput( "1", *submodules[1] );
        addModuleOutput( "0", *submodules[0] );
        addModuleOutput( "1", *submodules[1] );
    } 
    return out("1");
}

pdsp::Patchable& pdsp::VAFilter::in_signal(){
    return in("signal");
}

pdsp::Patchable& pdsp::VAFilter::out_signal(){
    return out("signal");
}

pdsp::Patchable& pdsp::VAFilter::in_pitch(){
    return in("pitch");
}

pdsp::Patchable& pdsp::VAFilter::in_cutoff(){
    return in("pitch");
}

pdsp::Patchable& pdsp::VAFilter::in_reso(){
    return in("reso");
}

pdsp::Patchable& pdsp::VAFilter::in_mode(){
    return in("mode");
}

float pdsp::VAFilter::meter_cutoff() const {
    return p2f.meter_input();
}      
    
    
