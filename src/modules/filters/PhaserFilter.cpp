
#include "PhaserFilter.h"

pdsp::PhaserFilter::PhaserFilter() { patch(); }
pdsp::PhaserFilter::PhaserFilter(const PhaserFilter& other){ patch(); }
pdsp::PhaserFilter& pdsp::PhaserFilter::operator=(const PhaserFilter& other){ return *this; }


void pdsp::PhaserFilter::patch(){
    channels(1);

    addModuleInput(  "signal", phasers[0]->in_signal() );
    addModuleOutput( "signal", phasers[0]->out_signal() );
    
    addModuleInput( "pitch",  p2f );    
    addModuleInput( "feedback", fbcontrol );
    addModuleInput( "spread", spreadcontrol );
}

pdsp::PhaserFilter::~PhaserFilter(){
    channels(0);
}

void pdsp::PhaserFilter::channels( size_t size ){
    size_t oldsize = phasers.size();
    if( size >= oldsize ){
        phasers.resize( size );
        for( size_t i=oldsize; i<phasers.size(); ++i ){
            phasers[i] = new APF4();
            p2f >> phasers[i]->in_freq();
            fbcontrol >> phasers[i]->in_feedback();
            spreadcontrol >> phasers[i]->in_spread();
        }        
    }else{
        for( size_t i=size; i<oldsize; ++i ){
            delete phasers[i];
        }
        phasers.resize( size );
    }
}

pdsp::Patchable& pdsp::PhaserFilter::ch( size_t index ){
    if( index >= phasers.size() ){
        channels(index+1);
    }
    return *(phasers[index]);
}

pdsp::Patchable& pdsp::PhaserFilter::in_signal(){
    return in("signal");
}

pdsp::Patchable& pdsp::PhaserFilter::in_pitch(){
    return in("pitch");
}

pdsp::Patchable& pdsp::PhaserFilter::in_cutoff(){
    return in("pitch");
}

pdsp::Patchable& pdsp::PhaserFilter::in_feedback(){
    return in("feedback");
}

pdsp::Patchable& pdsp::PhaserFilter::in_spread(){
    return in("spread");  
}

pdsp::Patchable& pdsp::PhaserFilter::out_signal(){
    return out("signal");
}

float pdsp::PhaserFilter::meter_pitch() const{
    return p2f.meter_input();
}

        
pdsp::Patchable& pdsp::PhaserFilter::operator[]( size_t index ){
    return ch( index );
}

