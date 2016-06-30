ofxPDSP
==============
ofxPDSP is an audio DSP addon/library for openFrameworks. It is based on those design principles:
- Native c++, made to be tightly integrated into c++ projects.
- Class design mimics modular synthesizers, where inputs and outputs are patched with the >> operator.
- Sample accurate timing.
- Lazy evaluated DSPs. DSP units are recursively processed in chunks big as the audio buffer size, CPU cycles are spared by not processing Units if the actual state of the system make their results irrelevant and by switching between routines running at audio rate (process every sample) and control rate (once every buffer) on the fly. PDSP is coded optimizing the instruction for this kind of branching and shows a low I1 cache miss rate (e.g. less than 0.3% on a i7 processor with 128 samples of buffersize, measured with valgrind). DSPs are also coded using SIMD instructions when possible.
- Thread-safe methods for metering and control.
- Cross-platform code: works on Linux, OSX, Windows, iOS, Android (only on ARM processors with NEON instructions).

It includes a good selection of modules ready to be used, like antialiasied oscillators and VA filters, an FFT convolver, analog-sounding envelopes, delays, noise generators, a sampler and a graincloud module, biquad equalizers (normal and 2x oversampled), units for dynamic processing, units for oversampling/downsampling and a framework for writing and sequencing generative music.
You can find a doxygenerated [documentation of the modules here](http://npisanti.com/ofxPDSP/index.html) and a [series of tutorials here](https://github.com/npisanti/ofxPDSP/wiki).

On top of that some other classes that uses openFrameworks for adding midi in/out, waveform plotting and manage GUIs.

Requires [ofxMidi](https://github.com/danomatika/ofxMidi) and [ofxSIMDFloats.](https://github.com/npisanti/ofxSIMDFloats)


Check out the videos of some included examples:

[![ofxPDSP example-midi_polysynth](http://img.youtube.com/vi/LvABH2Sdsqw/0.jpg)](https://www.youtube.com/watch?v=LvABH2Sdsqw "ofxPDSP example-midi_polysynth")

[![ofxPDSP example-graincloud](http://img.youtube.com/vi/t04So5y3hrI/0.jpg)](https://www.youtube.com/watch?v=t04So5y3hrI "ofxPDSP example-graincloud")

[![ofxPDSP example-wolfram_beats](http://img.youtube.com/vi/Dzli9RytUyY/0.jpg)](https://www.youtube.com/watch?v=Dzli9RytUyY "ofxPDSP example-wolfram_beats")

[![ofxPDSP example-serial-arduino](http://img.youtube.com/vi/MPi8LDFv7s8/0.jpg)](https://www.youtube.com/watch?v=MPi8LDFv7s8 "ofxPDSP example-serial-arduino")


Nicola Pisanti, MIT License 2016.  
For FFT the AudioFFT wrapper is used, Copyright (c) 2013 [HiFi-LoFi](https://github.com/HiFi-LoFi).  
For loading audio file on OSX [libaudiodecoder](https://github.com/asantoni/libaudiodecoder) is used, Copyright (c) 2010-2012 Albert Santoni, Bill Good, RJ Ryan.
