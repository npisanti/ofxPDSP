
#include "BitNoise.h"

pdsp::BitNoise::BitNoise() { patch(); }
pdsp::BitNoise::BitNoise(const pdsp::BitNoise &Other){ patch(); }
pdsp::BitNoise& pdsp::BitNoise::operator=(const pdsp::BitNoise &Other){ return *this; }


void pdsp::BitNoise::patch(){
    
    addModuleInput( "pitch",        p2fPhazor );
    addModuleInput( "decimation",   p2fDecimator );    
    addModuleInput( "bits",         bits_ctrl );  
    addModuleInput( "trig",         trigger );  
    
    addModuleOutput( "decimated_L",   bitcrunchA );  
    addModuleOutput( "noise_L",       noiseA ); 
     
    addModuleOutput( "decimated_R",   bitcrunchB );  
    addModuleOutput( "noise_R",       noiseB );  

                               p2fDecimator  >> decimateA.in_freq();
                                      noiseA >> decimateA >> bitcrunchA;
                 phasor.out_trig() >> noiseA.in_clock();
    p2fPhazor >> phasor.in_freq();

                               p2fDecimator  >> decimateB.in_freq();
                                      noiseB >> decimateB >> bitcrunchB;
                 phasor.out_trig() >> noiseB.in_clock();
    p2fPhazor >> phasor.in_freq();

    p2fPhazor.set( - 100.0f );
    p2fDecimator.set(151.0f );
    bits_ctrl.set( 12.0f );

    trigger >> noiseA.in_reseed();
    trigger >> noiseB.in_reseed();
    trigger >> phasor.in_retrig();
    bits_ctrl >> bitcrunchA.in_bits();
    bits_ctrl >> bitcrunchB.in_bits();
}

pdsp::Patchable& pdsp::BitNoise::in_pitch(){
    return in("pitch");
}

pdsp::Patchable& pdsp::BitNoise::in_decimation(){
    return in("decimation");
}

pdsp::Patchable& pdsp::BitNoise::in_bits(){
    return in("bits");
}

pdsp::Patchable& pdsp::BitNoise::in_trig(){
    return in("trig");
}

pdsp::Patchable& pdsp::BitNoise::ch( size_t index ){
    
    wrapChannelIndex( index, 2, "pdsp::BitNoise" );
    
    switch( index ){
        case 0: return out("decimated_L"); break;
        case 1: return out("decimated_R"); break;
    }
    
    return out("decimated_L");
}

pdsp::Patchable& pdsp::BitNoise::ch_noise( size_t index ){
    
    wrapChannelIndex( index );
    
    switch( index ){
        case 0: return out("noise_L"); break;
        case 1: return out("noise_R"); break;
    }
    
    return out("noise_L");
}


// -------------- backward compatibility ----------------------------

pdsp::Patchable& pdsp::BitNoise::out_decimated(){
    return out("decimated_L");
}

pdsp::Patchable& pdsp::BitNoise::out_noise(){
    return out("noise_L");
}

pdsp::Patchable& pdsp::BitNoise::out_L(){
    return out("decimated_L");
}

pdsp::Patchable& pdsp::BitNoise::out_noise_L(){
    return out("noise_L");
}

pdsp::Patchable& pdsp::BitNoise::out_R(){
    return out("decimated_R");
}

pdsp::Patchable& pdsp::BitNoise::out_noise_R(){
    return out("noise_R");
}

pdsp::Patchable& pdsp::BitNoise::out_0(){
    return out("decimated_L");
}

pdsp::Patchable& pdsp::BitNoise::out_noise_0(){
    return out("noise_L");
}

pdsp::Patchable& pdsp::BitNoise::out_1(){
    return out("decimated_R");
}

pdsp::Patchable& pdsp::BitNoise::out_noise_1(){
    return out("noise_R");
}
