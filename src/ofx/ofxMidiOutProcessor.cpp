
#include "ofxMidiOutProcessor.h"

#define OFXPDSP_MIDIOUTPUTCIRCULARBUFFERSIZE 1024


ofxMidiOutProcessor::ofxScheduledMidiMessage::ofxScheduledMidiMessage(){  };

ofxMidiOutProcessor::ofxScheduledMidiMessage::ofxScheduledMidiMessage(ofxMidiMessage message, chrono::high_resolution_clock::time_point schedule)
                : midi(message), scheduledTime(schedule){};

ofxMidiOutProcessor::ofxScheduledMidiMessage::ofxScheduledMidiMessage(const ofxMidiOutProcessor::ofxScheduledMidiMessage &other)
                : ofxMidiOutProcessor::ofxScheduledMidiMessage(other.midi, other.scheduledTime){}

ofxMidiOutProcessor::ofxScheduledMidiMessage& ofxMidiOutProcessor::ofxScheduledMidiMessage::operator= (const ofxMidiOutProcessor::ofxScheduledMidiMessage &other){
    this->midi          = other.midi;
    this->scheduledTime = other.scheduledTime;
    return *this;
}

ofxMidiOutProcessor::ofxScheduledMidiMessage::~ofxScheduledMidiMessage(){}


bool ofxMidiOutProcessor::scheduledMidiSort(const ofxScheduledMidiMessage &lhs, const ofxScheduledMidiMessage &rhs ){
    return (lhs.scheduledTime < rhs.scheduledTime);
}


//-------------------------------------------------------------------------------------------

ofxMidiOutProcessor::ofxMidiOutProcessor(){
    inputs[0].reserve(128);
    inputs[1].reserve(128);
    inputs[2].reserve(128);
    defaultNote.reserve(128);
    messagesToSend.reserve(128);
    messagesToSend.clear();
    verbose = false;
    selectedType = nullType;
    selectedMidiChannel = 0;
    
    midiOut_p = nullptr;
    connected = false;
    
    //midi daemon init
    messagesReady = false;
    runMidiDaemon = false;
    
    //processing init

    circularMax = OFXPDSP_MIDIOUTPUTCIRCULARBUFFERSIZE;
    circularBuffer.resize(circularMax);
    circularRead  = 0;
    circularWrite = 0;
    
    //testing
    messageCount = 0;
}

ofxMidiOutProcessor::~ofxMidiOutProcessor(){
    if(connected){
        closePort();
    }
}

void ofxMidiOutProcessor::setVerbose( bool verbose ){
    this->verbose = verbose;
}

void ofxMidiOutProcessor::linkToMidiOut(ofxMidiOut &midiOut){

    if( midiOut.isOpen() ){
        if(connected){
            closePort();
        }

        midiOut_p = &midiOut;
        if(verbose) cout<<"[ofxPDSP]: linked to midi Out \n";

        startMidiDaemon();
        
        connected = true;
    }else{
        if(verbose) cout<<"[ofxPDSP]: midi out not linked, open midi out before linking! \n";
    }
}

void ofxMidiOutProcessor::listPorts(){
    midiOut.listPorts();
}

void ofxMidiOutProcessor::openPort(int portIndex){
    if(connected){
        closePort();
    }
    
    midiOut.openPort( portIndex );
    
    if( midiOut.isOpen() ){
        if(verbose) cout<<"[ofxPDSP]: connected to midi port "<<portIndex<<"\n";
        
        midiOut_p = &midiOut;
        
        startMidiDaemon();
        
        connected = true;
    }
}

void ofxMidiOutProcessor:: closePort(){
    if(connected){
        if(verbose) cout<<"[ofxPDSP]: shutting down midi out\n";
        //stop the daemon before
        closeMidiDaemon();
        if(midiOut.isOpen()){
            midiOut.closePort();  
        }
        midiOut_p = nullptr;
        connected = false;        
    }
}

pdsp::ExtSequencer& ofxMidiOutProcessor::gate(int midiChannel, int defaultNote ){
    selectedType = gateType;
    selectedMidiChannel = midiChannel;
    selectedDefaultNote = defaultNote;
    return *this;
}

