
This is an overview of a selection of the available modules:

## Thread-Safe Control
- [ofxPDSPValue](classofx_p_d_s_p_value.html) : holds a value to control the modules (with optional smoothing), has ofParameter integration for GUIs
- [pdsp::TriggerControl / ofxPDSPTrigger](classpdsp_1_1_trigger_control.html) : sent triggers to the envelopes

## Oscillators / Noise
- [pdsp::VAOscillator](classpdsp_1_1_v_a_oscillator.html) : virtual analog osc with saw, pulse, triangle and sine outputs
- [pdsp::TableOscillator](classpdsp_1_1_table_oscillator.html) : wavetable oscillator, check the code in the included examples
- [pdsp::FMOperator](classpdsp_1_1_f_m_operator.html) : sine oscillator with pm input for FM synthesis
- [pdsp::PRNoiseGen](classpdsp_1_1_p_r_noise_gen.html) : noise generator

## Filters
- [pdsp::MultiLadder4](classpdsp_1_1_multi_ladder4.html) : resonant 4 pole classic ladder with 12db/24db lowpass, highpass and bandpass outputs
- [pdsp::SVF2](classpdsp_1_1_s_v_f2.html)  : resonant 2 pole state variable filter with lowpass, highpass and bandpass output
- [pdsp::OnePole](classpdsp_1_1_one_pole.html) : non-resonant one pole highpass / lowpass filter, you can also usit to slew control rate signals
- [pdsp::APF1](classpdsp_1_1_a_p_f1.html) : one pole All Pass Filter
- [pdsp::APF4](classpdsp_1_1_a_p_f4.html) : 4pole All Pass Filter (useful for phasers) with feedback control
- [pdsp::CombFilter](classpdsp_1_1_comb_filter.html) : Comb Filter (this is basically a tuned delay)

## Delays
- [pdsp::DampedDelay](classpdsp_1_1_damped_delay.html) : delay with feedback damping
- [pdsp::AllPassDelay](classpdsp_1_1_all_pass_delay.html) : delay with an allpass filter in the feedback path

## Envelopes / Amp
- [pdsp::ADSR](classpdsp_1_1_a_d_s_r.html) : classic Attack-Decay-Sustain-Release envelope
- [pdsp::AHR](classpdsp_1_1_a_h_r.html) : one-shot Attack-Hold-Release envelope, responsive only to positive triggers
- [pdsp::Amp](classpdsp_1_1_amp.html) : multiplies an input signal for a mod signal

## LFO / Modulators
- [pdsp::LFO](classpdsp_1_1_l_f_o.html) : Low Frequency Oscillator
- [pdsp::ClockedLFO](classpdsp_1_1_clocked_l_f_o.html) : LFO synchronized to the clock tempo
- [pdsp::TriggeredRandom](classpdsp_1_1_triggered_random.html) : Random value changed by a trigger input
- [pdsp::SampleAndHold](classpdsp_1_1_sample_and_hold.html) : Hold the signal when it receives a trigger 

## Samplers
- [pdsp::Sampler](classpdsp_1_1_sampler.html) : one-shot sampler
- [pdsp::GrainCloud](classpdsp_1_1_grain_cloud.html) : classic granular sampler

## Signal / Effects
- [pdsp::Bitcrucher](classpdsp_1_1_bitcruncher.html) : reduces the bit for the signal amplitude
- [pdsp::Decimator](classpdsp_1_1_decimator.html) : reduces the signal sampling frequency
- [pdsp::Saturator1](classpdsp_1_1_saturator1.html) : overdrive 
- [pdsp::DimensionChorus](classpdsp_1_1_dimension_chorus.html) : lush chorus

## Equalizers
- [pdsp::LowCut](classpdsp_1_1_low_cut.html) : cuts low frequencies
- [pdsp::LowShelfEQ](classpdsp_1_1_low_shelf_e_q.html) : low shelf equalizer
- [pdsp::PeakEQ](classpdsp_1_1_peak_e_q.html) : peak equalizer
- [pdsp::HighShelfEQ](classpdsp_1_1_high_shelf_e_q.html) : high shelf equalizer

## Reverbs  
- [pdsp::BasiVerb](classpdsp_1_1_basi_verb.html) : dubby metallic reverb   
- [pdsp::IRVerb](classpdsp_1_1_i_r_verb.html) : a impulse response based reverb  

## Routing
- [pdsp::LinearCrossfader](classpdsp_1_1_linear_crossfader.html) : crossfade between two signals
- [pdsp::Panner](classpdsp_1_1_panner.html) : pans the signal from the left to the right
- [pdsp::Switch](classpdsp_1_1_switch.html) : switches between different inputs

## Utility
- [pdsp::BipolarToUnipolar](classpdsp_1_1_bipolar_to_unipolar.html) : convert a bipolar signal to an unipolar one
- [pdsp::OneBarTimeMs](classpdsp_1_1_one_bar_time_ms.html) : outputs the time in ms for one musical bar
- [pdsp::PitchToFreq](classpdsp_1_1_pitch_to_freq.html) : converts pitch values to frequency values
- [pdsp::FreqToMs](classpdsp_1_1_freq_to_ms.html) : convert frequency values to millisecond values 
- [pdsp::Formula](classpdsp_1_1_formula.html) : base class for executing a matematical operation on an input

