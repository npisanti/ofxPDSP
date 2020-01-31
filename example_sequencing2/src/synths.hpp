
#pragma once

#include "ofxPDSP.h"

// ----------------------------------------------------------------------------------------------------------------------------
class MultiSampler : public pdsp::Patchable{
    
public:
    MultiSampler() { patch(); } 
    MultiSampler( const MultiSampler & other ) { patch(); } 
    
    ~MultiSampler(){ // deallocate elements
        for ( size_t i=0; i<samples.size(); ++i ) delete samples[i];
    }
    
    void patch (){
       
        addModuleInput("trig", triggers );
        addModuleInput("position", sampler.in_start() );
        addModuleInput("pitch", sampler.in_pitch()); 
        addModuleInput("select", sampler.in_select()); 
        addModuleOutput("signal", amp );
        
        env.enableDBTriggering(-24.0f, 0.0f);
        setAHR(0.0f, 20.0f, 100.0f); // all the samples used here are shorter than this
        
        triggers >> sampler >> amp;
        triggers >> env     >> amp.in_mod();
        
        samples.reserve( 32 );
    }
    
    void add( string path, bool setHoldTime = false ){

        samples.push_back( new pdsp::SampleBuffer() );
        samples.back()->load(path);
        sampler.addSample( samples.back() );

    }
    
    void gain(float dBvalue) {
        sampler * dB(dBvalue) >> amp;
    }
    
    float meter_env() const {
        return env.meter_output();
    }

    float meter_position() const {
        return sampler.meter_position();
    }
    
    void setAHR ( float attack, float hold, float release ) {
        env.set( attack, hold, release );
    }
    
private:
    pdsp::PatchNode     triggers;
    pdsp::Sampler       sampler;
    pdsp::AHR           env;
    pdsp::Amp           amp;

    std::vector<pdsp::SampleBuffer*> samples;
};


// ----------------------------------------------------------------------------------------------------------------------------
class Reese : public pdsp::Patchable{
    
public:
    Reese() { patch(); } 
    Reese( const Reese & other ) { patch(); } 
    
    void patch (){
       
        addModuleInput("trig", env.in_trig() );
        addModuleInput("pitch", pitchNode); 
        addModuleOutput("signal", amp );
        
        osc1.out_saw() * dB(12.0f) >> drive >> filter >> amp;
        osc2.out_saw() * dB(12.0f) >> drive;
      
        65.0f >> filter.in_cutoff();
        0.1f  >> filter.in_reso();

        env.enableDBTriggering(-24.0f, 0.0f);              
        env.set(40.0f, 100.0f, 1.0f, 200.0f) >> amp.in_mod();
        
        pitchNode >> osc1.in_pitch(); 
           -0.30f >> osc1.in_pitch(); 
            0.30f >> osc2.in_pitch(); 
        pitchNode >> osc2.in_pitch();         
    }
        
private:

    pdsp::PatchNode     pitchNode;
    pdsp::VAOscillator  osc1;
    pdsp::VAOscillator  osc2;
    pdsp::Saturator1    drive;
    pdsp::VAFilter      filter;
    pdsp::ADSR          env;
    pdsp::Amp           amp;
    
};

