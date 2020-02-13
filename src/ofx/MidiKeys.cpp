
#include "MidiKeys.h"

#include "ofxPDSPFunctions.h"

#ifndef __ANDROID__

#define OFXPDSP_MIDIKEYSPROCESSOR_MESSAGERESERVE 128


pdsp::midi::Keys::Keys(){
   
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

void pdsp::midi::Keys::setMidiChannel( int channel ){
    midiConverter.setMidiChannel( channel );
}

void pdsp::midi::Keys::setUnisonDetune(float pitchDetune){
    pitchDetune >> unisonDetuneAmt;
}

void pdsp::midi::Keys::processMidi(const pdsp::midi::Input &midiInProcessor, const int &bufferSize ) noexcept{
    midiConverter.processMidi(midiInProcessor.getMessageVector() , bufferSize);
}


void pdsp::midi::Keys::setSlew(float slewTimeMs){
    this->slewTime = slewTimeMs;

    out_pressure.setSlewRateModeReference(1.0f);
    out_pressure.setSlewTime(this->slewTime, pdsp::Rate);
    
    out_pitchbend.setSlewTime(slewTimeMs, pdsp::Rate);
    
}


void pdsp::midi::Keys::setPitchBend( float down, float up){
    if(down!=up){
        midiConverter.setPitchBend(down, up);
        float range = fabs(up - down);
        out_pitchbend.setSlewRateModeReference(range);
        out_pitchbend.setSlewTime(this->slewTime, pdsp::Rate);        
    }
}


int pdsp::midi::Keys::getVoicesNumber() const {
        return maxVoices;
}

void pdsp::midi::Keys::setNoteRange(int lowNote, int highNote){
    midiConverter.setNoteRange( lowNote, highNote );
    
}

void pdsp::midi::Keys::setPolyMode(int maxNotes, int unisonVoices ){
      
        this->maxNotes = maxNotes;
        
        for( auto & gate : gates ){
                gate.unLink();
        }
        
        for( auto & value : values ){
                value.unLink();
        }
        
        out_singletrigger.unLink();

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


void pdsp::midi::Keys::setMonoMode(int unisonVoices, bool legato, MonoPriority priority){

        for( auto & gate : gates ){
                gate.unLink();
        }
        
        for( auto & value : values ){
                value.unLink();
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

void pdsp::midi::Keys::setPortamento(PortamentoMode portamentoMode, float timeMs, pdsp::SlewMode_t slewMode, float scaling){
        this->portamentoMode = portamentoMode;
        this->portamentoTime = timeMs;
        this->slewMode = slewMode;
        
        for(int i=0; i<maxNotes; ++i){
                values[i].setSlewTime(timeMs, slewMode);
                values[i].setSlewRateModeReference(scaling);
        }
        midiConverter.setPortamento(portamentoMode);
}

pdsp::PatchNode & pdsp::midi::Keys::out_trig( int voice ) {
    return outs_trig[voice];
}

pdsp::PatchNode &  pdsp::midi::Keys::out_pitch( int voice ) {
    return outs_pitch[voice]; 
}

pdsp::SequencerGateOutput & pdsp::midi::Keys::out_single_trigger() {
    return out_singletrigger;
}

pdsp::SequencerValueOutput & pdsp::midi::Keys::out_pitch_bend() {
    return out_pitchbend;
}
  
pdsp::SequencerValueOutput  & pdsp::midi::Keys::out_aftertouch() {
    return out_pressure;
}

void pdsp::midi::Keys::setTuning( int i, float value ){
    if( i >= 0 && i <128 ){
        midiConverter.tuning[i] = f2p( value );
    }else{
        std::cout<<"[pdsp] warning! setTuning index outside range, returning 0.0f\n";
        pdsp_trace();
    }
}
void pdsp::midi::Keys::setTuningByPitch( int i, float value ){
    if( i >= 0 && i <128 ){
        midiConverter.tuning[i] = value;
    }else{
        std::cout<<"[pdsp] warning! setTuningByPitch index outside range, returning 0.0f\n";
        pdsp_trace();
    }
}

float pdsp::midi::Keys::getTuning( int i ) const {
    if( i >= 0 && i <128 ){
        return p2f( midiConverter.tuning[i] );
    }else{
        std::cout<<"[pdsp] warning! getTuning index outside range, returning 0.0f\n";
        pdsp_trace();
    }
    return 0.0f;
}

#endif