pdsp::ExtSequencer& ofxMidiOutProcessor::note(){ //link to the last gate 
    selectedType = noteType;
    return *this; 
}

pdsp::ExtSequencer& ofxMidiOutProcessor::cc(int midiChannel, int ccNumber){
    selectedType = ccType;
    selectedMidiChannel = midiChannel;
    selectedCC = ccNumber;
    return *this;
}

void ofxMidiOutProcessor::linkToMessageBuffer(pdsp::MessageBuffer &messageBuffer) {
    if(selectedType != nullType){
        switch(selectedType){
        case gateType:
            inputs[gateType].push_back(&messageBuffer);
            inputs[noteType].push_back(nullptr);
            defaultNote.push_back(selectedDefaultNote);
            midiChannelsNote.push_back(selectedMidiChannel);
            break;
        case noteType:
            if(!inputs[noteType].empty()){ 
                inputs[noteType].back() = &messageBuffer; //add note coupled to last inserted gate
            }
            break;
        case ccType:
            inputs[ccType].push_back(&messageBuffer);
            ccNumbers.push_back(selectedCC);
            midiChannelsCC.push_back(selectedMidiChannel);
            break;
        }
    }
    selectedType = nullType;
}

void ofxMidiOutProcessor::unlinkMessageBuffer(pdsp::MessageBuffer &messageBuffer) {
    
    int i=0;
    for (vector<pdsp::MessageBuffer*>::iterator it = inputs[gateType].begin(); it != inputs[gateType].end(); ++it){
        if (*it == &messageBuffer){
            inputs[gateType].erase(it);
            vector<pdsp::MessageBuffer*>::iterator linkedNote = inputs[noteType].begin() + i;
            inputs[noteType].erase(linkedNote);
            return;
        }
        i++;
    }
    
    for (vector<pdsp::MessageBuffer*>::iterator it = inputs[noteType].begin(); it != inputs[noteType].end(); ++it){
        if (*it == &messageBuffer){
            *it = nullptr; //unlink note
            return;
        }
    }
    
    for (vector<pdsp::MessageBuffer*>::iterator it = inputs[ccType].begin(); it != inputs[ccType].end(); ++it){
        if (*it == &messageBuffer){
            inputs[ccType].erase(it);
            return;
        }
    }
    
}

void ofxMidiOutProcessor::prepareToPlay( int expectedBufferSize, double sampleRate ){
    usecPerSample = 1000000.0 / sampleRate;
    
}

void ofxMidiOutProcessor::releaseResources() {}


void ofxMidiOutProcessor::process() noexcept{
    
    if(connected){
        //clear messages
        messagesToSend.clear();
        
        //add note messages
        int maxBuffer = inputs[gateType].size();
        for( int i=0; i<maxBuffer; ++i ){
            
            pdsp::MessageBuffer* noteBufferI = inputs[noteType][i];
            pdsp::MessageBuffer* gateBufferI = inputs[gateType][i];
            
            int gateMax = gateBufferI->size();
            int noteIndex=0;
            int noteMax;
            if( noteBufferI == nullptr ){ noteMax = 0; } //this deactivates the search for pitch
            else{ noteMax = noteBufferI->size(); }
          
            
            bufferChrono = chrono::high_resolution_clock::now();
          
            for(int gateIndex=0; gateIndex<gateMax; ++gateIndex){
                //check if we have to change the pitch
                if(  noteIndex<noteMax && 
                    (noteBufferI->messages[noteIndex].sample <= gateBufferI->messages[gateIndex].sample))
                {
                    defaultNote[i] = static_cast<int> ( noteBufferI->messages[noteIndex].value);
                    ++noteIndex;
                }
                
                //format message to sent
                float gateValue = gateBufferI->messages[gateIndex].value;
                int sample = gateBufferI->messages[gateIndex].sample;
                

                chrono::nanoseconds offset = chrono::nanoseconds (static_cast<long> ( sample * usecPerSample ));
                chrono::high_resolution_clock::time_point scheduleTime = bufferChrono + offset;
                
                ofxMidiMessage midi;
                if(gateValue == 0.0f){
                    midi.status = MIDI_NOTE_OFF;
                    midi.velocity = 64;
                }else{
                    midi.status = MIDI_NOTE_ON;
                    if(gateValue > 1.0f) gateValue = 1.0f;
                    midi.velocity = static_cast<int> ((gateValue*128) -1);
                }
                midi.pitch = defaultNote[i];
                midi.channel = midiChannelsNote[i];
                midi.control = 0;
                
                //todo:: correct nanosec time
                messagesToSend.push_back( ofxScheduledMidiMessage(midi, scheduleTime) );
            }
        }
        
        //add midi messages
        maxBuffer = inputs[ccType].size();
        for( int i=0; i<maxBuffer; ++i ){
            
            pdsp::MessageBuffer* ccBufferI = inputs[ccType][i];       
            int ccMax = ccBufferI->size();
            for(int ccIndex=0; ccIndex<ccMax; ++ccIndex){
                
                ofxMidiMessage midi;
                midi.status = MIDI_CONTROL_CHANGE;
                midi.channel = midiChannelsCC[i];
                midi.control = ccBufferI->messages[ccIndex].value;
                
                int sample = ccBufferI->messages[ccIndex].sample;
                
                chrono::nanoseconds offset = chrono::nanoseconds ( static_cast<long>(sample * usecPerSample) );
                chrono::high_resolution_clock::time_point scheduleTime = bufferChrono + offset;
                
                messagesToSend.push_back( ofxScheduledMidiMessage(midi, scheduleTime) );
            }   
        }
            
        //sort messages to send
        sort(messagesToSend.begin(), messagesToSend.end(), scheduledMidiSort);
        
        //send to daemon
        if( ! messagesToSend.empty()){
            prepareForDaemonAndNotify();
        }
    }//end checking connected
}

