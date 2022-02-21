
#include "CombFilter.h"

pdsp::CombFilter::CombFilter() { patch(); }
pdsp::CombFilter::CombFilter(const CombFilter& other){ patch(); }
pdsp::CombFilter& pdsp::CombFilter::operator=(const CombFilter& other){ return *this; }


void pdsp::CombFilter::patch(){
    channels(1);

    addModuleInput( "signal", delays[0]->in_signal() );
    addModuleOutput( "signal", delays[0]->out_signal() );    

    addModuleInput( "pitch",  p2f );    
    addModuleInput("feedback", fbcontrol );
    addModuleInput("damping", dampcontrol );
}

pdsp::CombFilter::~CombFilter(){
    channels(0);
}

void pdsp::CombFilter::channels( std::size_t size ){
    std::size_t oldsize = delays.size();
    if( size >= oldsize ){
        delays.resize( size );
        for( std::size_t i=oldsize; i<delays.size(); ++i ){
            delays[i] = new Delay();
            p2f >> f2ms >> delays[i]->in_time();
            fbcontrol >> delays[i]->in_feedback();
            dampcontrol >> delays[i]->in_damping();
        }        
    }else{
        for( std::size_t i=size; i<oldsize; ++i ){
            delete delays[i];
        }
        delays.resize( size );
    }
}

pdsp::Patchable& pdsp::CombFilter::ch( std::size_t index ){
    if( index >= delays.size() ){
        channels(index+1);
    }
    return *(delays[index]);
}

pdsp::Patchable& pdsp::CombFilter::in_signal(){
    return in("signal");
}

pdsp::Patchable& pdsp::CombFilter::out_signal(){
    return out("signal");
}

pdsp::Patchable& pdsp::CombFilter::in_pitch(){
    return in("pitch");
}

pdsp::Patchable& pdsp::CombFilter::in_feedback(){
    return in("feedback");
}

pdsp::Patchable& pdsp::CombFilter::in_damping(){
    return in("damping");  
}

float pdsp::CombFilter::meter_pitch() const{
    return p2f.meter_input();
}

//------------------ legacy -----------------------------------------

pdsp::Patchable& pdsp::CombFilter::operator[]( std::size_t index ){
    return ch( index );
}

pdsp::Patchable& pdsp::CombFilter::in_fb(){
    return in("feedback");
}

