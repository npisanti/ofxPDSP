
#include "OscInput.h"

#define OFXPDSP_OSCINPUT_MESSAGERESERVE 128
#define OFXPDSP_OSCCIRCULARBUFFER_SIZE 10000

pdsp::osc::Input::OscChannel::OscChannel(){
    
    key = "";
    messageBuffer = nullptr;
    gate_out = nullptr;
    value_out = nullptr;
    argument = 0;
    hasParser = false;
    
    code = [](float input) noexcept{
        return input;
    };
}

void pdsp::osc::Input::OscChannel::deallocate(){
    if( messageBuffer != nullptr ){
        delete messageBuffer;
    }
    if( gate_out != nullptr ) {
        delete gate_out;
    }
    if ( value_out != nullptr ) {
        delete value_out;
    }            
}


pdsp::osc::Input::Input() {
    
    oscChannels.clear();
    
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
}   


pdsp::osc::Input::~Input(){
    if(connected){
        close();
    }
    
    for (size_t i = 0; i < oscChannels.size(); ++i){
        oscChannels[i]->deallocate();
        delete oscChannels[i];
        oscChannels[i] = nullptr;
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
    bufferChrono = chrono::high_resolution_clock::now();
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

pdsp::SequencerGateOutput& pdsp::osc::Input::out_trig( string oscAddress, int argument ) {
    
    for ( OscChannel* & osc : oscChannels ){
        if( osc->key == oscAddress && osc->argument == argument ) {
            if( osc->gate_out != nullptr ){
                return *(osc->gate_out);
            }else if( osc->value_out != nullptr ){
                cout<<"[pdsp] warning! this osc string and argument was already used as value output, returning dummy gate output\n";
                pdsp::pdsp_trace();
                return invalidGate;
            } else {
                osc->messageBuffer = new pdsp::MessageBuffer();
                osc->gate_out = new pdsp::SequencerGateOutput();
                osc->gate_out->link( *(osc->messageBuffer) );
                return *(osc->gate_out);
            }
        }
    }
    
    // not found
    OscChannel* osc = new OscChannel();
    osc->key = oscAddress;
    osc->argument = argument;
    osc->messageBuffer = new pdsp::MessageBuffer();
    osc->gate_out = new pdsp::SequencerGateOutput();
    osc->gate_out->link( *(osc->messageBuffer) );
    oscChannels.push_back(osc);

    return *(osc->gate_out);
}


pdsp::SequencerValueOutput& pdsp::osc::Input::out_value( string oscAddress, int argument ) {
   
    for ( OscChannel* & osc : oscChannels ){
        if( osc->key == oscAddress  && osc->argument == argument ) {
            if( osc->value_out != nullptr ){
                return *(osc->value_out);
            }else if( osc->gate_out != nullptr ){
                    cout<<"[pdsp] warning! this osc string and argument was already used as gate output, returning dummy value output\n";
                    pdsp::pdsp_trace();
                    return invalidValue;                    
            }else{
                osc->messageBuffer = new pdsp::MessageBuffer();
                osc->value_out = new pdsp::SequencerValueOutput();
                osc->value_out->link( *(osc->messageBuffer) );
                return *(osc->value_out);
            }
        }
    }

    // not found
    OscChannel* osc = new OscChannel();
    osc->key = oscAddress;
    osc->argument = argument;
    osc->messageBuffer = new pdsp::MessageBuffer();
    osc->value_out = new pdsp::SequencerValueOutput();
    osc->value_out->link( *(osc->messageBuffer) );
    oscChannels.push_back(osc);

    return *(osc->value_out);    
}

std::function<float(float)> & pdsp::osc::Input::parser( string oscAddress, int argument ){
    for ( OscChannel* & osc : oscChannels ){
        if( osc->key == oscAddress  && osc->argument == argument ) {
            osc->hasParser = true; 
            return osc->code;
        }
    }
    
    // not found, creating
    OscChannel* osc = new OscChannel();
    osc->key = oscAddress;
    osc->argument = argument;
    osc->hasParser = true; 
    oscChannels.push_back(osc);
    return osc->code;
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
        for (size_t i = 0; i < oscChannels.size(); ++i){
            if( oscChannels[i]->messageBuffer != nullptr ){
                oscChannels[i]->messageBuffer->clearMessages();

                if(sendClearMessages){
                    oscChannels[i]->messageBuffer->addMessage(0.0f, 0);
                }                
            }
        }
        
        // adds the messages to the buffers, only the first arg of each osc message is read, as float
        for(_PositionedOscMessage &osc : readVector){
            int ma = osc.message.getNumArgs();
            for( int a=0; a<ma; ++a ){
                for (size_t i = 0; i < oscChannels.size(); ++i){
                    
                    if(osc.message.getAddress() == oscChannels[i]->key && oscChannels[i]->argument == a ){
                        float value = 0.0f;
                        switch( osc.message.getArgType( a ) ){
                            case OFXOSC_TYPE_INT32:
                                value = osc.message.getArgAsInt32(oscChannels[i]->argument);
                            break;
                            
                            case OFXOSC_TYPE_FLOAT:
                                value = osc.message.getArgAsFloat(oscChannels[i]->argument);
                            break;

                            case OFXOSC_TYPE_TRUE:
                                value = 1.0f;
                            break;
                            
                            case OFXOSC_TYPE_FALSE:
                                value = 0.0f;
                            break;
                            
                            case OFXOSC_TYPE_STRING:
                                // try to parse string
                                value = ofToFloat(osc.message.getArgAsString(oscChannels[i]->argument));
                            break;
                            
                            default: break;
                        }
                        
                        if( oscChannels[i]->hasParser ){
                            value = oscChannels[i]->code( value );
                        }
                        if( oscChannels[i]->messageBuffer != nullptr && value != Ignore ){
                            oscChannels[i]->messageBuffer->addMessage( value, osc.sample );
                        }
                    }
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
        for (size_t i = 0; i < oscChannels.size(); ++i){
            if( oscChannels[i]->messageBuffer != nullptr ){
                oscChannels[i]->messageBuffer->processDestination(bufferSize);
            }   
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
