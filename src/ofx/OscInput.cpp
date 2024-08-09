
#include "OscInput.h"

#define OFXPDSP_OSCINPUT_MESSAGERESERVE 128
#define OFXPDSP_OSCCIRCULARBUFFER_SIZE 10000

std::vector<pdsp::osc::Input*> pdsp::osc::Input::instances;

pdsp::osc::Input::Input() {
    
    parsers.clear();
    
    readVector.reserve(OFXPDSP_OSCINPUT_MESSAGERESERVE);
    circularBuffer.resize( OFXPDSP_OSCCIRCULARBUFFER_SIZE );
    
    lastread = 0;
    index = 0;
    
    connected = false;
    
    tempoLinked = false;
    tempoAddress = "";
    tempoArgument = 0;
    
    receiver.circularBuffer = &circularBuffer;
    receiver.index = &index;

    instances.push_back(this);
}   


pdsp::osc::Input::~Input(){
    for (std::size_t i = 0; i < instances.size(); ++i) {
        if (instances[i] == this) {
            instances.erase(instances.begin() + i);
            break;
        }
    }

    if(connected){
        close();
    }
    for( std::size_t i=0; i<parsers.size(); ++i ){
        delete parsers[i];
    }
}

void pdsp::osc::Input::setVerbose( bool verbose ){
    this->verbose = verbose;
}


void pdsp::osc::Input::openPort( int port ) {
    if(connected){
        close();
    }
    
    receiver.setup( port );
    connected = true;
    bufferChrono = std::chrono::high_resolution_clock::now();
}



void pdsp::osc::Input::close(){
    if(connected){
        if(verbose) cout<<"[pdsp] shutting down OSC out\n";
        //stop the daemon before
        
        receiver.stop();

        connected = false;        
    }
}

void pdsp::osc::Input::linkTempo( string oscAddress, int argument ){
    tempoLinked = true;
    tempoAddress = oscAddress;
    tempoArgument = argument;
}

int pdsp::osc::Input::checkParser( std::string oscAddress ){
    for( std::size_t i=0; i<parsers.size(); ++i ){
        if( parsers[i]->address == oscAddress ){
            return i;
        }
    }
    parsers.emplace_back();
    parsers.back() = new OscParser();
    parsers.back()->address = oscAddress;
    return parsers.size()-1;
}


pdsp::SequencerGateOutput& pdsp::osc::Input::out_trig( string oscAddress, int argument ) {
    int i = checkParser( oscAddress );
    return parsers[i]->out_trig( argument );
}


pdsp::SequencerValueOutput& pdsp::osc::Input::out_value( string oscAddress, int argument ) {
    int i = checkParser( oscAddress );
    return parsers[i]->out_value( argument );
}

std::function<float(float)> & pdsp::osc::Input::parser( string oscAddress, int argument ){
    int i = checkParser( oscAddress );
    return parsers[i]->parser( argument );
}

void pdsp::osc::Input::initTo( string oscAddress, int argument, float value  ){
    for( std::size_t i=0; i<parsers.size(); ++i ){
        if( parsers[i]->address == oscAddress ){
            parsers[i]->initTo( argument, value );
            return;
        }
    }
    std::cout<<"[pdsp] wrong address selected for initTo\n";
    pdsp_trace();
}


void pdsp::osc::Input::clearAll(){
    sendClearMessages = true;
}


void pdsp::osc::Input::prepareToPlay( int expectedBufferSize, double sampleRate ){
    oneSlashMicrosecForSample = 1.0 / (1000000.0 / sampleRate);
}

void pdsp::osc::Input::releaseResources(){}

void pdsp::osc::Input::pushToReadVector( pdsp::osc::Input::_PositionedOscMessage & message ){
        std::chrono::duration<double> offset = message.timepoint - bufferChrono; 
        message.sample = static_cast<int>( static_cast <double>( std::chrono::duration_cast<std::chrono::microseconds>(offset).count()) * oneSlashMicrosecForSample);
        readVector.push_back( message );    
}

