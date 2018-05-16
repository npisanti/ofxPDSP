
#include "ofxMidiKeysBuffers.h"

#ifndef __ANDROID__

#define MIDINOTEPROCESSORMESSAGERESERVE 32

ofxMidiKeysBuffers::ofxMidiKeysBuffers(){
    
        eventIdGenerator = 0;
        monoNoteIndex = -1;
        activeNotes = 0;
        sendClearMessages = true;
        gateMessages.reserve( MIDINOTEPROCESSORMESSAGERESERVE );
        pitchMessages.reserve( MIDINOTEPROCESSORMESSAGERESERVE );
        portaMessages.reserve( MIDINOTEPROCESSORMESSAGERESERVE );
        notes.reserve( MIDINOTEPROCESSORMESSAGERESERVE );

        pitchBendUpAmount   = 2;
        pitchBendDownAmount = 2;
        pitchBendMessages.reserve( MIDINOTEPROCESSORMESSAGERESERVE * 2 );
        pressureMessages.reserve( MIDINOTEPROCESSORMESSAGERESERVE * 2 );
            
        setNoteRange(0, 127);    
        
        channel = 0;
        
        voiceMode = Poly;
        monoMode = Last;
        portamentoMode = On;
        setMaxNotes(8);
        
}

void ofxMidiKeysBuffers::setPitchBend( float down, float up ){
        pitchBendUpAmount   =  up;
        pitchBendDownAmount = -down;        
}

void ofxMidiKeysBuffers::setVoiceMode(VoiceMode mode){
        voiceMode = mode;
        clearNotes();
}

void ofxMidiKeysBuffers::setMonoPriority(MonoPriority mode){
        monoMode = mode; 
        clearNotes();
}

void ofxMidiKeysBuffers::setPortamento( PortamentoMode mode ) {
        portamentoMode = mode;
}

void ofxMidiKeysBuffers::setNoteRange( int lowNote, int highNote ){
    this->lowNote = lowNote;
    this->highNote = highNote;
}

void ofxMidiKeysBuffers::setMidiChannel( int channel ){
    if( channel >0 && channel <= 16){
        this->channel = channel;
    }else{
        this->channel = 0;
    }
}

