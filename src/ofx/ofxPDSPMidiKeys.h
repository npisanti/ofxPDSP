
// ofxPDSPMidiKeys.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef OFXPDSPMIDI_PDSPMIDIKEYS_H_INCLUDED
#define OFXPDSPMIDI_PDSPMIDIKEYS_H_INCLUDED

#include "ofxMidi.h"
#include "../DSP/pdspCore.h"
#include "../DSP/control/GateSequencer.h"
#include "../DSP/control/ValueSequencer.h"
#include "ofxPositionedMidiMessage.h"
#include "ofxMidiKeysBuffers.h"
#include "ofxPDSPController.h"
#include "ofxPDSPMidiIn.h"

/*!
@brief utility class to parse note, pitch bend and channel pressure from a ofxPDSPMidiIn and convert it to a bank of patchable triggers and values
*/
class ofxPDSPMidiKeys : public ofxPDSPController  {
    
public:
    ofxPDSPMidiKeys();

    /*!
    @brief set the mode to Poly, with the given max notes and the given unison voices
    @param[in] maxNotes max note for the polyphony, 4 is the minimum
    @param[in] unisonVoices number of unison voices, 1 if not given
    
    Note stealing will steal the oldest note that are not the highest or the lowest playing notes
    */      
    void setPolyMode(int maxNotes, int unisonVoices = 1);

    /*!
    @brief set the mode to Mono, with the given unison voices value and priority 
    @param[in] 
    @param[in] true if you want to retrigger the envelopes only when all the key are released
    @param[in] priority you can use Last, Low or High as parameter
    */  
    void setMonoMode(int unisonVoices, bool legato, MonoPriority priority = Last);

    /*!
    @brief sets the pitch bend amount for up and down bending
    @param[in] down how many semitones for bending down
    @param[in] up how many semitones for bending up
    */  
    void setPitchBend( float down, float up);

    /*!
    @brief sets the portamento parameters
    @param[in] portamentoMode Off, On or Legato
    @param[in] timeMs slew time in milliseconds
    @param[in] slewMode Time or Rate .
    @param[in] scaling useful only for slewMode=Rate, (about) how many semitones the pitch will change in the set slew time
    */  
    void setPortamento(PortamentoMode portamentoMode, float timeMs, pdsp::SlewMode_t slewMode = pdsp::Time, float scaling=12.0f);
  
    /*!
    @brief set the note rang for the message parsing, messages out of this range will be ignored
    @param[in] lowNote low note, inclusive
    @param[in] highNote high note, inclusive
    */  
    void setNoteRange(int lowNote, int highNote);
  
    /*!
    @brief sets the detune amout for the unison
    @param[in] detune detune in semitones. Default detune is 0.3f (30 cents)
    */
    void setUnisonDetune(float pitchDetune);    
  
    /*!
    @brief sets the slew time of the value outputs
    @param[in] slewTimeMs slew time in milliseconds
    */
    void setSlew(float slewTimeMs);  
    
    /*!
    @brief returns the actual voice number 
    */
    int getVoicesNumber() const;
    
    /*!
    @brief vector of trigger outputs
    */
    vector<pdsp::PatchNode>         outs_trig;

    /*!
    @brief vector for the pitch outputs
    */
    vector<pdsp::PatchNode>         outs_pitch;

    /*!
    @brief single trigger output for the Poly mode
    */
    pdsp::GateSequencer             out_singletrigger;

    /*!
    @brief pitchbend output. The pitchbend is also summed to the pitch outputs, so this is just for modulations
    */    
    pdsp::ValueSequencer            out_pitchbend;

    /*!
    @brief channel pressure output
    */    
    pdsp::ValueSequencer            out_pressure;

/*!
    @cond HIDDEN_SYMBOLS
*/ 
    void processMidi(const ofxPDSPMidiIn &midiInProcessor, const int &bufferSize ) noexcept override;
/*!
    @endcond
*/        
    
private: 

    pdsp::ValueNode                 unisonDetuneAmt;

    int                 maxNotes;
    int                 maxVoices;

    PortamentoMode      portamentoMode;
    float               portamentoTime;
    pdsp::SlewMode_t    slewMode; 
    float               slewTime;
   
    ofxMidiKeysBuffers      midiConverter;
    
    vector<pdsp::GateSequencer>      gates;
    vector<pdsp::ValueSequencer>     values;
    vector<pdsp::Amp>                unisonPitchDetune;
    vector<pdsp::ValueNode>          voiceUnisonMod;
    
        
};




#endif //OFXPDSPMIDI_PDSPMIDIKEYS_H_INCLUDED
