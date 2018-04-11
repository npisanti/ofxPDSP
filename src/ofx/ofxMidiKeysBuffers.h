
// ofxMidiKeysBuffers.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef OFXPDSPMIDI_MIDIKEYSBUFFERS_H_INCLUDED
#define OFXPDSPMIDI_MIDIKEYSBUFFERS_H_INCLUDED

#ifndef __ANDROID__
#include "ofxMidi.h"
#include "ofxPositionedMidiMessage.h"
#endif

#include "../messages/header.h"

enum VoiceMode {Poly, Mono};
enum PortamentoMode {Off, On, Legato};
enum MonoPriority {Last, Low, High};

class NoteState_t {
public:
        NoteState_t() : gate(false), note(-1), eventNumber(0){};
        
        float gate;
        int note;
        int eventNumber;

};

#ifndef __ANDROID__
class ofxMidiKeysBuffers {
public:
        ofxMidiKeysBuffers();
           
        void processMidi (const std::vector<_ofxPositionedMidiMessage> & readVector, const int &bufferSize ) noexcept;
        
        void setVoiceMode(VoiceMode mode);
        void setMonoPriority(MonoPriority mode);
        void setPortamento( PortamentoMode mode );
        void setMaxNotes(int notes);
        void setPitchBend( float down, float up);
        void setNoteRange( int lowNote, int highNote );
        
        
        std::vector<pdsp::MessageBuffer>    gateMessages;
        std::vector<pdsp::MessageBuffer>    pitchMessages;
        std::vector<pdsp::MessageBuffer>    portaMessages;
        pdsp::MessageBuffer                 singleGateMessages;
        
        pdsp::MessageBuffer             pitchBendMessages;
        pdsp::MessageBuffer             pressureMessages;

private:

        void processPolyMidiNoteOn( const _ofxPositionedMidiMessage& midi) noexcept;
        void processPolyMidiNoteOff( const _ofxPositionedMidiMessage& midi) noexcept;
        void processMonoMidiNoteOn( const _ofxPositionedMidiMessage& midi) noexcept;
        void processMonoMidiNoteOff( const _ofxPositionedMidiMessage& midi) noexcept;
        
        int noteSteal();
        int getHighestPriorityMono();

        void checkIdGeneration();
        void clearNotes();
        

        bool    sendClearMessages;
        int     eventIdGenerator;
        int     activeNotes;
        int     maxNotes;
        int     monoNoteIndex;
        
        int lowNote; 
        int highNote;
        
        std::vector<NoteState_t>             notes;
        
        float   pitchBendUpAmount;
        float   pitchBendDownAmount;    
           
  
        VoiceMode               voiceMode;
        MonoPriority            monoMode;
        PortamentoMode          portamentoMode;
};       
    
#endif

#endif // OFXPDSPMIDI_MIDIKEYSBUFFERS_H_INCLUDED