void ofxMidiKeysBuffers::processMidi (const std::vector<_ofxPositionedMidiMessage> & readVector, const int &bufferSize ) noexcept{

        //clear buffers
        for(int i=0; i<gateMessages.size(); ++i){
                gateMessages[i].clearMessages();
                pitchMessages[i].clearMessages();
                portaMessages[i].clearMessages();
        }
        singleGateMessages.clearMessages();
        pitchBendMessages.clearMessages();
        pressureMessages.clearMessages();

        
        if(sendClearMessages){
                for(int i=0; i<gateMessages.size(); ++i){
                        gateMessages[i].addMessage(0.0f, 0);
                        switch(portamentoMode){
                            case Off: case Legato: portaMessages[i].addMessage(0.0f, 0); break;
                            case On: portaMessages[i].addMessage(1.0f, 0); break;
                        }
                }
                singleGateMessages.addMessage(0.0f, 0);
                pitchBendMessages.addMessage(0.0f, 0);
                pressureMessages.addMessage(0.0f, 0);
                sendClearMessages = false;  
        }


        switch( voiceMode ){
            case Poly :
                for(const _ofxPositionedMidiMessage &msg : readVector){
                    if ( channel==0 || msg.message.channel == channel ){
                        switch(msg.message.status){
                            case MIDI_NOTE_ON:
                                if(msg.message.pitch >= lowNote && msg.message.pitch<= highNote){
                                    if(msg.message.velocity == 0){
                                        processPolyMidiNoteOff( msg);
                                    }else{
                                        processPolyMidiNoteOn( msg);
                                    }
                                }
                            break;
                            
                            case MIDI_NOTE_OFF:
                                if(msg.message.pitch >= lowNote && msg.message.pitch<= highNote){
                                    processPolyMidiNoteOff(msg);
                                }
                            break;
                            
                            case MIDI_PITCH_BEND:
                            {
                                float value = static_cast<float>((msg.message.value - 8192)) * 0.0001220703125f; // divide by 8192
                                if(value>0) {
                                    value *= pitchBendUpAmount;
                                }else{
                                    value *= pitchBendDownAmount;
                                }
                                pitchBendMessages.addMessage(value, msg.sample);
                            }
                            break;
                            
                            case MIDI_AFTERTOUCH:
                            {
                                float value = static_cast<float>(msg.message.value +1)*0.0078125f; 
                                pressureMessages.addMessage(value, msg.sample); 
                            }
                            break;
                            
                            default: break;
                        }                        
                    }
                } 
            break;
            
            case Mono:
                for(const _ofxPositionedMidiMessage &msg : readVector){
                    if ( channel==0 || msg.message.channel == channel ){
                            
                        switch(msg.message.status){
                            case MIDI_NOTE_ON:
                                if(msg.message.pitch >= lowNote && msg.message.pitch<= highNote) {
                                    if(msg.message.velocity == 0){
                                        processMonoMidiNoteOff( msg);
                                    }else{
                                        processMonoMidiNoteOn( msg);
                                    }
                                }
                            break;
                            
                            case MIDI_NOTE_OFF:
                                if(msg.message.pitch >= lowNote && msg.message.pitch<= highNote) {
                                    processMonoMidiNoteOff(msg);
                                }
                            break;
                            
                            case MIDI_PITCH_BEND:
                            {
                                float value = static_cast<float>((msg.message.value - 8192)) * 0.0001220703125f; // divide by 8192
                                if(value>0) {
                                    value *= pitchBendUpAmount;
                                }else{
                                    value *= pitchBendDownAmount;
                                }
                                pitchBendMessages.addMessage(value, msg.sample);
                            }
                            break;
                            
                            case MIDI_AFTERTOUCH:
                            {
                                float value = static_cast<float>(msg.message.value +1)*0.0078125f; 
                                pressureMessages.addMessage(value, msg.sample); 
                            }
                            break;
                            
                            default: break;
                        }
                    }
                }
            break;
                
            default: break;
        }
    
    
        for(int i=0; i < static_cast<int>(notes.size()); ++i){
            gateMessages[i].processDestination(bufferSize);
            pitchMessages[i].processDestination(bufferSize);
        }
        pitchBendMessages.processDestination(bufferSize);
        pressureMessages.processDestination(bufferSize);
        singleGateMessages.processDestination(bufferSize);
        
        // for checking overflow of note generation, it will fail only there are more than 2000 events generated between calls, unlikely
        if (eventIdGenerator>30000) {
                eventIdGenerator = 0;
                for(int i=0; i<static_cast<int>(notes.size()); ++i){
                        notes[i].eventNumber = notes[i].eventNumber - 30000;
                }
        }
}


void ofxMidiKeysBuffers::setMaxNotes(int notes){
        maxNotes = notes;  //basically maxNotes is not used anymore, to remove in future releases
        this->notes.resize(notes);
        gateMessages.resize(notes);
        pitchMessages.resize(notes);
        portaMessages.resize(notes);
        
        for(int i=0; i<notes; ++i){
                gateMessages[i].reserve(MIDINOTEPROCESSORMESSAGERESERVE);
                pitchMessages[i].reserve(MIDINOTEPROCESSORMESSAGERESERVE);
                portaMessages[i].reserve(MIDINOTEPROCESSORMESSAGERESERVE);
        }
        
        clearNotes();
}

void ofxMidiKeysBuffers::clearNotes(){
        for(int i=0; i<static_cast<int>(notes.size()); ++i){
                notes[i].gate = 0.0f;
                notes[i].note = -1;
                notes[i].eventNumber = eventIdGenerator++;
        }
        sendClearMessages = true;
        activeNotes = 0;
}

void ofxMidiKeysBuffers::checkIdGeneration(){
    // for checking overflow of note generation, it will fail only there are more than 2000 events generated between calls, unlikely
    if (eventIdGenerator>30000) {
        eventIdGenerator = 0;
        for(int i=0; i<static_cast<int>(notes.size()); ++i){
            notes[i].eventNumber = notes[i].eventNumber - 30000;
        }
    }
}


