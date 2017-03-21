
// ofxPDSPMidiControls.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef OFXPDSPMIDI_PDSPMIDICONTROLS_H_INCLUDED
#define OFXPDSPMIDI_PDSPMIDICONTROLS_H_INCLUDED

#include "ofxMidi.h"
#include "../DSP/control/ValueSequencer.h"
#include <chrono>
#include "ofxPositionedMidiMessage.h"
#include "ofxMidiCCBuffers.h"
#include "ofxPDSPController.h"
#include "ofxPDSPMidiIn.h"

/*!
@brief utility class to parse control change data from a ofxPDSPMidiIn and convert it to a bank of patchable modules with optional slewed output. By default outputs are slewed to 50ms. 
*/
class ofxPDSPMidiControls : public ofxPDSPController {
    
public:
    ofxPDSPMidiControls();
    ofxPDSPMidiControls(int maxCC);
    /*!
    @brief sets the slew times of the CCs
    @param[in] slewTimeMs new slew time in milliseconds
    */
    void setCCSlew(float slewTimeMs);

    /*!
    @brief sets the max CC number parsed, and resizes the out_cc vector
    @param[in] slewTimeMs new slew time in milliseconds
    */
    void setMaxCCNum(int ccNum);


    /*!
    @brief returns the number of the max returned CC
    @param[in] slewTimeMs new slew time in milliseconds
    */
    int  getMaxCCNum();

    /*!
    @brief returns a control value corrisponding to the given cc number, the output is mapped to the 0.0f-1.0f range.
    @param[in] cc the cc number for the output
    */    
    pdsp::ValueSequencer & out( int cc );
    
/*!
    @cond HIDDEN_SYMBOLS
*/
    vector<pdsp::ValueSequencer>    outs_cc; // this has to become private in later versions
    
    void processMidi(const ofxPDSPMidiIn &midiInProcessor, const int &bufferSize ) noexcept override;   
/*!
    @endcond 
*/
private: 
    float               slewTime;
    ofxMidiCCBuffers    midiCC;
        
};


#endif //OFXPDSPMIDI_PDSPMIDICONTROLS_H_INCLUDED
