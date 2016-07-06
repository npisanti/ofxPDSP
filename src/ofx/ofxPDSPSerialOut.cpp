
#include "ofxPDSPSerialOut.h"

#define OFXPDSP_SERIALOUTPUTCIRCULARBUFFERSIZE 1024


ofxPDSPSerialOut::ScheduledSerialMessage::ScheduledSerialMessage(){  };

ofxPDSPSerialOut::ScheduledSerialMessage::ScheduledSerialMessage(int channel, float message,  chrono::high_resolution_clock::time_point schedule) {
    if (channel<1) channel = 1;
    if (channel>127) channel = 127;
    int msg = (int) message;
    if (msg<0) msg = 0; 
    if (msg>127) msg = 127; 

    channel = - channel;
    this->channel = (signed char) channel;
    this->message = (signed char) msg;

    this->scheduledTime = schedule;
                    
};

ofxPDSPSerialOut::ScheduledSerialMessage::ScheduledSerialMessage(const ofxPDSPSerialOut::ScheduledSerialMessage &other){
    this->channel          = other.channel;
    this->message          = other.message;
    this->scheduledTime = other.scheduledTime;
}

ofxPDSPSerialOut::ScheduledSerialMessage& ofxPDSPSerialOut::ScheduledSerialMessage::operator= (const ofxPDSPSerialOut::ScheduledSerialMessage &other){
    this->channel          = other.channel;
    this->message          = other.message;
    this->scheduledTime = other.scheduledTime;
    return *this;
}

ofxPDSPSerialOut::ScheduledSerialMessage::~ScheduledSerialMessage(){}


bool ofxPDSPSerialOut::scheduledSort(const ScheduledSerialMessage &lhs, const ScheduledSerialMessage &rhs ){
    return (lhs.scheduledTime < rhs.scheduledTime);
}


//-------------------------------------------------------------------------------------------

ofxPDSPSerialOut::ofxPDSPSerialOut(){
    inputs.reserve(128);
    messagesToSend.reserve(128);
    messagesToSend.clear();
    verbose = false;
    selectedChannel = 0;
    
    connected = false;
    
    chronoStarted = false;
    
    //midi daemon init
    messagesReady = false;
    runDaemon = false;
    
    //processing init

    circularMax = OFXPDSP_SERIALOUTPUTCIRCULARBUFFERSIZE;
    circularBuffer.resize(circularMax);
    circularRead  = 0;
    circularWrite = 0;
    
    //testing
    messageCount = 0;
}

ofxPDSPSerialOut::~ofxPDSPSerialOut(){
    if(connected){
        close();
    }
}

void ofxPDSPSerialOut::setVerbose( bool verbose ){
    this->verbose = verbose;
}

void ofxPDSPSerialOut::listPorts(){
	serial.listDevices();
}

void ofxPDSPSerialOut::openPort(int portIndex, int baudRate){
    if(connected){
        close();
    }
    
    serial.setup( portIndex, baudRate );
    
    if( serial.isInitialized() ){
        // set this the right way <-------------------------------------------------------------------------------=====
        //if(verbose) cout<<"[pdsp] serial connected to "<<serial.getDeviceList()[portIndex]<<"\n";
        
        startDaemon();
        
        connected = true;
    }else{
        if(verbose) cout<<"[pdsp] failed serial connection to index "<<portIndex<<"\n";
    }
}

void ofxPDSPSerialOut::openPort(string name, int baudRate){
    if(connected){
        close();
    }
    
    serial.setup( name, baudRate );
    
    if( serial.isInitialized() ){
        if(verbose) cout<<"[pdsp] serial connected to "<<name<<"\n";
        
        startDaemon();
        
        connected = true;
    }else{
        if(verbose) cout<<"[pdsp] failed serial connection to "<<name<<"\n";
    }
}

void ofxPDSPSerialOut:: close(){
    if(connected){
        if(verbose) cout<<"[pdsp] shutting down serial out\n";
        //stop the daemon before
        closeDaemon();
        if(serial.isInitialized()){
            serial.close(); 
        }
        connected = false;        
    }
}

pdsp::ExtSequencer& ofxPDSPSerialOut::channel(int channelNumber) {
    selectedChannel = channelNumber;
    return *this;
}

void ofxPDSPSerialOut::linkToMessageBuffer(pdsp::MessageBuffer &messageBuffer) {
    cout<<"linking message buffer\n";
    inputs.push_back(&messageBuffer);
    channels.push_back(selectedChannel);
}

