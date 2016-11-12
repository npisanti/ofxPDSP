
This is an overview of a selection of the available modules. Some of the modules can work both in mono or stereo, usually when one of the outputs is not patched that channel is not processed and the module works in mono.

## Thread-Safe Control / ofParameter compatible
- ofxPDSPValue : holds a value to control the modules (with optional smoothing), has ofParameter integration for GUIs
- pdsp::TriggerControl / ofxPDSPTrigger : send triggers
- ofxPDSPValueMultiplier : a combination of pdsp::Amp and ofxPDSPValue for easy mod amount control
- ofxPDSPMonoFader : a combination of pdsp::Amp and ofxPDSPValue for easy gain control
- ofxPDSPStereoFader : stereo version of ofxPDSPMonoFader

## Oscillators / Noise
- pdsp::VAOscillator : band limited oscillator with saw, pulse, triangle and sine outputs
- pdsp::TableOscillator : wavetable oscillator, check the code in the included examples
- pdsp::FMOperator : sine oscillator with pm input for FM synthesis
- pdsp::PRNoiseGen : noise generator
- pdsp::BitNoise : digital noise generator with lots of shaping control and stereo outputs

## Filters
- pdsp::VAFilter : stereo/mono resonant 4 pole filter with switchable 12db/24db LP, BP and HP outputs and pitched cutoff control
- pdsp::OnePole : non-resonant one pole highpass / lowpass filter, you can also usit to slew control rate signals, with control in hertz
- pdsp::APF4 : 4pole All Pass Filter (Phaser) with frequency control in hertz and feedback control
- pdsp::CombFilter : Comb Filter with frequency control in pitch, feedback and damping control (this is basically a tuned delay)

## Delays
- pdsp::DampedDelay : delay with feedback damping
- pdsp::AllPassDelay : delay with an allpass filter in the feedback path

## Envelopes / Amp
- pdsp::ADSR : classic Attack-Decay-Sustain-Release envelope
- pdsp::AHR : one-shot Attack-Hold-Release envelope, responsive only to positive triggers
- pdsp::Amp : multiplies an input signal for a mod signal

## LFO / Modulators
- pdsp::LFO : Low Frequency Oscillator
- pdsp::ClockedLFO : LFO synchronized to the clock tempo
- pdsp::TriggeredRandom : Random value changed by a trigger input
- pdsp::SampleAndHold : Hold the signal when it receives a trigger 

## Samplers
- pdsp::Sampler : one-shot sampler
- pdsp::GrainCloud : classic granular sampler

## Signal / Effects
- pdsp::Bitcruncher : reduces the bit for the signal amplitude
- pdsp::Decimator : reduces the signal sampling frequency
- pdsp::Saturator1 : overdrive 
- pdsp::DimensionChorus : lush stereo chorus

## Equalizers
- pdsp::LowCut : cuts low frequencies
- pdsp::LowShelfEQ : low shelf equalizer
- pdsp::PeakEQ : peak equalizer
- pdsp::HighShelfEQ : high shelf equalizer
- pdsp::HighCut : cuts high frequencies

## Reverbs  
- pdsp::BasiVerb : dubby metallic reverb with mono input and stereo output 
- pdsp::IRVerb : a impulse response based reverb, with mono/stereo inputs and outputs

## Routing
- pdsp::LinearCrossfader : crossfade between two signals
- pdsp::Panner : pans the signal from the left to the right
- pdsp::Switch : switches between different inputs

## Utility
- pdsp::BipolarToUnipolar : convert a bipolar signal to an unipolar one
- pdsp::OneBarTimeMs : outputs the time in ms for one musical bar
- pdsp::PitchToFreq : converts pitch values to frequency values, for musical control of oscillator and filters
- pdsp::FreqToMs : convert frequency values to millisecond values 
- pdsp::Formula : virtual class to extend for executing a matematical operation on an input