int ofxMidiKeysBuffers::noteSteal(){

        int dummyVoiceEventNum = 32000;
        int foundIndex=-1;

        if(activeNotes!=static_cast<int>(notes.size())){ //there is at least 1 inactive note to steal
                for(int i=0; i<static_cast<int>(notes.size()); ++i){
                        if ( !(notes[i].gate>0) ) {
                                if(notes[i].eventNumber<dummyVoiceEventNum){
                                        dummyVoiceEventNum=notes[i].eventNumber;
                                        foundIndex=i;
                                }
                        }
                }
        }else{ // if there are no inactive note we steal an active note
                //we steal the oldest note that is not the highest or lowest
                int indexHighest = -1;
                int indexLowest = -1;
                int highest = -4000;
                int lowest = 4000;

                //blacklist highest and lowest
                for(int i=0; i<static_cast<int>(notes.size()); ++i){
                        if(notes[i].note<lowest){
                                lowest = notes[i].note;
                                indexLowest = i;
                        }else if(notes[i].note>highest){
                                highest = notes[i].note;
                                indexHighest = i;
                        }
                }

                for(int i=0; i<static_cast<int>(notes.size()); ++i){
                        if(i!=indexLowest && i!=indexHighest){
                                if(notes[i].eventNumber<dummyVoiceEventNum){
                                        dummyVoiceEventNum=notes[i].eventNumber;
                                        foundIndex=i;
                                }
                        }
                }
                activeNotes--; //to counterbalance activeNotes++ on the noteOn, as we are stealing active notes
        }
        return foundIndex;
}




void ofxMidiKeysBuffers::processPolyMidiNoteOn(const _ofxPositionedMidiMessage& midi ) noexcept{
        
        bool retrigger = false;
        int noteNumber = midi.message.pitch;
        int noteIndex=-1;

        float gateValue = static_cast<float>(midi.message.velocity+1)*0.0078125f;  // add 1 and divide by 128, 
                                                                                //so range is 0.0078125f-1.0f
        //search a note with the same note number, gated or not
        for(int i=0; i<static_cast<int>(notes.size()); ++i){
                if(notes[i].note == noteNumber){
                        //retrigger note
                        noteIndex = i;
                        retrigger= true;
                }
        }

        if(!retrigger){
                //steal note
                noteIndex = noteSteal();
        }

        gateMessages[noteIndex].addMessage(gateValue, midi.sample);

        if(!retrigger){ //with retrigger the pitch is always the same
                pitchMessages[noteIndex].addMessage(static_cast<float>(noteNumber), midi.sample);

                switch(portamentoMode){
                case Off:
                        portaMessages[noteIndex].addMessage(0.0f, midi.sample);  
                        break;
                default:
                        portaMessages[noteIndex].addMessage(1.0f, midi.sample); 
                        break;
                }
                
        }

        //update the note state
        notes[noteIndex].note = noteNumber;
        notes[noteIndex].gate = gateValue;
        notes[noteIndex].eventNumber = ++eventIdGenerator;

        singleGateMessages.addMessage(gateValue, midi.sample);
     
        activeNotes++;
}


void ofxMidiKeysBuffers::processPolyMidiNoteOff(const _ofxPositionedMidiMessage& midi ) noexcept{

        //std::cout<<"poly note off\n";
        int noteNumber = midi.message.pitch;
        int noteIndex = -1;

        //1 - search a note with the same note number, gated or not, update and call retrigger on the priorityArray
        for(int i=0; i<static_cast<int>(notes.size()); ++i){
                if(notes[i].note == noteNumber){
                        //retrigger note
                        noteIndex = i;
                }
        }

        if(noteIndex!=-1){
                // do the necessary calculations on the outputs
                //gateMessages[noteIndex].addMessage(0.0f, sample);
                notes[noteIndex].gate = 0.0f;
                notes[noteIndex].eventNumber = ++eventIdGenerator;

                gateMessages[noteIndex].addMessage(0.0f, midi.sample);
                if(activeNotes==1){
                        //single trigger note off
                        singleGateMessages.addMessage(0.0f, midi.sample);
                        
                }
                activeNotes--;
        }                
}






int ofxMidiKeysBuffers::getHighestPriorityMono(){
    int indexToReturn = -1;

    if(monoMode==Last){
        int event = -4000;
        for(int i=0; i<static_cast<int>(notes.size()); ++i){
            if( (notes[i].gate>0) && notes[i].eventNumber>event){
                event = notes[i].eventNumber;
                indexToReturn = i;
            }
        }
    }else if(monoMode==Low){
        int lowest = 4000;
        for(int i=0; i<static_cast<int>(notes.size()); ++i){
            if((notes[i].gate>0) && notes[i].note<lowest){
                lowest = notes[i].note;
                indexToReturn = i;
            }
        }
    }else if(monoMode==High){
        int highest = -4000;
        for(int i=0; i<static_cast<int>(notes.size()); ++i){
            if( (notes[i].gate>0) && notes[i].note>highest){
                highest = notes[i].note;
                indexToReturn = i;
            }
        }
    }
    return indexToReturn;
}