void ofxPDSPSerialOut::unlinkMessageBuffer(pdsp::MessageBuffer &messageBuffer) {
    
    int i=0;
    for (vector<pdsp::MessageBuffer*>::iterator it = inputs.begin(); it != inputs.end(); ++it){
        if (*it == &messageBuffer){
            inputs.erase(it);
            vector<int>::iterator linkedChannels = channels.begin() + i;
            channels.erase(linkedChannels);
            return;
        }
        i++;
    }
    
}

void ofxPDSPSerialOut::prepareToPlay( int expectedBufferSize, double sampleRate ){
    usecPerSample = 1000000.0 / sampleRate;
    
}

void ofxPDSPSerialOut::releaseResources() {}

// OK -----------------------------------------------^^^^^^^^^^^-------------

void ofxPDSPSerialOut::process( int bufferSize ) noexcept{
    
    if(connected){
        //clear messages
        messagesToSend.clear();
        
        //add note messages
        int maxBuffer = inputs.size();
        
        if(chronoStarted){
            chrono::nanoseconds bufferOffset = chrono::nanoseconds (static_cast<long> ( bufferSize * usecPerSample ));
            bufferChrono = bufferChrono + bufferOffset;
        }else{
            bufferChrono = chrono::high_resolution_clock::now();
            chronoStarted = true;
        }
        
        for( int i=0; i<maxBuffer; ++i ){
            
            pdsp::MessageBuffer* messageBuffer = inputs[i];
            int msg_channel = channels[i];
            
            /* // OLD WAY
            bufferChrono = chrono::high_resolution_clock::now();
            */
            
            int bufferMax = messageBuffer->size();

            for(int n=0; n<bufferMax; ++n){                
                //format message to sent
                float msg_value = messageBuffer->messages[n].value;
                int msg_sample = messageBuffer->messages[n].sample;
                
                chrono::nanoseconds offset = chrono::nanoseconds (static_cast<long> ( msg_sample * usecPerSample ));
                chrono::high_resolution_clock::time_point scheduleTime = bufferChrono + offset;
                
                messagesToSend.push_back( ScheduledSerialMessage(msg_channel, msg_value, scheduleTime) );
            }
        }
        
        //sort messages to send
        sort(messagesToSend.begin(), messagesToSend.end(), scheduledSort);
        
        //send to daemon
        if( ! messagesToSend.empty()){
            prepareForDaemonAndNotify();
        }
    }//end checking connected
}

void ofxPDSPSerialOut::startDaemon(){ // OK
    
    runDaemon = true;
    daemonThread = thread( daemonFunctionWrapper, this );   
    
}

void ofxPDSPSerialOut::prepareForDaemonAndNotify(){
    
    unique_lock<mutex> lck (outMutex);  
    //send messages in circular buffer
    for(ScheduledSerialMessage &msg : messagesToSend){
        circularBuffer[circularWrite] = msg;
        ++circularWrite;
        if(circularWrite==circularMax){
            circularWrite = 0;
        }
    }
    messagesReady = true;
    outCondition.notify_all();
    
}
   
    
void ofxPDSPSerialOut::daemonFunctionWrapper(ofxPDSPSerialOut* parent){
    parent->daemonFunction();
}
   
    
void ofxPDSPSerialOut::daemonFunction() noexcept{
    
    while (runDaemon){

        //midiMutex.lock();
        unique_lock<mutex> lck (outMutex);
        while(!messagesReady) outCondition.wait(lck);
        
        if(circularRead != circularWrite){
            
            ScheduledSerialMessage& nextMessage = circularBuffer[circularRead];
            
            if( nextMessage.scheduledTime < chrono::high_resolution_clock::now() ){ //we have to process the scheduled midi
                
                // SEND MESSAGES HERE
                serial.writeByte(nextMessage.channel);
                serial.writeByte(nextMessage.message);
                
                #ifndef NDEBUG
                    if(verbose) cout << "[pdsp] serial message: channel = "<< (- (int)nextMessage.channel)<< " | value = "<<(int)nextMessage.message<<"\n";
                #endif
                
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
   
    if(verbose) cout<<"[pdsp] closing serial out daemon thread\n";
}
    
 
    
void ofxPDSPSerialOut::closeDaemon(){
    runDaemon = false;
    
    unique_lock<mutex> lck (outMutex);  
    //set messages in circular buffer
    messagesReady = true;
    outCondition.notify_all();
    daemonThread.detach();

}
    
    
    
