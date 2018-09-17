
#include "HighCut.h"


pdsp::HighCut::Submodule::Submodule(){
    addModuleInput( "signal", lpA );
    addModuleInput( "freq", freq );  
    addModuleOutput( "signal",  lpB );

    freq >> lpA.in_freq();
    freq >> lpB.in_freq();
    lpA >> lpB;
}

void pdsp::HighCut::patch(){

    channels(1);
    addModuleInput(  "signal", *submodules[0]);
    addModuleOutput( "signal", *submodules[0]);
       
    addModuleInput("freq", freq);
    
    freq.set(10000.0f);

}

void pdsp::HighCut::channels( size_t size ){
    
    size_t oldsize = submodules.size();
    
    if( size >= oldsize ){
        submodules.resize( size );
                
        for (size_t i=oldsize; i<submodules.size(); ++i ){
            submodules[i] = new pdsp::HighCut::Submodule();
            freq >> submodules[i]->in("freq");          
        }        
    }else{
        for( size_t i=size; i<oldsize; ++i ){
            delete submodules[i];
        }
        submodules.resize( size );
    }
}

pdsp::Patchable& pdsp::HighCut::ch( size_t index ){
    if( index >= submodules.size() ){
        channels(index+1);
    }
    return *(submodules[index]);
}

pdsp::Patchable& pdsp::HighCut::in_signal(){
    return in("signal");
}

pdsp::Patchable& pdsp::HighCut::in_freq(){
    return in("freq");
}

pdsp::Patchable& pdsp::HighCut::out_signal(){
    return out("signal");
}


// ----------------- backward compatibility -------------------------

pdsp::Patchable& pdsp::HighCut::operator[]( size_t index ){
    return ch( index );
}

pdsp::Patchable& pdsp::HighCut::in_0(){
    if( submodules.size()<2 ){
        channels(2);
        addModuleInput( "0", *submodules[0] );
        addModuleInput( "1", *submodules[1] );
        addModuleOutput( "0", *submodules[0] );
        addModuleOutput( "1", *submodules[1] );
    } 
    return in("0");
}
    
pdsp::Patchable& pdsp::HighCut::in_1(){
    if( submodules.size()<2 ){
        channels(2);
        addModuleInput( "0", *submodules[0] );
        addModuleInput( "1", *submodules[1] );
        addModuleOutput( "0", *submodules[0] );
        addModuleOutput( "1", *submodules[1] );
    } 
    return in("1");
}

pdsp::Patchable& pdsp::HighCut::out_0(){
    if( submodules.size()<2 ){
        channels(2);
        addModuleInput( "0", *submodules[0] );
        addModuleInput( "1", *submodules[1] );
        addModuleOutput( "0", *submodules[0] );
        addModuleOutput( "1", *submodules[1] );
    } 
    return out("0");
}
    
pdsp::Patchable& pdsp::HighCut::out_1(){
    if( submodules.size()<2 ){
        channels(2);
        addModuleInput( "0", *submodules[0] );
        addModuleInput( "1", *submodules[1] );
        addModuleOutput( "0", *submodules[0] );
        addModuleOutput( "1", *submodules[1] );
    } 
    return out("1");
}


