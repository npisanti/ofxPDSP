
#include "synth.h"

void PolySynth::setup(int numVoices){
    
    // WAVETABLE OPERATION EXAMPLE --------------------------------------------
    wavetable.setVerbose( true ); // activate logs of waveform loadings and size increments
    
    wavetable.setup( 600, 256 ); // 600 samples is AKWF sample length, 256 max partials

    // methods for classic waveforms, you can give the number of partials of the waveform
    // more partial = fuller sound but with more aliasing, less partial = duller but with less aliasing
    // highestPartial( float pitch) gives you the max number of partial before aliasing at a given pitch

    wavetable.addSawWave( pdsp::highestPartial(60.0f) ); // this saw wave won't alias before C3=60
    wavetable.addSquareWave( pdsp::highestPartial(72.0f) ); // this saw wave won't alias before C4=72
    wavetable.addTriangleWave( 64 ); // Triangle wave with 64 partials
    wavetable.addSineWave();
   
    // adding waveforms from samples
    // the old trusty Adventure Kid WaveForms   
    // http://www.adventurekid.se/akrt/waveforms/adventure-kid-waveforms/
    wavetable.addSample( "data/AKWF_0001.wav");
    wavetable.addSample( "data/AKWF_0022.wav");
    wavetable.addSample( "data/AKWF_0042.wav");
    

    // creating a wave from the values of each partial sine wave amplitude
    wavetable.addAdditiveWave ( { 1.0f, 1.0f, 1.0f, 1.0f } ); // first 4 partial at full amplitude, like an hammond set at 8888
    
    
    // this waveform is harmonically scaled to the standard harmonic series, so just the first partial is at full amplitude
    // (unless you use values greater than 1.0f for the other partials )
    // true == harmonically scaling active, false by default 
    wavetable.addAdditiveWave ({ 1.0f, 0.0f, -1.0f, 0.5f, 0.5f, 1.0f, -1.0f, 0.5f, 0.5f, 1.0f, -1.0f, 0.5f, 0.5f, 1.0f, -1.0f, 0.5f }, true ); 
  
  
    // additive wave from a vector, we set a partial every three to 1
    // remember that index 0 of the vector is the fundamental
    partials_vector.resize(64);
    for(int i=0; i<(int)partials_vector.size(); ++i){
        if(i%3==0){
            partials_vector[i] = 1.0; // odd harmonics 100%
        }else{
            partials_vector[i] = 0.0; // no even harmonics
        }
    }
    // remember to set "true" for harmonic scaling, otherwise you should have set the partials directly to the scaled amplitude
    wavetable.addAdditiveWave ( partials_vector, true ); 
  
  
    // creating a NOISY WAVE, manually setting the buffer values
    // half of the waveform shape is just noise, the other half is silence
    wavetable.addEmpty();
    int index = wavetable.size() -1;
    int half = wavetable.tableLength() / 2;
    // setting the wavetable buffer
    for(int n=0; n<wavetable.tableLength(); n++){
        if(n<half){
            // wavetable.table(int index) gives you access to a waveform buffer
            wavetable.table(index)[n] = pdsp::brand(); // random value in the -1.0f <--> 1.0f range
        }else{
            wavetable.table(index)[n] = 0.0f;
        }
    }
    // if you are careful with table index you can change waveform buffer values while you are not using them
    // and then switch the table index when you're done
    // ------------------------------------------------------------------------

    // -------------------------- PATCHING ------------------------------------
    voices.resize( numVoices );
 
    for(int i=0; i<numVoices; ++i){
        voices[i].setup( *this, i );
    }
    
    chorus.ch(0) >> gain.ch(0);
    chorus.ch(1) >> gain.ch(0);

    // pdsp::Switch EXAMPLE ---------------------------------------------------
    lfo_switch.resize(5);  // resize input channels
    lfo.out_triangle()          >> lfo_switch.input(0); // you cannot use this input() method in a chain
    lfo.out_saw()               >> lfo_switch.input(1); // because: API reasons
    lfo.out_square()            >> lfo_switch.input(2);
    lfo.out_sine()              >> lfo_switch.input(3);
    lfo.out_sample_and_hold()   >> lfo_switch.input(4);
 
    lfo_wave_ctrl >> lfo_switch.in_select(); // input for output selection
 
    lfo_speed_ctrl >> lfo.in_freq();          
    lfo_switch >> lfo_to_table;
    lfo_switch >> lfo_to_filter;
    table_lfo_mod_ctrl  >> lfo_to_table.in_mod();
    filter_lfo_mod_ctrl >> lfo_to_filter.in_mod();
    // ------------------------------------------------------------------------
    
    // CONTROLS ---------------------------------------------------------------
    ui.setName("WAVESYNTH");
    ui.add(table_ctrl.set("table index", 8.0f, 0.0f, (float)(wavetable.size()-1) ) );
    table_ctrl.enableSmoothing(200.0f);

    ui.add(filter_mode_ctrl.set("filter mode", 0, 0, 5) );
    ui.add(cutoff_ctrl.set("filter cutoff", 82, 10, 120));
    ui.add(reso_ctrl.set("filter reso", 0.0f, 0.0f, 1.0f) );
 
    cutoff_ctrl.enableSmoothing(200.0f);
    
    ui.add(env_attack_ctrl.set( "env attack", 400, 5, 1200) );
    ui.add(env_decay_ctrl.set(  "env decay", 400, 5, 1200) );
    ui.add(env_sustain_ctrl.set("env sustain", 1.0f, 0.0f, 1.0f) );
    ui.add(env_release_ctrl.set("env release", 900, 5, 2000));    
    ui.add( env_table_amt.set( "env to table", 0.0f, 0.0f, 2.0f) );
    ui.add( env_filter_amt.set("env to filter", 30, 0, 60) );    

    ui.add(lfo_wave_ctrl.set("lfo wave", 0, 0, 4));
    ui.add(lfo_speed_ctrl.set("lfo freq", 0.5f, 0.005f, 4.0f));
    ui.add(table_lfo_mod_ctrl.set("lfo to table", 1.0f, 0.0f, 2.0f) );
    ui.add(filter_lfo_mod_ctrl.set("lfo to filter", 0, 0, 60) );
    // ------------------------------------------------------------------------
   
    // Chorus -----------------------------------------------------------------
    chorus_speed_ctrl >> chorus.in_speed();
    chorus_depth_ctrl >> chorus.in_depth();
    ui.add(chorus_speed_ctrl.set("chorus freq", 0.5f, 0.25f, 1.0f));
    ui.add(chorus_depth_ctrl.set("chorus depth", 3.5f, 1.0f, 10.0f));
    ui.add(gain.set("gain", -12, -48, 12));
    gain.enableSmoothing(50.f);
    // ------------------------------------------------------------------------
}


