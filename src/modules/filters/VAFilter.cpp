
#include "VAFilter.h"

const float pdsp::VAFilter::LowPass24 = 0.0f;
const float pdsp::VAFilter::LowPass12 = 1.0f;
const float pdsp::VAFilter::HighPass24 = 2.0f;
const float pdsp::VAFilter::HighPass12 = 3.0f;
const float pdsp::VAFilter::BandPass24 = 4.0f;
const float pdsp::VAFilter::BandPass12 = 5.0f;
            

void pdsp::VAFilter::patch(){
 
    channels(1);

    addModuleInput("pitch", p2f);
    addModuleInput("reso", reso);
    addModuleInput("mode", mode);    
    
    p2f.set(80.0f);
    mode.set(0.0f);
    
}

pdsp::VAFilter::~VAFilter(){
    channels(0);
}

void pdsp::VAFilter::channels( int size ){
    
    int oldsize = filters.size();
    
    if( size >= oldsize ){
        filters.resize( size );
        switches.resize( size );
        
        for (size_t i=oldsize; i<filters.size(); ++i ){
            filters[i] = new MultiLadder4();
            switches[i] = new Switch();
            
            switches[i]->resize(6);
            p2f >> filters[i]->in_freq();
            reso >> filters[i]->in_reso();
            mode >> switches[i]->in_select();
            filters[i]->out_lpf4() >> switches[i]->input(0);
            filters[i]->out_lpf2() >> switches[i]->input(1);
            filters[i]->out_hpf4() >> switches[i]->input(2);
            filters[i]->out_hpf2() >> switches[i]->input(3);
            filters[i]->out_bpf4() >> switches[i]->input(4);
            filters[i]->out_bpf2() >> switches[i]->input(5);
            
            addModuleInput( std::to_string(i).c_str(), *filters[i] );
            addModuleOutput( std::to_string(i).c_str(), *switches[i] );
        }        
    }else{
        for (int i=size; i<oldsize; ++i ){
            delete filters[i];
            delete switches[i];
        }
        filters.resize( size );
        switches.resize( size );
    }
}


pdsp::Patchable& pdsp::VAFilter::ini( int ch ){
    return in( std::to_string(ch).c_str() );
}

pdsp::Patchable& pdsp::VAFilter::outi( int ch ){
    return out( std::to_string(ch).c_str() );
}


pdsp::Patchable& pdsp::VAFilter::in_0(){
    if( filters.size()<2 ) channels(2);
    return in("0");
}
    
pdsp::Patchable& pdsp::VAFilter::in_1(){
    if( filters.size()<2 ) channels(2);
    return in("1");
}

pdsp::Patchable& pdsp::VAFilter::in_L(){
    if( filters.size()<2 ) channels(2);
    return in("0");
}
    
pdsp::Patchable& pdsp::VAFilter::in_R(){
    if( filters.size()<2 ) channels(2);
    return in("1");
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

pdsp::Patchable& pdsp::VAFilter::out_0(){
    if( filters.size()<2 ) channels(2);
    return out("0");
}

pdsp::Patchable& pdsp::VAFilter::out_1(){
    if( filters.size()<2 ) channels(2);
    return out("1");
}

pdsp::Patchable& pdsp::VAFilter::out_L(){
    if( filters.size()<2 ) channels(2);
    return out("0");
}

pdsp::Patchable& pdsp::VAFilter::out_R(){
    if( filters.size()<2 ) channels(2);
    return out("1");
}

float pdsp::VAFilter::meter_cutoff() const {
    return p2f.meter_input();
}      
    
    
