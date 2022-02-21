
#include "LowCut.h"


pdsp::LowCut::Submodule::Submodule(){
    addModuleInput( "signal", lpA );
    addModuleInput( "freq", freq );  
    addModuleOutput( "signal",  lpB.out_hpf() );

    freq >> lpA.in_freq();
    freq >> lpB.in_freq();
    lpA.out_hpf() >> lpB;
}


void pdsp::LowCut::patch(){

    channels(1);
    addModuleInput(  "signal", *submodules[0]);
    addModuleOutput( "signal", *submodules[0]);
       
    addModuleInput("freq", freq);
    
    freq.set(30.0f);

}

void pdsp::LowCut::channels( std::size_t size ){
    
    std::size_t oldsize = submodules.size();
    
    if( size >= oldsize ){
        submodules.resize( size );
                
        for( std::size_t i=oldsize; i<submodules.size(); ++i ){
            submodules[i] = new pdsp::LowCut::Submodule();
            freq >> submodules[i]->in("freq");          
        }        
    }else{
        for( std::size_t i=size; i<oldsize; ++i ){
            delete submodules[i];
        }
        submodules.resize( size );
    }
}

pdsp::Patchable& pdsp::LowCut::ch( std::size_t index ){
    if( index >= submodules.size() ){
        channels(index+1);
    }
    return *(submodules[index]);
}

pdsp::Patchable& pdsp::LowCut::in_freq(){
    return in("freq");
}


pdsp::Patchable& pdsp::LowCut::in_signal(){
    return in("signal");
}

pdsp::Patchable& pdsp::LowCut::out_signal(){
    return out("signal");
}


// ----------------- backward compatibility -------------------------

pdsp::Patchable& pdsp::LowCut::operator[]( std::size_t index ){
    return ch( index );
}

pdsp::Patchable& pdsp::LowCut::in_0(){
    if( submodules.size()<2 ){
        channels(2);
        addModuleInput( "0", *submodules[0] );
        addModuleInput( "1", *submodules[1] );
        addModuleOutput( "0", *submodules[0] );
        addModuleOutput( "1", *submodules[1] );
    } 
    return in("0");
}
    
pdsp::Patchable& pdsp::LowCut::in_1(){
    if( submodules.size()<2 ){
        channels(2);
        addModuleInput( "0", *submodules[0] );
        addModuleInput( "1", *submodules[1] );
        addModuleOutput( "0", *submodules[0] );
        addModuleOutput( "1", *submodules[1] );
    } 
    return in("1");
}

pdsp::Patchable& pdsp::LowCut::out_0(){
    if( submodules.size()<2 ){
        channels(2);
        addModuleInput( "0", *submodules[0] );
        addModuleInput( "1", *submodules[1] );
        addModuleOutput( "0", *submodules[0] );
        addModuleOutput( "1", *submodules[1] );
    } 
    return out("0");
}
    
pdsp::Patchable& pdsp::LowCut::out_1(){
    if( submodules.size()<2 ){
        channels(2);
        addModuleInput( "0", *submodules[0] );
        addModuleInput( "1", *submodules[1] );
        addModuleOutput( "0", *submodules[0] );
        addModuleOutput( "1", *submodules[1] );
    } 
    return out("1");
}

