
#include "ofxPDSPMidiKeys.h"

#ifndef __ANDROID__

#define OFXPDSP_MIDIKEYSPROCESSOR_MESSAGERESERVE 128


ofxPDSPMidiKeys::ofxPDSPMidiKeys(){
   
    gates.reserve(16);
    values.reserve(16);
    outs_trig.reserve(32);
    outs_pitch.reserve(32);
    unisonPitchDetune.reserve(32);
    voiceUnisonMod.reserve(32);
    out_singletrigger.setSingleTrigger(true);
    setUnisonDetune(0.3f);

    portamentoMode = Off;
    portamentoTime = 200.0f;
    slewMode = pdsp::Time;

    setSlew(40.0f);
    setPitchBend(-2, 2);

    setPolyMode(8, 1);
}

void ofxPDSPMidiKeys::setMidiChannel( int channel ){
    midiConverter.setMidiChannel( channel );
}

void ofxPDSPMidiKeys::setUnisonDetune(float pitchDetune){
    pitchDetune >> unisonDetuneAmt;
}

void ofxPDSPMidiKeys::processMidi(const ofxPDSPMidiIn &midiInProcessor, const int &bufferSize ) noexcept{
    midiConverter.processMidi(midiInProcessor.getMessageVector() , bufferSize);
}


void ofxPDSPMidiKeys::setSlew(float slewTimeMs){
    this->slewTime = slewTimeMs;

    out_pressure.setSlewRateModeReference(1.0f);
    out_pressure.setSlewTime(this->slewTime, pdsp::Rate);
    
    out_pitchbend.setSlewTime(slewTimeMs, pdsp::Rate);
    
}


void ofxPDSPMidiKeys::setPitchBend( float down, float up){
    if(down!=up){
        midiConverter.setPitchBend(down, up);
        float range = fabs(up - down);
        out_pitchbend.setSlewRateModeReference(range);
        out_pitchbend.setSlewTime(this->slewTime, pdsp::Rate);        
    }
}


int ofxPDSPMidiKeys::getVoicesNumber() const {
        return maxVoices;
}

void ofxPDSPMidiKeys::setNoteRange(int lowNote, int highNote){
    midiConverter.setNoteRange( lowNote, highNote );
    
}

void ofxPDSPMidiKeys::setPolyMode(int maxNotes, int unisonVoices ){
        
        for(int i=0; i<this->maxNotes; ++i){
                gates[i].unLink();
                values[i].unLink();
        }
        out_singletrigger.unLink();
        this->maxNotes = maxNotes;
        
        midiConverter.setVoiceMode(Poly);
        midiConverter.setMaxNotes(maxNotes);
        
        gates.resize(maxNotes);
        values.resize(maxNotes);


        midiConverter.pitchBendMessages >> out_pitchbend;
        
        
        for(int i=0; i<maxNotes; ++i){
                midiConverter.gateMessages[i]  >> gates[i];
                midiConverter.pitchMessages[i] >> values[i].in_message();
                midiConverter.portaMessages[i] >> values[i].in_slew();
        }
        midiConverter.singleGateMessages >> out_singletrigger;
        
        
        maxVoices = maxNotes * unisonVoices;
        
        outs_trig.resize(maxVoices);
        outs_pitch.resize(maxVoices);
        unisonPitchDetune.resize(maxVoices);
        voiceUnisonMod.resize(maxVoices);
        
        for(int i=0; i<maxVoices; ++i){
                outs_trig[i].disconnectAll();
                outs_pitch[i].disconnectAll();
        }
        
        
        if(unisonVoices<=1){
                for(int i=0; i<maxNotes; ++i){
                        gates[i]        >>  outs_trig[i];
                        values[i]       >>  outs_pitch[i];
                        out_pitchbend   >>  outs_pitch[i];
                }
        }else{
                for (int k = 0; k<unisonVoices;k++){ //k is the unison index
                    for(int i=0; i<maxNotes; ++i){

                        gates[i]    >>  outs_trig[i + maxNotes*k ];

                        values[i]       >>  outs_pitch[i + maxNotes*k ];
                        out_pitchbend   >>  outs_pitch[i + maxNotes*k ];

                        unisonPitchDetune[i + maxNotes*k ] >> outs_pitch[i + maxNotes*k ];

                                            unisonPitchDetune[i + maxNotes*k ].disconnectAll();
                        unisonDetuneAmt >>  unisonPitchDetune[i + maxNotes*k ].in_mod();
                        voiceUnisonMod[i + maxNotes*k ] >> unisonPitchDetune[i + maxNotes*k ].in_signal();

                        //those are the right unison amounts
                        voiceUnisonMod[i + maxNotes*k].set(static_cast<float>(k) - ( static_cast<float>(unisonVoices)-1.0f)*0.5f);
                    }
                }
        }        
        
        setPortamento(portamentoMode, portamentoTime, slewMode);
}


