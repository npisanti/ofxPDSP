
// ofxPDSPAddonBridge.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef OFXPDSP_ADDONBRIDGE_H_INCLUDED
#define OFXPDSP_ADDONBRIDGE_H_INCLUDED

//#include "ofMain.h"
#include "../DSP/pdspCore.h"

/*!
@brief abstract class to extend for integrate openFrameworks addon into ofxPDSP
*/
class ofxPDSPAddonBridge : public pdsp::Unit {
    
public:    
    ofxPDSPAddonBridge();
    
    void setChannels ( int inputs, int outputs );
    
    pdsp::InputNode & in( int num );
    pdsp::OutputNode & out( int num );
    
    pdsp::OutputNode & out_silent();
    
protected:
    virtual void audioProcess(float *input, int bufferSize, int nChannels) = 0;

private:
    void process(int bufferSize) noexcept override;

    void prepareUnit( int expectedBufferSize, double sampleRate ) override;
    void releaseResources() override;
    
    int bufferLength;
    int maxChannels;
    
    float* buffer;
    
    std::vector<pdsp::InputNode> inputs;
    std::vector<pdsp::OutputNode> outputs;
    
    pdsp::OutputNode dummyOut;
    
};







#endif // OFXPDSP_ADDONBRIDGE_H_INCLUDED
