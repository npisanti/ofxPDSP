
#include "synth.h"

SynthGlobal::SynthGlobal(){
    
    // WAVETABLE OPERATION EXAMPLE -----------------------------------------------------------------
    // wavetable.setVerbose( true );
   
    // adding waveforms from samples
    // the old trusty Adventure Kid WaveForms   
    // http://www.adventurekid.se/akrt/waveforms/adventure-kid-waveforms/
    wavetable.addSample( "data/AKWF_0001.wav");
    wavetable.addSample( "data/AKWF_0022.wav");
    wavetable.addSample( "data/AKWF_0042.wav");
    
    // creating a wave from an harmonic series 
    wavetable.addAdditiveWave ( { 1.0, 1.0, 1.0, 1.0 } ); // first 4 partial at full amplitude, like an hammond organ set at 8888
    
    // this waveform is harmonically scaled to the standard harmonic series, so just the first partial is at full amplitude
    // (unless you use values greater than 1.0f for the other partials )
    // true == harmonically scaling active, false by default 
    wavetable.addAdditiveWave ( { 1.0, 0.0, -1.0, 0.5, 0.5, 1.0, -1.0, 0.5, 0.5, 1.0, -1.0, 0.5, 0.5, 1.0, -1.0, 0.5, }, true ); 
    
    // creating a NOISY WAVE, manually setting the buffer values
    // half of the waveform shape is just noise, the other half is silence
    wavetable.addEmpty();
    int index = wavetable.size() -1;
    int half = wavetable.tableLength() / 2;
    // setting the wavetable buffer
    for(int n=0; n<wavetable.tableLength(); n++){
        if(n<half){
            // wavetable.table(int index) gives you access to a waveform buffer
            wavetable.table(index)[n] = pdspBRan(); // random value in the -1.0f <--> 1.0f range
        }else{
            wavetable.table(index)[n] = 0.0f;
        }
    }
    // if you are careful with table index you can change waveform buffer values while you are not using them
    // and then switch the table index when you're done
    // ---------------------------------------------------------------------------------------------

    // pdsp::Switch EXAMPLE ------------------------------------------------------------------------
    lfoSwitch.resize(5);  // resize input channels
    lfo.out_triangle()          >> lfoSwitch.input(0); // you cannot use this input() method in a chain
    lfo.out_saw()               >> lfoSwitch.input(1); // because: API reasons
    lfo.out_square()            >> lfoSwitch.input(2);
    lfo.out_sine()              >> lfoSwitch.input(3);
    lfo.out_sample_and_hold()   >> lfoSwitch.input(4);
 
    lfo_wave_ctrl >> lfoSwitch.in_select(); // input for output selection
 
    lfo_speed_ctrl >> lfo.in_freq();          
    lfoSwitch >> lfoToTable;
    lfoSwitch >> lfoToFilter;
    table_lfo_mod_ctrl  >> lfoToTable.in_mod();
    filter_lfo_mod_ctrl >> lfoToFilter.in_mod();
    // ---------------------------------------------------------------------------------------------
    
    // CONTROLS ------------------------------------------------------------------------------------
    ui_osc.setName("oscillator");
    ui_osc.add(table_ctrl.set("table index", 3.0f, 0.0f, (float)(wavetable.size()-1) ) );

    ui_filter.setName("filter");
    ui_filter.add(cutoff_ctrl.set("cutoff", 82, 10, 120));
    ui_filter.add(reso_ctrl.set("resonance", 0.0f, 0.0f, 1.0f) );
    cutoff_ctrl.enableSmoothing(200.0f);
    
    ui_mod_env.setName( "envelope");
    ui_mod_env.add(env_attack_ctrl.set( "attack", 400, 5, 1200) );
    ui_mod_env.add(env_decay_ctrl.set(  "decay", 400, 5, 1200) );
    ui_mod_env.add(env_sustain_ctrl.set("sustain", 1.0f, 0.0f, 1.0f) );
    ui_mod_env.add(env_release_ctrl.set("release", 900, 5, 2000));    
    ui_mod_env.add( env_table_ctrl.set( "-> table", 0.0f, 0.0f, 2.0f) );
    ui_mod_env.add( env_filter_ctrl.set("-> filter", 30, 0, 60) );    

    ui_lfo.setName("LFO");
    ui_lfo.add(lfo_wave_ctrl.set("wave", 0, 0, 4));
    ui_lfo.add(lfo_speed_ctrl.set("speed (hz)", 0.5f, 0.005f, 4.0f));
    ui_lfo.add(table_lfo_mod_ctrl.set("-> table", 1.0f, 0.0f, 2.0f) );
    ui_lfo.add(filter_lfo_mod_ctrl.set("-> filter", 0, 0, 60) );
    // ---------------------------------------------------------------------------------------------
   
    // Chorus --------------------------------------------------------------------------------------
    chorus_speed_ctrl >> chorus.in_speed();
    chorus_depth_ctrl >> chorus.in_depth();
    ui_chorus.setName("chorus");
    ui_chorus.add(chorus_speed_ctrl.set("speed (hz)", 0.5f, 0.25f, 1.0f));
    ui_chorus.add(chorus_depth_ctrl.set("depth (ms)", 3.5f, 1.0f, 10.0f));
    // ---------------------------------------------------------------------------------------------
}

void SynthGlobal::addToGUI( ofxPanel & gui ){
    gui.add(ui_osc);
    gui.add(ui_filter);
    gui.add(ui_mod_env);
    gui.add(ui_lfo);
    gui.add(ui_chorus);
}

// as we need an external object we will not patch on construction but later with this setup()
void SynthVoice::setup( SynthGlobal & m ){

    addModuleInput("trig", voiceTrigger);
    addModuleInput("pitch", voicePitch);
    addModuleOutput("signal", voiceAmp);

    oscillator.setTable( m.wavetable );

    // SIGNAL PATH
    oscillator >> filter >> voiceAmp >> m.chorus.in_0();
                            voiceAmp >> m.chorus.in_1();
    
    // MODULATIONS AND CONTROL
    envelope >> envToTable >> oscillator.in_table();
              m.lfoToTable >> oscillator.in_table();
              m.table_ctrl >> oscillator.in_table();
                voicePitch >> oscillator.in_pitch();

    voiceTrigger >> envelope * 0.25f >> voiceAmp.in_mod();
                    envelope >> envToFilter >> p2f;
                              m.lfoToFilter >> p2f;
                              m.cutoff_ctrl >> p2f;
                                               p2f >> filter.in_cutoff(); 
                                       m.reso_ctrl >> filter.in_reso();

        m.env_attack_ctrl  >> envelope.in_attack();
        m.env_decay_ctrl   >> envelope.in_decay();
        m.env_sustain_ctrl >> envelope.in_sustain();
        m.env_release_ctrl >> envelope.in_release();

        m.env_filter_ctrl >> envToFilter.in_mod();
        m.env_table_ctrl  >> envToTable.in_mod();
}

float SynthVoice::meter_mod_env() const{
    return envelope.meter_output();
}

float SynthVoice::meter_pitch() const{
    return oscillator.meter_pitch();
}

pdsp::Patchable& SynthGlobal::out_L(){
    return chorus.out_0();
}

pdsp::Patchable& SynthGlobal::out_R(){
    return chorus.out_1();
}
