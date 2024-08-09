
#include "SerialOut.h"

#ifndef TARGET_OF_IOS
#ifndef __ANDROID__

#define OFXPDSP_SERIALOUTPUTCIRCULARBUFFERSIZE 4096

pdsp::serial::Output::ScheduledSerialMessage::ScheduledSerialMessage(){  };

pdsp::serial::Output::ScheduledSerialMessage::ScheduledSerialMessage(int channel, float message,  std::chrono::high_resolution_clock::time_point schedule) {
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

pdsp::serial::Output::ScheduledSerialMessage::ScheduledSerialMessage(const pdsp::serial::Output::ScheduledSerialMessage &other){
    this->channel          = other.channel;
    this->message          = other.message;
    this->scheduledTime = other.scheduledTime;
}

pdsp::serial::Output::ScheduledSerialMessage& pdsp::serial::Output::ScheduledSerialMessage::operator= (const pdsp::serial::Output::ScheduledSerialMessage &other){
    this->channel          = other.channel;
    this->message          = other.message;
    this->scheduledTime = other.scheduledTime;
    return *this;
}

pdsp::serial::Output::ScheduledSerialMessage::~ScheduledSerialMessage(){}


bool pdsp::serial::Output::scheduledSort(const ScheduledSerialMessage &lhs, const ScheduledSerialMessage &rhs ){
    return (lhs.scheduledTime < rhs.scheduledTime);
}


//-------------------------------------------------------------------------------------------

pdsp::serial::Output::Output(){
    inputs.reserve(128);
    messagesToSend.reserve(128);
    messagesToSend.clear();
    verbose = false;
    selectedChannel = 0;
    
    connected = false;
    
    chronoStarted = false;
    
    //midi daemon init
    runDaemon = false;
    
    //processing init

    circularBuffer.resize(OFXPDSP_SERIALOUTPUTCIRCULARBUFFERSIZE);   
    writeindex = 0;
    send = 0;
    
    //testing
    messageCount = 0;
}

pdsp::serial::Output::~Output(){
    if(connected){
        close();
    }
}

void pdsp::serial::Output::setVerbose( bool verbose ){
    this->verbose = verbose;
}

void pdsp::serial::Output::listPorts(){
	serial.listDevices();
}

void pdsp::serial::Output::openPort(int portIndex, int baudRate){
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

void pdsp::serial::Output::openPort(string name, int baudRate){
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

void pdsp::serial::Output:: close(){
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

pdsp::ExtSequencer& pdsp::serial::Output::channel(int channelNumber) {
    selectedChannel = channelNumber;
    return *this;
}

void pdsp::serial::Output::linkToMessageBuffer(pdsp::MessageBuffer &messageBuffer) {
#ifndef NDEBUG
    cout<<"[pdsp] linking message buffer\n";
#endif
    inputs.push_back(&messageBuffer);
    channels.push_back(selectedChannel);
}

void pdsp::serial::Output::unlinkMessageBuffer(pdsp::MessageBuffer &messageBuffer) {
    
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

void pdsp::serial::Output::prepareToPlay( int expectedBufferSize, double sampleRate ){
    usecPerSample = 1000000.0 / sampleRate;
    
}

void pdsp::serial::Output::releaseResources() {}

// OK -----------------------------------------------^^^^^^^^^^^-------------

void pdsp::serial::Output::process( int bufferSize ) noexcept{
    if(connected){
        //clear messages
        messagesToSend.clear();
        
        //add note messages
        int maxBuffer = inputs.size();
        
        if(chronoStarted){
            std::chrono::nanoseconds bufferOffset = std::chrono::nanoseconds (static_cast<long> ( bufferSize * usecPerSample ));
            bufferChrono = bufferChrono + bufferOffset;
        }else{
            bufferChrono = std::chrono::high_resolution_clock::now();
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
                
				std::chrono::nanoseconds offset = std::chrono::nanoseconds (static_cast<long> ( msg_sample * usecPerSample ));
				std::chrono::high_resolution_clock::time_point scheduleTime = bufferChrono + offset;
                
                messagesToSend.push_back( ScheduledSerialMessage(msg_channel, msg_value, scheduleTime) );
            }
        }
        
        //sort messages to send
        sort(messagesToSend.begin(), messagesToSend.end(), scheduledSort);
        
        //send to daemon
        for(ScheduledSerialMessage &msg : messagesToSend){
            circularBuffer[writeindex] = msg;
            int write = writeindex+1;
            if( write >= (int)circularBuffer.size() ){ write = 0; };
            writeindex = write;
        }
    }//end checking connected
}

void pdsp::serial::Output::startDaemon(){ // OK
    
    runDaemon = true;
    daemonThread = std::thread( daemonFunctionWrapper, this );   
    
}
    
void pdsp::serial::Output::daemonFunctionWrapper(pdsp::serial::Output* parent){
    parent->daemonFunction();
}
       
void pdsp::serial::Output::daemonFunction() noexcept{
    while (runDaemon){

        while( send!=writeindex && circularBuffer[send].scheduledTime < std::chrono::high_resolution_clock::now() ){
           
            // SEND MESSAGES HERE
            ScheduledSerialMessage& nextMessage = circularBuffer[send];
                           
            // SEND MESSAGES HERE
            serial.writeByte( (char)nextMessage.channel );
            serial.writeByte( (char)nextMessage.message );
            
            #ifndef NDEBUG
                if(verbose) cout << "[pdsp] serial message: channel = "<< (- (int)nextMessage.channel)<< " | value = "<<(int)nextMessage.message<<"\n";
            #endif

            send++;
        }

        std::this_thread::sleep_for(std::chrono::microseconds(500));
    }    
    
    if(verbose) cout<<"[pdsp] closing serial out daemon thread\n";
}
    
 
    
void pdsp::serial::Output::closeDaemon(){
    runDaemon = false;
    daemonThread.detach();
}

#endif // __ANDROID__
#endif // TARGET_OF_IOS
    
    
