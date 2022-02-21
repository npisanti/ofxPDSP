
#include "BasiVerb.h"

pdsp::BasiVerb::BasiVerb() { patch(); }
pdsp::BasiVerb::BasiVerb(const BasiVerb& other){ patch(); }
pdsp::BasiVerb& pdsp::BasiVerb::operator=(const BasiVerb& other){ return *this; }

//const float pdsp::BasiVerb::RT60Calculator::delay_mult [] = { 1.0f, 1.10007f, 1.332f, 1.355f, 1.3999f, 1.455f };
//const float pdsp::BasiVerb::RT60Calculator::delay_mult [] = { 1.0f, 1.0689655f, 1.2105263157f, 1.307692307f, 1.34782608f, 1.4f };
const float pdsp::BasiVerb::RT60Calculator::delay_mult [] = { 1.0f, 1.12f, 1.22f, 1.36f, 1.44f, 1.56f };
const float pdsp::BasiVerb::damp_mult [] = { 0.8964f, 0.8798f, 0.87f, 0.8632f, 0.8466f, 0.751f };

const float pdsp::BasiVerb::RT60Calculator::delay_min = 25.0f;
const float pdsp::BasiVerb::RT60Calculator::delay_range = 50.0f;


pdsp::BasiVerb::RT60Calculator::RT60Calculator(){
    
    addInput("rt60",   input_rt60 );
    addInput("density", input_shape );
    
    addOutput( "g0", delay_g[0] );
    addOutput( "g1", delay_g[1] );
    addOutput( "g2", delay_g[2] );
    addOutput( "g3", delay_g[3] );
    addOutput( "g4", delay_g[4] );
    addOutput( "g5", delay_g[5] );
    addOutput( "time0", delay_times[0] );
    addOutput( "time1", delay_times[1] );
    addOutput( "time2", delay_times[2] );
    addOutput( "time3", delay_times[3] );
    addOutput( "time4", delay_times[4] );
    addOutput( "time5", delay_times[5] );
    
    updateOutputNodes();
    
    input_shape.setDefaultValue(0.85f); // default density
    input_rt60.setDefaultValue(3.33f); // default time
    
    if(dynamicConstruction){
        prepareToPlay(globalBufferSize, globalSampleRate);
    }
}

pdsp::BasiVerb::RT60Calculator::~RT60Calculator(){ }

void pdsp::BasiVerb::RT60Calculator::prepareUnit( int expectedBufferSize, double sampleRate ) {};
void pdsp::BasiVerb::RT60Calculator::releaseResources () {};

void pdsp::BasiVerb::RT60Calculator::process (int bufferSize) noexcept {
    
    int changed;
    const float * rt60Buffer = processInput(input_rt60, changed);
    int shapeState;
    const float * shapeBuffer = processInput(input_shape, shapeState);  
    
    changed += shapeState;
    
    if(changed){
        
        float oneSlashRT60 = 1.0f / rt60Buffer[0];
        
        float dens = shapeBuffer[0];
        if(dens > 1.0f){
            dens = 1.0f;
        }else if(dens < 0.0f){
            dens = 0.0f;
        }
        
        float delay_time_one = delay_min + ( (1.0f - dens ) * delay_range );
        //float delay_time_one = 50.0f; // 50 msec
        
        for(int i= 0; i< 6; ++i){
            
            float dt = delay_time_one * delay_mult[i]; 
            delay_times[i] << dt; 
            
            float exponent = 3.0f * dt * 0.001f * oneSlashRT60 ;
            float g = 1.0f / powf(10.0f, exponent);;
            if(g >= 0.0f){
                delay_g[i] << g; 
            }else{
                delay_g[i] << 0.0f; 
            }
        }
    }
    
}


void pdsp::BasiVerb::patch(){
    
    addModuleInput( "signal",  input_signal );
    addModuleInput("time", coeffs.in("rt60") );
    addModuleInput("density", coeffs.in("density") );
    addModuleInput("damping", damping_ctrl );
    addModuleInput("hi_cut", hi_cut_ctrl );
   
    addModuleInput("mod_freq", phasor.in_freq() );
    addModuleInput("mod_amount", modAmt.in_mod() );
 
    addModuleOutput("L", output1);
    addModuleOutput("R", output2);
    
    hi_cut_ctrl.set(5000.0f);
    damping_ctrl.set(0.5f);  
    modAmt.set(0.8f); 
    phasor.set(0.2f); // standar freq = 0.1hz


    phasor.in_freq() >> LFO >> lfoOut >> modAmt;
    
    for(int i=0; i<6; ++i){

        input_signal >> delays[i] >> lpf1; 
        if(i%2==0){
            delays[i] >> lpf2;
        }else{
            delays[i] * (-1.0f) >> lpf2; 
        }
        
        coeffs.delay_times[i] >> delays[i].in_time();
        modAmt                >> delays[i].in_time();
        
        coeffs.delay_g[i]     >> delays[i].in_feedback();
        damping_ctrl  * damp_mult[i] >> delays[i].in_damping();

    }
    
    /*
    54.81f >> apf1L.in_freq();
    62.64f >> apf1R.in_freq();
    
    0.707f >> apf1L.in_feedback();
    0.707f >> apf1R.in_feedback();
    
    lpf1 >> apf1L * 0.05f >> output1;
    lpf2 >> apf1R * 0.05f >> output2;
    */

    6.0f >> APF_1.in_time();
    6.0f >> APF_2.in_time();
    
    0.707f >> APF_1.in_feedback();
    0.707f >> APF_1.in_feedback();
    
    
    lpf1 >> APF_1 * 0.05f >> output1;
    lpf2 >> APF_2 * 0.05f >> output2;

}

pdsp::Patchable& pdsp::BasiVerb::in_signal(){
    return in("signal");
}

pdsp::Patchable& pdsp::BasiVerb::in_time(){
    return in("time");
}

pdsp::Patchable& pdsp::BasiVerb::in_density(){
    return in("density");
}

pdsp::Patchable& pdsp::BasiVerb::in_damping(){
    return in("damping");
}

pdsp::Patchable& pdsp::BasiVerb::in_hi_cut(){
    return in("hi_cut");
}

pdsp::Patchable& pdsp::BasiVerb::in_mod_freq(){
    return in("mod_freq");
}

pdsp::Patchable& pdsp::BasiVerb::in_mod_amount(){
    return in("mod_amount");
}

pdsp::Patchable& pdsp::BasiVerb::ch( std::size_t index ){
    
    wrapChannelIndex( index, 2, "pdsp::BasiVerb" );
    
    switch( index ){
        case 0: return out("L"); break;
        case 1: return out("R"); break;
    }
    
    return out("L");
}


pdsp::Patchable& pdsp::BasiVerb::out_0(){
    return out("L");
}

pdsp::Patchable& pdsp::BasiVerb::out_1(){
    return out("R");
}

pdsp::Patchable& pdsp::BasiVerb::out_L(){
    return out("L");
}

pdsp::Patchable& pdsp::BasiVerb::out_R(){
    return out("R");
}

float pdsp::BasiVerb::meter_lfo() const {
    return lfoOut.meter_output();
}

