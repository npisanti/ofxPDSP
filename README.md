ofxPDSP
==============
ofxPDSP is an [openFrameworks](https://openframeworks.cc/) addon for audio synthesis and generative music. It is based on those design principles:
- Native c++, made to be tightly integrated into c++ projects.
- Class design mimics modular synthesizers, where inputs and outputs are patched with the >> operator.
- Sample accurate timing for the included sequencer.
- Thread-safe methods for metering and control.
- Lazy evaluated DSPs. DSP units are recursively processed in chunks big as the audio buffer size, DSPs switch between routines running at audio rate (process every sample) and control rate (once every buffer) on the fly when the result is the same. DSPs are also coded using SIMD instructions when possible.
- Cross-platform code: works on Linux (x86/ARM), OSX, Windows, iOS, Android (only on ARM processors with NEON instructions).

It includes [a good selection of modules ready to be used](http://npisanti.com/ofxPDSP/md__modules.html), like antialiasied oscillators and VA filters, an FFT convolver, analog-sounding envelopes, a delay, a noise generator, a sampler and a graincloud module, biquad equalizers (normal and 2x oversampled), modules for dynamic processing, units for oversampling/downsampling and a framework for writing and sequencing generative music.
On top of that some other classes that uses openFrameworks for adding midi and OSC in/out, serial output, sequencer monitoring, waveform plotting and scopes, other audio addon wrapping and ofParameter-compatible control.

To learn how to use ofxPDSP be sure to check out all the included examples, starting from the basics, and please also read [this reference page](http://npisanti.com/ofxPDSP/md__modules.html) for a selection of the most high level classes and function (probably you won't ever need more than that).

It is requires the latest stable release of openFrameworks (0.10) or the master release, [my fork of ofxMidi](https://github.com/npisanti/ofxMidi), [ofxSIMDFloats](https://github.com/npisanti/ofxSIMDFloats), [ofxAudioFile](https://github.com/npisanti/ofxAudioFile) and ofxOsc.
    
Check out the videos of some included examples:    
    
[![ofxPDSP example-scoring4-wolfram-dub](http://img.youtube.com/vi/0-NXEFEFiE4/0.jpg)](https://www.youtube.com/watch?v=0-NXEFEFiE4 "ofxPDSP example-scoring4-wolfram-dub")

[![ofxPDSP example-graincloud](http://img.youtube.com/vi/t04So5y3hrI/0.jpg)](https://www.youtube.com/watch?v=t04So5y3hrI "ofxPDSP example-graincloud")

[![ofxPDSP example-midi_polysynth](http://img.youtube.com/vi/LvABH2Sdsqw/0.jpg)](https://www.youtube.com/watch?v=LvABH2Sdsqw "ofxPDSP example-midi_polysynth")

[![ofxPDSP example-scoring3-masterplan](http://img.youtube.com/vi/8Rf95kmRJ-g/0.jpg)](https://www.youtube.com/watch?v=8Rf95kmRJ-g "ofxPDSP example-scoring3-masterplan")

[![ofxPDSP example-serial-arduino](http://img.youtube.com/vi/MPi8LDFv7s8/0.jpg)](https://www.youtube.com/watch?v=MPi8LDFv7s8 "ofxPDSP example-serial-arduino")

[![ofxPDSP example-data-to-wave](http://img.youtube.com/vi/oMH_W4b1z5A/0.jpg)](https://www.youtube.com/watch?v=oMH_W4b1z5A "ofxPDSP example-data-to-wave")

Nicola Pisanti, MIT License 2016-2018.   
For FFT the AudioFFT wrapper is used, Copyright (c) 2013 [HiFi-LoFi](https://github.com/HiFi-LoFi).  
For loading audio file on OSX [libaudiodecoder](https://github.com/asantoni/libaudiodecoder) was used, it is still included for legacy purposes but it will be removed in one of the next versions. Copyright (c) 2010-2012 Albert Santoni, Bill Good, RJ Ryan.
