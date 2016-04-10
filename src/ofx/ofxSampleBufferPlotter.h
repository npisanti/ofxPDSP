
// ofxSampleBufferPlotter.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef OFXPDSP_SAMPLEBUFFER_PLOTTER_H_INCLUDED
#define OFXPDSP_SAMPLEBUFFER_PLOTTER_H_INCLUDED

#include "ofMain.h"
#include "../DSP/samplers/SampleBuffer.h"

/*!
@brief utility class to generate a waveform plot
*/
class ofxSampleBufferPlotter{
 
public:    
    ofxSampleBufferPlotter();

    /*!
    @brief generates the waveform grafic for the given buffer.
    @param[in] sampleBuffer your sample buffer to plot
    @param[in] channel the sample buffer channel to plot
    @param[in] color color for plotting the graphic
    @param[in] width width of the plot
    @param[in] height height of the plot
    */    
    void setWaveform(pdsp::SampleBuffer& sampleBuffer, int channel, ofColor color, int width, int height);

    /*!
    @brief draws the waveform to the given coordinates
    #param[in] x x coordinate
    #param[in] y y coordinate
    */ 
    void draw(int x, int y);

    /*!
    @brief returns a read-only reference to the internally stored ofFbo
    */
    const ofFbo& getFbo();

private:
    ofFbo   waveformCanvas;
    bool    fboFilled;
    int     width;
    int     height;

};


#endif // OFXPDSP_SAMPLEBUFFER_PLOTTER_H_INCLUDED