void ofxMidiOutProcessor::startMidiDaemon(){
    
    runMidiDaemon = true;
    midiDaemonThread = thread( midiDaemonFunctionWrapper, this );   
    
}

void ofxMidiOutProcessor::prepareForDaemonAndNotify(){
    
    unique_lock<mutex> lck (midiOutMutex);  
    //send messages in circular buffer
    for(ofxScheduledMidiMessage &msg : messagesToSend){
        circularBuffer[circularWrite] = msg;
        ++circularWrite;
        if(circularWrite==circularMax){
            circularWrite = 0;
        }
    }
    messagesReady = true;
    midiOutCondition.notify_all();
    
}
   
    
void ofxMidiOutProcessor::midiDaemonFunctionWrapper(ofxMidiOutProcessor* parent){
    parent->midiDaemonFunction();
}
   
    
void ofxMidiOutProcessor::midiDaemonFunction() noexcept{
    
    while (runMidiDaemon){

        //midiMutex.lock();
        unique_lock<mutex> lck (midiOutMutex);
        while(!messagesReady) midiOutCondition.wait(lck);
        
        if(circularRead != circularWrite){
            
            ofxScheduledMidiMessage& nextMessage = circularBuffer[circularRead];
            
            if( nextMessage.scheduledTime < chrono::high_resolution_clock::now() ){ //we have to process the scheduled midi
                
                switch(nextMessage.midi.status){
                case MIDI_NOTE_ON:
                    midiOut_p->sendNoteOn(nextMessage.midi.channel, nextMessage.midi.pitch, nextMessage.midi.velocity);
                    break;
                case MIDI_NOTE_OFF:
                    midiOut_p->sendNoteOff(nextMessage.midi.channel, nextMessage.midi.pitch, nextMessage.midi.velocity);
                    break;
                case MIDI_CONTROL_CHANGE:
                    midiOut_p->sendNoteOff(nextMessage.midi.channel, nextMessage.midi.control, nextMessage.midi.value);
                    break;
                default: break;
                }
                
                ++circularRead;
                if(circularRead == circularMax){
                    circularRead = 0;
                }
            }

        }else{
            messagesReady = false;
        }

        this_thread::yield();

    }
   
    if(verbose) cout<<"[ofxPDSP] closing midi out daemon thread\n";
}
    
 
    
void ofxMidiOutProcessor::closeMidiDaemon(){
    runMidiDaemon = false;
    
    unique_lock<mutex> lck (midiOutMutex);  
    //set messages in circular buffer
    messagesReady = true;
    midiOutCondition.notify_all();
    midiDaemonThread.detach();

}
    
    
    
