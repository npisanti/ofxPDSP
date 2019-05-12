
#include "ComputerKeyboard.h"

namespace pdsp{

ComputerKeyboard::ComputerKeyboard(){
    
        eventIdGenerator = 0;
        monoNoteIndex = -1;
        activeNotes = 0;

        notes.reserve( 32 );

        keys.resize(13);
        
        keys[0]  = 'a';
        keys[1]  = 'w';
        keys[2]  = 's';
        keys[3]  = 'e';
        keys[4]  = 'd';
        keys[5]  = 'f';
        keys[6]  = 't';
        keys[7]  = 'g';
        keys[8]  = 'y';
        keys[9]  = 'h';
        keys[10] = 'u';
        keys[11] = 'j';
        keys[12] = 'k';

        keysPressed.resize(13);
        
        keysWhite.resize(13);
        keysWhite[0]  = true;
        keysWhite[1]  = false;
        keysWhite[2]  = true;
        keysWhite[3]  = false;
        keysWhite[4]  = true;
        keysWhite[5]  = true;
        keysWhite[6]  = false;
        keysWhite[7]  = true;
        keysWhite[8]  = false;
        keysWhite[9]  = true;
        keysWhite[10] = false;
        keysWhite[11] = true;
        keysWhite[12] = true;
        
        voiceMode = Poly;
        monoMode = Last;
        setMaxNotes(8);

        dynamic = 1.0f;
        basePitch = 48;
        
}


void ComputerKeyboard::setPolyMode( int voices ) {
    voiceMode = Poly;
    setMaxNotes( voices );
}

void ComputerKeyboard::setMonoMode( bool legato, pdsp::MonoPriority priority ) {
    voiceMode = Mono;
    monoMode = priority;
    this->legato = legato;
    setMaxNotes( 1 );
}

int ComputerKeyboard::getVoicesNumber() const{
    return maxNotes;
}

void ComputerKeyboard::setVoiceMode( pdsp::VoiceMode mode){
        voiceMode = mode;
        clearNotes();
}

void ComputerKeyboard::setMonoPriority( pdsp::MonoPriority mode){
        monoMode = mode; 
        clearNotes();
}

void ComputerKeyboard::setPortamento( pdsp::PortamentoMode mode, float timeMs ) {
        if(mode == On ){
            for(auto & value : outs_pitch ){
                value.enableSmoothing( timeMs );
            }
        }else{            
            if(mode == Legato){
                cout<<"[pdsp] legato mode not available for ComputerKeyboard\n";
            }
            for(auto & value : outs_pitch ){
                value.disableSmoothing();
            }
        }
        
}


void ComputerKeyboard::setMaxNotes(int notes){
        maxNotes = notes;  //basically maxNotes is not used anymore, to remove in future releases
        this->notes.resize(notes);
        outs_trig.resize(notes);
        outs_pitch.resize(notes);

        clearNotes();
}

void ComputerKeyboard::clearNotes(){
        for(int i=0; i<static_cast<int>(notes.size()); ++i){
                notes[i].gate = 0.0f;
                notes[i].note = -1;
                notes[i].eventNumber = eventIdGenerator++;
        }
        for( size_t i=0; i<outs_trig.size(); ++i ){
                outs_trig[i].off();
        }
        activeNotes = 0;
}

void ComputerKeyboard::keysControl( int key ) {
    
    switch( key){
        case 'c':
            if(dynamic>0.0f) dynamic -= 0.125f;
            break;
        
        case 'v':
            if(dynamic<1.0f) dynamic += 0.125f;
            break;
        
        case 'z':
            basePitch  -= 12;
            break;
        
        case 'x':
            basePitch += 12;
            break;
    }
    
}


void ComputerKeyboard::keyPressed ( int key ) {
    
        int pitch = -1;
        for(int i=0; i<13; ++i){
            if(key == keys[i] && keysPressed[i]==0){
                keysPressed[i] = 1;
                pitch = basePitch +i;
            }
        }

        if(pitch > 0){
            switch( voiceMode ){
                case Poly :
                    processPolyNoteOn( pitch );
                    break;
                    
                case Mono:
                    processMonoNoteOn( pitch );
                    break;
                    
                default: break;
            }
    
            // for checking overflow of note generation, it will fail only there are more than 2000 events generated between calls, unlikely
            checkIdGeneration();            
        }
}

void ComputerKeyboard::keyReleased ( int key ) {

        int pitch = -1;
        for(int i=0; i<13; ++i){
            if(key == keys[i] ){
                keysPressed[i] = 0;
                pitch = basePitch + i;
            }
        }

        if(pitch > 0){
            switch( voiceMode ){
                case Poly :
                    processPolyNoteOff( pitch );
                    break;
                    
                case Mono:
                    processMonoNoteOff( pitch );
                    break;
                default: break;
            }        
            
            // for checking overflow of note generation, it will fail only there are more than 2000 events generated between calls, unlikely
            checkIdGeneration();            
        }else{
            
            keysControl ( key ); // function keys
        
        }
}



void ComputerKeyboard::checkIdGeneration(){
    // for checking overflow of note generation, it will fail only there are more than 2000 events generated between calls, unlikely
    if (eventIdGenerator>30000) {
        eventIdGenerator = 0;
        for(int i=0; i<static_cast<int>(notes.size()); ++i){
            notes[i].eventNumber = notes[i].eventNumber - 30000;
        }
    }
}


int ComputerKeyboard::noteSteal(){

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




void ComputerKeyboard::processPolyNoteOn( int pitch ) noexcept{
        
        bool retrigger = false;
        int noteIndex=-1;

        
        //search a note with the same note number, gated or not
        for(int i=0; i<static_cast<int>(notes.size()); ++i){
                if(notes[i].note == pitch){
                        //retrigger note
                        noteIndex = i;
                        retrigger= true;
                }
        }

        if(!retrigger){
                //steal note
                noteIndex = noteSteal();
        }

        outs_trig[noteIndex].trigger( dynamic );

        if(!retrigger){ //with retrigger the pitch is always the same
                outs_pitch[noteIndex].set(static_cast<float>(pitch));                
        }

        //update the note state
        notes[noteIndex].note = pitch;
        notes[noteIndex].gate = dynamic;
        notes[noteIndex].eventNumber = ++eventIdGenerator;
     
        activeNotes++;
}


void ComputerKeyboard::processPolyNoteOff( int pitch ) noexcept{

        //std::cout<<"poly note off\n";
        int noteIndex = -1;

        //1 - search a note with the same note number, gated or not, update and call retrigger on the priorityArray
        for(int i=0; i<static_cast<int>(notes.size()); ++i){
                if(notes[i].note == pitch){
                        //retrigger note
                        noteIndex = i;
                }
        }

        if(noteIndex!=-1){
                // do the necessary calculations on the outputs
                //gateMessages[noteIndex].addMessage(0.0f, sample);
                notes[noteIndex].gate = 0.0f;
                notes[noteIndex].eventNumber = ++eventIdGenerator;
                outs_trig[noteIndex].off();

                activeNotes--;
        }                
}






int ComputerKeyboard::getHighestPriorityMono(){
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



void ComputerKeyboard::processMonoNoteOn( int pitch ) noexcept{
        
        bool retrigger = false;
        int noteIndex  = -1;

        for(int i=0; i<static_cast<int>(notes.size()); ++i){
                if(notes[i].note == pitch){
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
        notes[noteIndex].note = pitch;
        notes[noteIndex].gate = dynamic;
        notes[noteIndex].eventNumber = ++eventIdGenerator;
        
        //now we must check if the new note is suitable to become a the new monophonic note
        int lastMonoNoteIndex = monoNoteIndex;

        if(monoMode!=Last){
                monoNoteIndex = getHighestPriorityMono();
        }else{
                monoNoteIndex = noteIndex;
        }

        if(monoNoteIndex!=lastMonoNoteIndex){ //if mono note index is changed
                outs_pitch[0].set(static_cast<float>(notes[monoNoteIndex].note));
        } 
        
        if(monoNoteIndex!=lastMonoNoteIndex || activeNotes==0){
                if(legato && activeNotes!=0){
                    outs_trig[0].trigger( - dynamic );
                }else{
                    outs_trig[0].trigger(dynamic);
                }
        }

        activeNotes++;        
}


void ComputerKeyboard::processMonoNoteOff( int pitch ) noexcept {
       
        int noteIndex = -1;

        //search a note with the same note number, gated or not, update and call retrigger on the priorityArray
        for(int i=0; i<static_cast<int>(notes.size()); ++i){
                if(notes[i].note == pitch){
                        //retrigger note
                        noteIndex = i;
                }
        }

        if(noteIndex!=-1){
                           
                notes[noteIndex].gate = 0.0f;
                notes[noteIndex].eventNumber = ++eventIdGenerator;

                if(activeNotes==1){
                        //single trigger note off
                        outs_trig[0].off();
                }else{
                        //we got another active note to use instead of the released one maybe
                        if(noteIndex==monoNoteIndex){
                            
                                //we released the active mono note index, but we can switch to another note
                                monoNoteIndex = getHighestPriorityMono();
                                outs_pitch[0].set( static_cast<float>(notes[monoNoteIndex].note) );
                                outs_trig[0].trigger(dynamic);

                        }
                }
                activeNotes--;
        }        
}


void ComputerKeyboard::draw(int x, int y, int w, int h) {
    
    ofPushMatrix();
    ofTranslate(x, y);
    float keyW = w / 13.0f;
    float keyH = h * 0.4f;
    float pressH = keyH - 20;    
    ofNoFill();
    
    //ofDrawRectangle(0, 0, w, h);
    for(int i=0; i<13; ++i){
        
        float posX = i*keyW;
        float posY = 0;
        
        if(keysWhite[i]){
            posY+=keyH;
        }

        if(keysPressed[i] == 1){
            float pressY = posY + 20;
            ofFill();
            ofDrawRectangle( posX, pressY, keyW, pressH);
            ofNoFill();
        }
        
        ofDrawRectangle(posX, posY, keyW, keyH);        
        ofDrawBitmapString( (char) keys[i], posX+4, posY+10);
    }
    
    std::string info = "z/x: base pitch ( ";
    info += to_string( basePitch );
    info += " )\nc-v dynamic ( ";
    info += to_string (dynamic);
    info += " )";
    ofDrawBitmapString( info, 0, keyH*2 + 20);
    
    ofPopMatrix();
    
}


pdsp::TriggerControl & ComputerKeyboard::out_trig( int voice ) {
    return outs_trig[voice];
}

pdsp::ValueControl &  ComputerKeyboard::out_pitch( int voice ) {
    return outs_pitch[voice]; 
}


}