void pdsp::osc::Input::processOsc( int bufferSize ) noexcept {
    
    if(connected){
        
        readVector.clear();
        int read = index;

        if(read<lastread){ // two segments
            for(int i=lastread+1; i<(int)circularBuffer.size(); ++i){
                pushToReadVector( circularBuffer[i] );
            }            
            for( int i=0; i<=read; ++i){
                pushToReadVector( circularBuffer[i] );
            }            
        }else{
            for(int i=lastread+1; i<=read; ++i){
                pushToReadVector( circularBuffer[i] );
            }
        }
        
        lastread = read;
        bufferChrono = std::chrono::high_resolution_clock::now();
        
        //now sanitize messages to bufferSize
        for(_PositionedOscMessage &msg : readVector){
            if(msg.sample >= bufferSize){ msg.sample = bufferSize-1; } else
            if(msg.sample < 0 ) { msg.sample = 0; }
        }
        
        // clean the message buffers
        for (std::size_t i = 0; i < parsers.size(); ++i){
            parsers[i]->clear( sendClearMessages );
        }
        
        // adds the messages to the buffers, only the first arg of each osc message is read, as float
        for(_PositionedOscMessage &osc : readVector){
            for( std::size_t k=0; k<parsers.size(); ++k){
                if( parsers[k]->address == osc.message.getAddress() ){
                    parsers[k]->process( osc.message, osc.sample );
                }
            } 
            
            if( tempoLinked ){
                if(osc.message.getAddress() == tempoAddress ){
                    if( tempoArgument < int(osc.message.getNumArgs()) ){
                        switch( osc.message.getArgType( tempoArgument ) ){
                            case OFXOSC_TYPE_INT32:
                                tempoChanged = true;
                                tempo = osc.message.getArgAsInt32( tempoArgument );
                            break;
                            
                            case OFXOSC_TYPE_FLOAT:
                                tempoChanged = true;
                                tempo = osc.message.getArgAsFloat( tempoArgument );
                            break;
                            
                            default: break;
                        }
                    }
                }
            }
        }
        
        // now process all the linked sequencers
        for (std::size_t i = 0; i < parsers.size(); ++i){
            parsers[i]->processDestinations( bufferSize );
        }   
    }
}

bool pdsp::osc::Input::hasTempoChange(){
    if( tempoChanged ){
        tempoChanged = false;
        return true;
    }else{
        return false;
    }
}
double pdsp::osc::Input::getTempo(){
    return tempo;
}


void pdsp::osc::Input::CustomOscReceiver::ProcessMessage(const _PDSPOscReceivedMessage_t &m, const _PDSPIpEndpointName_t &remoteEndpoint){
	// convert the message to an ofxOscMessage
	ofxOscMessage msg;

	// set the address
	msg.setAddress(m.AddressPattern());
	
	// set the sender ip/host
	char endpointHost[_PDSPIpEndpointName_t::ADDRESS_STRING_LENGTH];
	remoteEndpoint.AddressAsString(endpointHost);
	msg.setRemoteEndpoint(endpointHost, remoteEndpoint.port);

	// transfer the arguments
	for(_PDSPOscReceivedMessage_t::const_iterator arg = m.ArgumentsBegin(); arg != m.ArgumentsEnd(); ++arg){
		if(arg->IsInt32()){
			msg.addIntArg(arg->AsInt32Unchecked());
		}
		else if(arg->IsInt64()){
			msg.addInt64Arg(arg->AsInt64Unchecked());
		}
		else if( arg->IsFloat()){
			msg.addFloatArg(arg->AsFloatUnchecked());
		}
		else if(arg->IsDouble()){
			msg.addDoubleArg(arg->AsDoubleUnchecked());
		}
		else if(arg->IsString()){
			msg.addStringArg(arg->AsStringUnchecked());
		}
		else if(arg->IsSymbol()){
			msg.addSymbolArg(arg->AsSymbolUnchecked());
		}
		else if(arg->IsChar()){
			msg.addCharArg(arg->AsCharUnchecked());
		}
		else if(arg->IsMidiMessage()){
			msg.addMidiMessageArg(arg->AsMidiMessageUnchecked());
		}
		else if(arg->IsBool()){
			msg.addBoolArg(arg->AsBoolUnchecked());
		}
		else if(arg->IsNil()){
			msg.addNoneArg();
		}
		else if(arg->IsInfinitum()){
			msg.addTriggerArg();
		}
		else if(arg->IsTimeTag()){
			msg.addTimetagArg(arg->AsTimeTagUnchecked());
		}
		else if(arg->IsRgbaColor()){
			msg.addRgbaColorArg(arg->AsRgbaColorUnchecked());
		}
		else if(arg->IsBlob()){
			const char * dataPtr;
			_PDSPosc_bundle_element_size_t len = 0;
			arg->AsBlobUnchecked((const void*&)dataPtr, len);
			ofBuffer buffer(dataPtr, len);
			msg.addBlobArg(buffer);
		}
		else {
			ofLogError("ofxOscReceiver") << "ProcessMessage(): argument in message "
				<< m.AddressPattern() << " is an unknown type "
				<< (int) arg->TypeTag() << " '" << (char) arg->TypeTag() << "'";
				break;
		}
	}

    int write = index->load() +1;
    if(write>=(int)circularBuffer->size()){ write = 0; } 
    circularBuffer->at(write).message = msg;
    circularBuffer->at(write).timepoint = std::chrono::high_resolution_clock::now();

    index->store( write );   
}
