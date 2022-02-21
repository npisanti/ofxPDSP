
#include "HighShelfEQ.h"

void pdsp::HighShelfEQ::patch(){
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

void pdsp::HighShelfEQ::channels( std::size_t size ){
    
    std::size_t oldsize = eqs.size();
    
    if( size >= oldsize ){
        eqs.resize( size );
                
        for( std::size_t i=oldsize; i<eqs.size(); ++i ){
            eqs[i] = new pdsp::BiquadHighShelf();
            freq >> eqs[i]->in_freq();   
            Q >> eqs[i]->in_Q();   
            gain >> eqs[i]->in_gain();
        }        
    }else{
        for( std::size_t i=size; i<oldsize; ++i ){
            delete eqs[i];
        }
        eqs.resize( size );
    }
}

pdsp::Patchable& pdsp::HighShelfEQ::ch( std::size_t index ){
    if( index >= eqs.size() ){
        channels(index+1);
    }
    return *(eqs[index]);
}

pdsp::Patchable& pdsp::HighShelfEQ::in_freq(){
    return in("freq");
}

pdsp::Patchable& pdsp::HighShelfEQ::in_Q(){
    return in("Q");
}

pdsp::Patchable& pdsp::HighShelfEQ::in_gain(){
    return in("gain");
}

pdsp::Patchable& pdsp::HighShelfEQ::in_signal(){
    return in("signal");
}

pdsp::Patchable& pdsp::HighShelfEQ::out_signal(){
    return out("signal");
}


// ----------------- backward compatibility -------------------------

pdsp::Patchable& pdsp::HighShelfEQ::operator[]( std::size_t index ){
    return ch( index );
}

pdsp::Patchable& pdsp::HighShelfEQ::in_0(){
    if( eqs.size()<2 ){
        channels(2);
        addModuleInput( "0", *eqs[0] );
        addModuleInput( "1", *eqs[1] );
        addModuleOutput( "0", *eqs[0] );
        addModuleOutput( "1", *eqs[1] );
    } 
    return in("0");
}
    
pdsp::Patchable& pdsp::HighShelfEQ::in_1(){
    if( eqs.size()<2 ){
        channels(2);
        addModuleInput( "0", *eqs[0] );
        addModuleInput( "1", *eqs[1] );
        addModuleOutput( "0", *eqs[0] );
        addModuleOutput( "1", *eqs[1] );
    } 
    return in("1");
}

pdsp::Patchable& pdsp::HighShelfEQ::out_0(){
    if( eqs.size()<2 ){
        channels(2);
        addModuleInput( "0", *eqs[0] );
        addModuleInput( "1", *eqs[1] );
        addModuleOutput( "0", *eqs[0] );
        addModuleOutput( "1", *eqs[1] );
    } 
    return out("0");
}
    
pdsp::Patchable& pdsp::HighShelfEQ::out_1(){
    if( eqs.size()<2 ){
        channels(2);
        addModuleInput( "0", *eqs[0] );
        addModuleInput( "1", *eqs[1] );
        addModuleOutput( "0", *eqs[0] );
        addModuleOutput( "1", *eqs[1] );
    } 
    return out("1");
}