void PolySynth::Voice::setup( PolySynth & m, int v ){

    addModuleInput("trig", voiceTrigger);
    addModuleInput("pitch", oscillator.in_pitch());
    addModuleOutput("signal", voiceAmp);

    oscillator.setTable( m.wavetable );

    // SIGNAL PATH
    oscillator >> filter >> voiceAmp >> m.chorus.ch(0);
                            voiceAmp >> m.chorus.ch(1);
    
    // MODULATIONS AND CONTROL
    envelope >> m.env_table_amt.ch(v) >> oscillator.in_table();
                       m.lfo_to_table >> oscillator.in_table();
                         m.table_ctrl >> oscillator.in_table();

    voiceTrigger >> envelope >> voiceAmp.in_mod();
                    envelope >> m.env_filter_amt.ch(v) >> filter.in_pitch();
                                       m.lfo_to_filter >> filter.in_pitch();
                                         m.cutoff_ctrl >> filter.in_pitch();
                                           m.reso_ctrl >> filter.in_reso();
                                    m.filter_mode_ctrl >> filter.in_mode();


        m.env_attack_ctrl  >> envelope.in_attack();
        m.env_decay_ctrl   >> envelope.in_decay();
        m.env_sustain_ctrl >> envelope.in_sustain();
        m.env_release_ctrl >> envelope.in_release();
}

float PolySynth::Voice::meter_mod_env() const{
    return envelope.meter_output();
}

float PolySynth::Voice::meter_pitch() const{
    return oscillator.meter_pitch();
}

pdsp::Patchable& PolySynth::ch( int index ){
    index = index%2;
    return gain.ch( index );
}