void ofxPDSPMidiKeys::setMonoMode(int unisonVoices, bool legato, MonoPriority priority){

        for(int i=0; i<this->maxNotes; ++i){
                gates[i].unLink();
                values[i].unLink();
        }
        out_singletrigger.unLink();
        
        midiConverter.setVoiceMode(Mono);
        midiConverter.setMonoPriority(priority);
        
        this->maxNotes = 10; //ten fingers!
        midiConverter.setMaxNotes(maxNotes);      
        
        gates.resize(1);
        values.resize(1);
        
        gates[0].setSingleTrigger(legato);
   
        maxVoices = unisonVoices;
        
        outs_trig.resize(maxVoices);
        outs_pitch.resize(maxVoices);
        unisonPitchDetune.resize(maxVoices);
        voiceUnisonMod.resize(maxVoices);
        
        for(int i=0; i<maxVoices; ++i){
                outs_trig[i].disconnectAll();
                outs_pitch[i].disconnectAll();
        }
        
        midiConverter.pitchBendMessages >> out_pitchbend;
        
        midiConverter.gateMessages[0]  >> gates[0];
        midiConverter.pitchMessages[0] >> values[0].in_message();
        midiConverter.portaMessages[0] >> values[0].in_slew();
       
        if(unisonVoices<=1){
                                    outs_trig[0].disconnectIn();
                gates[0]        >>  outs_trig[0];
                
                                    outs_pitch[0].disconnectIn();
                values[0]       >>  outs_pitch[0]; 
                out_pitchbend   >>  outs_pitch[0];
                
        }else{
                for(int k = 0; k<unisonVoices; k++){
                        outs_trig[k].disconnectIn();
                        gates[0]    >> outs_trig[k];

                        outs_pitch[k].disconnectIn();
                        values[0]      >> outs_pitch[k]; 
                        out_pitchbend  >>  outs_pitch[k];
                        
                        unisonPitchDetune[k] >> outs_pitch[k]; 

                                             unisonPitchDetune[k].disconnectAll();
                        unisonDetuneAmt >>   unisonPitchDetune[k].in_mod();
                        voiceUnisonMod[k] >> unisonPitchDetune[k].in_signal();

                        //those are the right unison amounts
                        voiceUnisonMod[k].set(static_cast<float>(k) - ( static_cast<float>(unisonVoices)-1.0f)*0.5f);
                        // unison mod = k - (unisonVoices-1)*0.5
                }
        }
        
        setPortamento(portamentoMode, portamentoTime, slewMode);
        
}

void ofxPDSPMidiKeys::setPortamento(PortamentoMode portamentoMode, float timeMs, pdsp::SlewMode_t slewMode, float scaling){
        this->portamentoMode = portamentoMode;
        this->portamentoTime = timeMs;
        this->slewMode = slewMode;
        
        for(int i=0; i<maxNotes; ++i){
                values[i].setSlewTime(timeMs, slewMode);
                values[i].setSlewRateModeReference(scaling);
        }
        midiConverter.setPortamento(portamentoMode);
}

pdsp::PatchNode & ofxPDSPMidiKeys::out_trig( int voice ) {
    return outs_trig[voice];
}

pdsp::PatchNode &  ofxPDSPMidiKeys::out_pitch( int voice ) {
    return outs_pitch[voice]; 
}

pdsp::GateSequencer & ofxPDSPMidiKeys::out_single_trigger() {
    return out_singletrigger;
}

pdsp::ValueSequencer & ofxPDSPMidiKeys::out_pitch_bend() {
    return out_pitchbend;
}
  
pdsp::ValueSequencer  & ofxPDSPMidiKeys::out_aftertouch() {
    return out_pressure;
}

#endif
