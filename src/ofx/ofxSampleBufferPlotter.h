
// ofxSampleBufferPlotter.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef OFXPDSP_SAMPLEBUFFER_PLOTTER_H_INCLUDED
#define OFXPDSP_SAMPLEBUFFER_PLOTTER_H_INCLUDED

#include "ofMain.h"
#include "../DSP/samplers/SampleBuffer.h"

class ofxSampleBufferPlotter{
 
public:    
    ofxSampleBufferPlotter();
    
    void setWaveform(pdsp::SampleBuffer& sampleBuffer, int channel, ofColor color, int width, int height);
    void draw(int x, int y);
    const ofFbo& getFbo();

private:
    ofFbo   waveformCanvas;
    bool    fboFilled;
    int     width;
    int     height;

};


#endif // OFXPDSP_SAMPLEBUFFER_PLOTTER_H_INCLUDED
