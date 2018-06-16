
#include "CombFilter.h"

pdsp::CombFilter::CombFilter() { patch(); }
pdsp::CombFilter::CombFilter(const CombFilter& other){ patch(); }
pdsp::CombFilter& pdsp::CombFilter::operator=(const CombFilter& other){ return *this; }


void pdsp::CombFilter::patch(){
    channels(1);
    
    addModuleInput( "pitch",  p2f );    
    addModuleInput("feedback", fbcontrol );
    addModuleInput("damping", dampcontrol );
}

pdsp::CombFilter::~CombFilter(){
    channels(0);
}

void pdsp::CombFilter::channels( int size ){
    int oldsize = delays.size();
    if( size >= oldsize ){
        delays.resize( size );
        for (size_t i=oldsize; i<delays.size(); ++i ){
            delays[i] = new Delay();
            p2f >> f2ms >> delays[i]->in_time();
            fbcontrol >> delays[i]->in_feedback();
            dampcontrol >> delays[i]->in_damping();
            addModuleInput( std::to_string(i).c_str(), delays[i]->in_signal() );
            addModuleOutput( std::to_string(i).c_str(), delays[i]->out_signal() );
        }        
    }else{
        for (int i=size; i<oldsize; ++i ){
            delete delays[i];
        }
        delays.resize( size );
    }
}

pdsp::Patchable& pdsp::CombFilter::operator[]( const int & ch ){
    return *(delays[ch]);
}

pdsp::Patchable& pdsp::CombFilter::in_signal(){
    return in("0");
}

pdsp::Patchable& pdsp::CombFilter::in_pitch(){
    return in("pitch");
}

pdsp::Patchable& pdsp::CombFilter::in_fb(){
    return in("fb");
}

pdsp::Patchable& pdsp::CombFilter::in_feedback(){
    return in("feedback");
}

pdsp::Patchable& pdsp::CombFilter::in_damping(){
    return in("damping");  
}

pdsp::Patchable& pdsp::CombFilter::out_signal(){
    return out("0");
}


float pdsp::CombFilter::meter_pitch() const{
    return p2f.meter_input();
}
