
// MidiKeysBuffers.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef OFXPDSPMIDI_MIDIKEYSBUFFERS_H_INCLUDED
#define OFXPDSPMIDI_MIDIKEYSBUFFERS_H_INCLUDED

#ifndef __ANDROID__
#include "ofxMidi.h"
#include "PositionedMidiMessage.h"
#endif

#include "../messages/header.h"

namespace pdsp{ 

enum VoiceMode {Poly, Mono};
enum PortamentoMode {Off, On, Legato};
enum MonoPriority {Last, Low, High};

namespace helper {

class NoteState_t {
public:
        NoteState_t() : gate(false), note(-1), eventNumber(0){};
        
        float gate;
        int note;
        int eventNumber;

};

}} // end namespaces

#ifndef __ANDROID__

namespace pdsp{  namespace helper {
    
class MidiKeysBuffers {
public:
        MidiKeysBuffers();
           
        void processMidi (const std::vector<_PositionedMidiMessage> & readVector, const int &bufferSize ) noexcept;
        
        void setVoiceMode(VoiceMode mode);
        void setMonoPriority(MonoPriority mode);
        void setPortamento( PortamentoMode mode );
        void setMaxNotes(int notes);
        void setPitchBend( float down, float up);
        void setNoteRange( int lowNote, int highNote );
        void setMidiChannel( int channel );
        
        
        std::vector<pdsp::MessageBuffer>    gateMessages;
        std::vector<pdsp::MessageBuffer>    pitchMessages;
        std::vector<pdsp::MessageBuffer>    portaMessages;
        pdsp::MessageBuffer                 singleGateMessages;
        
        pdsp::MessageBuffer             pitchBendMessages;
        pdsp::MessageBuffer             pressureMessages;

private:

        void processPolyMidiNoteOn( const _PositionedMidiMessage& midi) noexcept;
        void processPolyMidiNoteOff( const _PositionedMidiMessage& midi) noexcept;
        void processMonoMidiNoteOn( const _PositionedMidiMessage& midi) noexcept;
        void processMonoMidiNoteOff( const _PositionedMidiMessage& midi) noexcept;
        
        int noteSteal();
        int getHighestPriorityMono();

        void checkIdGeneration();
        void clearNotes();
        

        bool    sendClearMessages;
        int     eventIdGenerator;
        int     activeNotes;
        int     maxNotes;
        int     monoNoteIndex;
        int     channel;
        
        int lowNote; 
        int highNote;
        
        std::vector<NoteState_t>             notes;
        
        float   pitchBendUpAmount;
        float   pitchBendDownAmount;    
           
  
        VoiceMode               voiceMode;
        MonoPriority            monoMode;
        PortamentoMode          portamentoMode;
};       
    
    
}} // end namespaces

#endif

#endif // OFXPDSPMIDI_MIDIKEYSBUFFERS_H_INCLUDED
