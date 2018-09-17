
#include "LowShelfEQ.h"

void pdsp::LowShelfEQ::patch(){
    channels(1);
    addModuleInput(  "signal", *eqs[0]);
    addModuleOutput( "signal", *eqs[0]);
       
    addModuleInput("freq", freq);
    addModuleInput("Q", Q);
    addModuleInput("gain", gain);
    
    freq.set(10000.0f);
    Q.set(0.707f);
    gain.set(0.0f);

}

void pdsp::LowShelfEQ::channels( size_t size ){
    
    size_t oldsize = eqs.size();
    
    if( size >= oldsize ){
        eqs.resize( size );
                
        for( size_t i=oldsize; i<eqs.size(); ++i ){
            eqs[i] = new pdsp::BiquadLowShelf();
            freq >> eqs[i]->in_freq();   
            Q >> eqs[i]->in_Q();   
            gain >> eqs[i]->in_gain();
        }        
    }else{
        for( size_t i=size; i<oldsize; ++i ){
            delete eqs[i];
        }
        eqs.resize( size );
    }
}

pdsp::Patchable& pdsp::LowShelfEQ::ch( size_t index ){
    if( index >= eqs.size() ){
        channels(index+1);
    }
    return *(eqs[index]);
}

pdsp::Patchable& pdsp::LowShelfEQ::in_freq(){
    return in("freq");
}

pdsp::Patchable& pdsp::LowShelfEQ::in_Q(){
    return in("Q");
}

pdsp::Patchable& pdsp::LowShelfEQ::in_gain(){
    return in("gain");
}

pdsp::Patchable& pdsp::LowShelfEQ::in_signal(){
    return in("signal");
}

pdsp::Patchable& pdsp::LowShelfEQ::out_signal(){
    return out("signal");
}


// ----------------- backward compatibility -------------------------

pdsp::Patchable& pdsp::LowShelfEQ::operator[]( size_t index ){
    return ch( index );
}

pdsp::Patchable& pdsp::LowShelfEQ::in_0(){
    if( eqs.size()<2 ){
        channels(2);
        addModuleInput( "0", *eqs[0] );
        addModuleInput( "1", *eqs[1] );
        addModuleOutput( "0", *eqs[0] );
        addModuleOutput( "1", *eqs[1] );
    } 
    return in("0");
}
    
pdsp::Patchable& pdsp::LowShelfEQ::in_1(){
    if( eqs.size()<2 ){
        channels(2);
        addModuleInput( "0", *eqs[0] );
        addModuleInput( "1", *eqs[1] );
        addModuleOutput( "0", *eqs[0] );
        addModuleOutput( "1", *eqs[1] );
    } 
    return in("1");
}

pdsp::Patchable& pdsp::LowShelfEQ::out_0(){
    if( eqs.size()<2 ){
        channels(2);
        addModuleInput( "0", *eqs[0] );
        addModuleInput( "1", *eqs[1] );
        addModuleOutput( "0", *eqs[0] );
        addModuleOutput( "1", *eqs[1] );
    } 
    return out("0");
}
    
pdsp::Patchable& pdsp::LowShelfEQ::out_1(){
    if( eqs.size()<2 ){
        channels(2);
        addModuleInput( "0", *eqs[0] );
        addModuleInput( "1", *eqs[1] );
        addModuleOutput( "0", *eqs[0] );
        addModuleOutput( "1", *eqs[1] );
    } 
    return out("1");
}
