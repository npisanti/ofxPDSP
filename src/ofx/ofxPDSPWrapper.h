
// ofxPDSPWrapper.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef OFXPDSP_ADDONWRAPPER_H_INCLUDED
#define OFXPDSP_ADDONWRAPPER_H_INCLUDED

#include "../DSP/pdspCore.h"
#include "ofMain.h"
/*!
@brief abstract class to extend for integrate openFrameworks addon into ofxPDSP
*/
class ofxPDSPWrapper : public pdsp::Unit {
    
public:    
    ofxPDSPWrapper();
    
    /*!
    @brief sets the number of inputs and output channels of the wrapper. Mandatory to call
    @param inputs number of inputs
    @param outputs number of outputs
    */
    void setChannels ( int inputs, int outputs );
  
    /*!
    @brief returns the input at the given index, ready to be patched
    @param num index of the input
    */
    pdsp::InputNode & in( int num );

    /*!
    @brief returns the output at the given index, ready to be patched
    @param num index of the output
    */
    pdsp::OutputNode & out( int num );

    /*!
    @brief returns a dummy output to be patched to the engine.blackhole() 
    */
    pdsp::OutputNode & out_silent();
    
protected:

    /*!
    @brief this is the function you have to overload for processing your data. You can analyze he input values, or if you want to make your DSP you can change them or set them
    @param input array of interlaced values
    @param size of the buffer, input array lenght is bufferSIze*nChannels
    @param the max number between inputs and outputs
    */
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


#endif // OFXPDSP_ADDONWRAPPER_H_INCLUDED
