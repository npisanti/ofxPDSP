
#include "Zaps.h"


void Zap::patch(){
    addModuleInput("trig", triggers); 
    addModuleInput("pitch", osc.in_pitch()); 
    addModuleInput("pitch_mod_amount", pModAmt.in_mod()); 
    addModuleInput("mod_env_time", pEnv.in_release()); 
    addModuleInput("mod_env_velo", pEnv.in_velocity()); 
    addModuleInput("amp_env_time", aEnv.in_hold()); 
    addModuleOutput("signal", amp ); 

    //patching
                                triggers >> aEnv.set(3.0f, 40.0f, 20.0f)  >> amp.in_mod();
                                                                      osc >> amp;
    triggers >> pEnv.set(0.0f, 0.0f,  50.0f) >> pModAmt.set(48.0f) >> osc.in_pitch();
                                                      pitchControl >> osc.in_pitch(); // overriden by ensemble

    pitchControl.setv(36.0f);
    
}           

void Zap::enableDB( float minValue ){
    aEnv.enableDBTriggering( minValue, 0.0f);
}

float Zap::meter_env() const {
    return aEnv.meter_output();
}


void Zaps::setup( int numVoices ) {
    
    voices.resize(numVoices);
    
    ui.setName("zaps UI");
    ui.add(fader.set("gain", -12, -48, 12 ) );
    ui.add(envAmtControl.set("env to pitch", 48, 0, 72 ) );
    ui.add(envDecayControl.set("env decay", 50, 5, 100 ) );
    ui.add(veloToEnv.set( "env dyn amt", 0.5f, 0.0f, 1.0f ) );

    float panWidth = 0.30f;
    
    for( size_t i=0; i<voices.size(); ++i){
        veloToEnv >> voices[i].in("mod_env_velo");
        envAmtControl >> voices[i].in("pitch_mod_amount");
        envDecayControl >> voices[i].in("mod_env_time");
        string label = "zap ";
        label += ofToString( i+1 );
        label += " pitch";
        ui.add( voices[i].pitchControl.set(label, 46+i*7, 24, 84 ));

        float pan = pdspSpread( i, voices.size(), panWidth ); 
        voices[i] * panL(pan) >> fader.in_L();
        voices[i] * panR(pan) >> fader.in_R();
    }

}
