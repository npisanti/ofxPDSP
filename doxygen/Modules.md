
This is an overview of a selection of the available modules. There are many classes in pdsp, but many are just for very advanced users as they are used to build higher level classes. This is a selection of the most higher level pdsp classes, probably you will never need more than this. Some of the modules can work both in mono or stereo, usually when one of the outputs is not patched that channel is not processed and the module works in mono.

## Oscillators / Noise
- pdsp::VAOscillator : band limited oscillator with saw, pulse, triangle and sine outputs
- pdsp::TableOscillator : wavetable oscillator, check the code in the included examples
- pdsp::DataOscillator : wavetable oscillator with thread-safe setting of waveform in realtime
- pdsp::FMOperator : sine oscillator with pm input for FM synthesis
- pdsp::WhiteNoise : noise generator
- pdsp::BitNoise : digital noise generator with lots of shaping control and stereo outputs

## Filters
- pdsp::VAFilter : stereo/mono resonant 4 pole ladder filter with switchable 12db/24db LP, BP and HP outputs and pitched cutoff control
- pdsp::CombFilter : Comb Filter with frequency control in pitch, feedback and damping control (this is basically a tuned delay)

## Delays
- pdsp::Delay : delay with feedback damping

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
- pdsp::Saturator2 : warmer overdrive 
- pdsp::DimensionChorus : lush stereo chorus
- pdsp::Ducker : pushes down the signal when a trigger is received, useful for side-chaining

## Equalizers
- pdsp::LowCut : cuts low frequencies
- pdsp::LowShelfEQ : low shelf equalizer band
- pdsp::PeakEQ : peak equalizer band
- pdsp::HighShelfEQ : high shelf equalizer band
- pdsp::HighCut : cuts high frequencies
- pdsp::AALowShelfEQ : low shelf equalizer band, 2x oversampled
- pdsp::AAPeakEQ : peak equalizer band, 2x oversampled
- pdsp::AAHighShelfEQ : high shelf equalizer band, 2x oversampled

## Reverbs  
- pdsp::BasiVerb : dubby metallic reverb with mono input and stereo output 
- pdsp::IRVerb : a impulse response based reverb, with mono/stereo inputs and outputs

## Routing
- pdsp::PatchNode : just sums the inputs (or just pass it to output if there is only one)
- pdsp::LinearCrossfader : crossfade between two signals
- pdsp::Panner : pans the signal from the left to the right
- pdsp::Switch : switches between different inputs

## Sequencing ( you access those classes from `engine.sequencer` )
- pdsp::SequencerProcessor : engine.score is an instance of this class, manage all the sections
- pdsp::SequencerSection : play one pdsp::Sequence at time and has multiple output to patch to the DSP modules
- pdsp::Sequence : a fixed or generative sequence to send values to one or more outputs
- pdsp::SeqChange : the behavior of a pdsp::Sequence after it ends
- pdsp::Behavior : contains standard behaviors, you mostly use pdsp::Behavior::Loop or pdsp::Behavior::OneShot

## Thread-Safe Control / ofParameter compatible
- pdsp::Parameter : holds a value to control the modules (with optional smoothing), has ofParameter integration for GUIs
- pdsp::TriggerControl : send triggers
- pdsp::ParameterAmp : a combination of pdsp::Amp and pdsp::Parameter for easy mod amount control
- pdsp::ParameterGain : a combination of pdsp::Amp and pdsp::Parameter for easy gain control, with stereo in/out

## Audio Interface
- pdsp::Engine : manages audio inputs / outputs and runs the DSPs

## Midi
- pdsp::midi::Input : for using midi input
- pdsp::midi::Output : sequencer to midi output
- pdsp::midi::Keys : for using midi keyboards
- pdsp::midi::Pads : for using midi notes as triggers 
- pdsp::midi::Controls : midi control change input

## OSC
- pdsp::osc::Input : OSC input
- pdsp::osc::Output : OSC output

## Utility
- pdsp::Scope : show running buffers
- pdsp::SampleBufferPlotter : draws pdsp::SampleBuffer waveforms 

    
    
    
## PDSP Functions 
ofxPDSP also has a selection of functions for various tasks. For using random inside ofxPDSP you have use its random functions as `ofRandom` isn't thread safe:
   
        int pdsp::dice( int size ); // returns a number from 0 to size-1
        int pdsp::dice( int min, int maxExclusive ); // returns a number from min to max-1
        bool pdsp::chance( float probability ); // probability range is 0.0f-1.0f, returns true or false
        float pdsp::urand(); // returns a number from 0.0f to 1.0f, 1.0f is included
        float pdsp::brand(); // returns a number from -1.0f to 1.0f, -1.0f and 1.0f are included
   
other useful functions are:
   
        float dB( float value ); // returns the linear equivalent of the given deciBel value
        float pdsp::panL( float pan ); // multiply for this function to pan the left channel 
        float pdsp::panR( float pan ); // multiply for this function to pan the right channel 
        float spread(int index, int max, float spread, bool fromCenter); // return pans value for a vector of voices
        float pdsp::f2p( float freq ); // frequency to pitch
        float pdsp::p2f( float pitch ); // pitch to frequency
        float pdsp::pratio( float pitch, float ratio ); // pitch/ratio, multiply the frequency of the given pitch for ration, and then converts it back to pitch value again (useful for FM synths)
        float pdsp::highestPartial( float pitch ); // take a pitch value as input an returns the highest partial before 20000hz of a waweform with that base pitch, useful for wavetables
