
// ofxPDSPMidiPads
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef OFXPDSP_PDSPMIDIPADS_H_INCLUDED
#define OFXPDSP_PDSPMIDIPADS_H_INCLUDED

#include "ofxMidi.h"
#include "../messages/header.h"
#include "../DSP/control/GateSequencer.h"
#include "ofxPositionedMidiMessage.h"
#include "ofxPDSPController.h"
#include "ofxPDSPMidiIn.h"

/*!
@brief utility class to parse midi note data from a ofxPDSPMidiIn and convert it to a bank trigger outputs
*/
class ofxPDSPMidiPads  : public ofxPDSPController {
    
public:
    ofxPDSPMidiPads();
 
    /*!
    @brief adds a new trigger layer. This will parse the notes from noteLow o noteHigh (both included) and convert the midi message to a pdsp trigger output
    @param[in] noteLow low note for parsing, included
    @param[in] noteLow high note for parsing, included
    */   
    void addTriggerLayer(int noteLow, int noteHigh);
    
    /*!
    @brief sets a trigger layer. This layer will parse the notes from noteLow o noteHigh (both included) and convert the midi message to a pdsp trigger output
    @param[in] layerIndex index of the layer to set
    @param[in] noteLow low note for parsing, included
    @param[in] noteLow high note for parsing, included
    */   
    void setTriggerLayer(int layerIndex, int noteLow, int noteHigh);
    
    /*!
    @brief resizes the trigger layers number
    @param[in] size new size
    */      
    void resizeLayers(int size);

    /*!
    @brief inits the class setting its layer starting from low note, setting span notes for each layer, with numNotes layer. The layers' note don't overlap.
    @param[in] lowNote the lowest note of the first layer
    @param[in] numLayer the number of layers
    @param[in] layerSpan how many notes each layer has
    */    
    void simpleInit(int lowNote, int numLayer, int layerSpan=1);
    
    /*!
    @brief return the trigger output of the given note layer.
    @param[in] layerIndex layer index
    */    
    pdsp::GateSequencer &  out_trig( int layerIndex );

    /*!
    @brief returns the low note for the given layer
    @param[in] layerIndex layer index
    */
    int getLayerLow(int layerIndex) const;
    
    /*!
    @brief returns the high note for the given layer
    @param[in] layerIndex layer index
    */
    int getLayerHigh(int layerIndex) const;

/*!
    @cond HIDDEN_SYMBOLS
*/

    /*!
    @brief this is the vector of output triggers binded to the note layers.
    */
    std::vector<pdsp::GateSequencer>     outs_trig;
    
    void processMidi(const ofxPDSPMidiIn &midiInProcessor, const int &bufferSize ) noexcept override;
/*!
    @endcond
*/   

private:
    std::vector<pdsp::MessageBuffer>     trigBuffers;       
    std::vector<int>                     lowThreshold;
    std::vector<int>                     highThreshold;

    int size;
};


#endif // OFXPDSP_PDSPMIDIPADS_H_INCLUDED
