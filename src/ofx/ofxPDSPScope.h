
// ofxPDSPScope.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef OFXPDSP_SCOPE_H_INCLUDED
#define OFXPDSP_SCOPE_H_INCLUDED

#include "../DSP/pdspCore.h"
#include "ofMain.h"


/*!
@brief Utility class to monitor the signal 
*/
class ofxPDSPScope : public pdsp::Unit{
    
public:
    
    ofxPDSPScope();
    
    ~ofxPDSPScope();
    ofxPDSPScope(const ofxPDSPScope& other);
    ofxPDSPScope& operator=(const ofxPDSPScope& other);
                
    /*!
    @brief set the number of samples to be reported by the scope. This number has to be greter than the bufferSize, otherwise the buffersize will be used. High value let you see more of the general envelope, low value are closer to the waveform.
    @param[in] samplesLength samples to monitor
    */
    Patchable& set( int samplesLength);
    
    /*!
    @brief Sets "signal" as selected input and returns this Unit ready to be patched. This is the default input. This input is the passing signal to monitor.
    */   
    Patchable& in_signal();
    

    /*!
    @brief Sets "signal" as selected output and returns this Unit ready to be patched. This is the default output. To monitor the signal this output has to be connected to something, the engine blackhole() input is a good choice. This output is always silent!
    */ 
    Patchable& out_signal();
   
    /*!
    @brief draws the scope at the given coordinates
    @param[in] x
    @param[in] y
    @param[in] w width
    @param[in] h height
    */     
    void draw( int x, int y, int w, int h);
    
private:

    void prepareUnit ( int expectedBufferSize, double sampleRate ) override;
    
    void releaseResources () override;

    void process (int bufferSize) noexcept override;
    
    pdsp::OutputNode output;

    pdsp::InputNode input_signal;
    
    vector<float> buffer;
    
    int bufferLen;
    int bufferPos;
    
};    
        



#endif  // OFXPDSP_SCOPE_H_INCLUDED