void ofxMidiKeysBuffers::processMonoMidiNoteOn(const _ofxPositionedMidiMessage& midi ) noexcept{
        
        bool retrigger = false;
        int noteNumber = midi.message.pitch;
        int noteIndex  = -1;

        float gateValue = static_cast<float>(midi.message.velocity+1)*0.0078125f;  // add 1 and divide by 128, 
                                                                                //so range is 0.0078125f-1.0f
        
        for(int i=0; i<static_cast<int>(notes.size()); ++i){
                if(notes[i].note == noteNumber){
                        //retrigger note
                        noteIndex = i;
                        retrigger= true;
                }
        }

        if(!retrigger){
                //steal note
                noteIndex = noteSteal();
        }
        
        //update the note state
        notes[noteIndex].note = noteNumber;
        notes[noteIndex].gate = gateValue;
        notes[noteIndex].eventNumber = ++eventIdGenerator;
        
        //now we must check if the new note is suitable to become a the new monophonic note
        int lastMonoNoteIndex = monoNoteIndex;

        if(monoMode!=Last){
                monoNoteIndex = getHighestPriorityMono();
        }else{
                monoNoteIndex = noteIndex;
        }

        if(monoNoteIndex!=lastMonoNoteIndex){ //if mono note index is changed
                pitchMessages[0].addMessage(static_cast<float>(notes[monoNoteIndex].note), midi.sample);
                switch(portamentoMode){
                case Off:
                        portaMessages[0].addMessage(0.0f, midi.sample);  
                        break;
                case On:
                        portaMessages[0].addMessage(1.0f, midi.sample); 
                        break;
                case Legato:
                        if(activeNotes==0){
                                portaMessages[0].addMessage(0.0f, midi.sample);    
                        }else{
                                portaMessages[0].addMessage(1.0f, midi.sample); 
                        }
                        break;
                default: break;
                }
        } 
        
        if(monoNoteIndex!=lastMonoNoteIndex || activeNotes==0){
                gateMessages[0].addMessage(notes[monoNoteIndex].gate, midi.sample);
        }

        activeNotes++;        
}


void ofxMidiKeysBuffers::processMonoMidiNoteOff(const _ofxPositionedMidiMessage& midi ) noexcept {
       
        int noteNumber = midi.message.pitch;
        int noteIndex = -1;

        //search a note with the same note number, gated or not, update and call retrigger on the priorityArray
        for(int i=0; i<static_cast<int>(notes.size()); ++i){
                if(notes[i].note == noteNumber){
                        //retrigger note
                        noteIndex = i;
                }
        }

        if(noteIndex!=-1){
                           
                notes[noteIndex].gate = 0.0f;
                notes[noteIndex].eventNumber = ++eventIdGenerator;

                if(activeNotes==1){
                        //single trigger note off
                        gateMessages[0].addMessage(0.0f, midi.sample);
                }else{
                        //we got another active note to use instead of the released one maybe
                        if(noteIndex==monoNoteIndex){
                            
                                //we released the active mono note index, but we can switch to another note
                                monoNoteIndex = getHighestPriorityMono();
                                pitchMessages[0].addMessage(static_cast<float>(notes[monoNoteIndex].note), midi.sample);
                              
                                switch(portamentoMode){
                                case Off:
                                        portaMessages[0].addMessage(0.0f, midi.sample);  
                                        break;
                                case On:
                                        portaMessages[0].addMessage(1.0f, midi.sample); 
                                        break;
                                case Legato:
                                        if(activeNotes==0){
                                                portaMessages[0].addMessage(0.0f, midi.sample);    
                                        }else{
                                                portaMessages[0].addMessage(1.0f, midi.sample); 
                                        }
                                        break;
                                default: break;
                                }
                                                                
                                gateMessages[0].addMessage(notes[monoNoteIndex].gate, midi.sample);

                        }
                }
                activeNotes--;
        }        
}

#endif
