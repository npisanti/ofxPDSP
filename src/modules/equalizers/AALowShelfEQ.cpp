
#include "AALowShelfEQ.h"

pdsp::AALowShelfEQ::Submodule::Submodule(){
    addModuleInput( "signal", upsampler );
    addModuleInput( "freq", eq.in_freq() );
    addModuleInput( "Q", eq.in_Q() );
    addModuleInput("gain", eq.in_gain() );  
    addModuleOutput( "signal",  downsampler );
    eq.setOversampleLevel(2);
    upsampler >> eq >> downsampler;
}


void pdsp::AALowShelfEQ::patch(){
    channels(1);
    addModuleInput(  "signal", *submodules[0]);
    addModuleOutput( "signal", *submodules[0]);
       
    addModuleInput("freq", freq);
    addModuleInput("Q", Q);
    addModuleInput("gain", gain );  
    
    freq.set(10000.0f);
    Q.set(0.707f);
    gain.set(0.0f);   
}



void pdsp::AALowShelfEQ::channels( std::size_t size ){
    
    std::size_t oldsize = submodules.size();
    
    if( size >= oldsize ){
        submodules.resize( size );
                
        for( std::size_t i=oldsize; i<submodules.size(); ++i ){
            submodules[i] = new pdsp::AALowShelfEQ::Submodule();
            freq >> submodules[i]->in("freq");
            Q >> submodules[i]->in("Q");
            gain >> submodules[i]->in("gain");            
        }        
    }else{
        for( std::size_t i=size; i<oldsize; ++i ){
            delete submodules[i];
        }
        submodules.resize( size );
    }
}

pdsp::Patchable& pdsp::AALowShelfEQ::ch( std::size_t index ){
    if( index >= submodules.size() ){
        channels(index+1);
    }
    return *(submodules[index]);
}

pdsp::Patchable& pdsp::AALowShelfEQ::in_signal(){
    return in("signal");
}

pdsp::Patchable& pdsp::AALowShelfEQ::out_signal(){
    return out("signal");
}

pdsp::Patchable& pdsp::AALowShelfEQ::in_freq(){
    return in("freq");
}

pdsp::Patchable& pdsp::AALowShelfEQ::in_Q(){
    return in("Q");
}

pdsp::Patchable& pdsp::AALowShelfEQ::in_gain(){
    return in("gain");
}



// ----------------- backward compatibility -------------------------

pdsp::Patchable& pdsp::AALowShelfEQ::operator[]( std::size_t index ){
    return ch( index );
}

pdsp::Patchable& pdsp::AALowShelfEQ::in_0(){
    if( submodules.size()<2 ){
        channels(2);
        addModuleInput( "0", *submodules[0] );
        addModuleInput( "1", *submodules[1] );
        addModuleOutput( "0", *submodules[0] );
        addModuleOutput( "1", *submodules[1] );
    } 
    return in("0");
}
    
pdsp::Patchable& pdsp::AALowShelfEQ::in_1(){
    if( submodules.size()<2 ){
        channels(2);
        addModuleInput( "0", *submodules[0] );
        addModuleInput( "1", *submodules[1] );
        addModuleOutput( "0", *submodules[0] );
        addModuleOutput( "1", *submodules[1] );
    } 
    return in("1");
}

pdsp::Patchable& pdsp::AALowShelfEQ::out_0(){
    if( submodules.size()<2 ){
        channels(2);
        addModuleInput( "0", *submodules[0] );
        addModuleInput( "1", *submodules[1] );
        addModuleOutput( "0", *submodules[0] );
        addModuleOutput( "1", *submodules[1] );
    } 
    return out("0");
}
    
pdsp::Patchable& pdsp::AALowShelfEQ::out_1(){
    if( submodules.size()<2 ){
        channels(2);
        addModuleInput( "0", *submodules[0] );
        addModuleInput( "1", *submodules[1] );
        addModuleOutput( "0", *submodules[0] );
        addModuleOutput( "1", *submodules[1] );
    } 
    return out("1");
}

