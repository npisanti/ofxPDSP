
#include "OscParser.h"

pdsp::SequencerGateOutput pdsp::osc::OscParser::invalidGate = pdsp::SequencerGateOutput();

pdsp::SequencerValueOutput pdsp::osc::OscParser::invalidValue = pdsp::SequencerValueOutput();

pdsp::osc::OscParser::OscChannel::OscChannel(){
    messageBuffer = nullptr;
    gate_out = nullptr;
    value_out = nullptr;
    hasParser = false;
}

void pdsp::osc::OscParser::OscChannel::deallocate(){
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
    
pdsp::osc::OscParser::OscParser(){
    address = "";
}
    
    
pdsp::osc::OscParser::~OscParser(){    
    for (size_t i = 0; i < channels.size(); ++i){
        channels[i]->deallocate();
        delete channels[i];
        channels[i] = nullptr;
    }
}

pdsp::SequencerGateOutput& pdsp::osc::OscParser::out_trig( int out ){
    
    if( out < (int) channels.size() ){
        if( channels[out]->messageBuffer!=nullptr){
            if(channels[out]->value_out!=nullptr){
                cout<<"[pdsp] warning! this osc string and argument was already used as value output, returning dummy gate output\n";
                pdsp::pdsp_trace();
                return invalidGate;
            }else if(channels[out]->gate_out!=nullptr){
                return *(channels[out]->gate_out);
            }
        }
    }else{
        size_t oldsize = channels.size();
        channels.resize( out+1 );
        for( size_t i=oldsize; i<channels.size(); ++i ){
            channels[i] = new OscChannel();
        }
    }
    
    channels[out]->messageBuffer = new pdsp::MessageBuffer();
    channels[out]->gate_out = new pdsp::SequencerGateOutput();
    channels[out]->gate_out->link( *(channels[out]->messageBuffer) );
    
    return *(channels[out]->gate_out);
}

pdsp::SequencerValueOutput& pdsp::osc::OscParser::out_value( int out ){
    
    if( out < (int) channels.size() ){
        if( channels[out]->messageBuffer!=nullptr){
            if(channels[out]->gate_out!=nullptr){
                cout<<"[pdsp] warning! this osc string and argument was already used as gate output, returning dummy value output\n";
                pdsp::pdsp_trace();
                return invalidValue;  
            }else if(channels[out]->value_out!=nullptr){
                return *(channels[out]->value_out);
            }
        }
    }else{
        size_t oldsize = channels.size();
        channels.resize( out+1 );
        for( size_t i=oldsize; i<channels.size(); ++i ){
            channels[i] = new OscChannel();
        }
    }
    
    channels[out]->messageBuffer = new pdsp::MessageBuffer();
    channels[out]->value_out = new pdsp::SequencerValueOutput();
    channels[out]->value_out->link( *(channels[out]->messageBuffer) );
    
    return *(channels[out]->value_out);
}

std::function<float(float)> & pdsp::osc::OscParser::parser( int argument ){
    if( argument >= (int) channels.size() ){
        size_t oldsize = channels.size();
        channels.resize( argument+1 );
        for( size_t i=oldsize; i<channels.size(); ++i ){
            channels[i] = new OscChannel();
        }
    }
    channels[argument]->hasParser = true;
    return channels[argument]->code;
}

void pdsp::osc::OscParser::process( ofxOscMessage msg, int sample ){
    int ma = msg.getNumArgs();
    
    for( int a=0; a<ma; ++a ){
        bool bParse = channels[a]->hasParser;
        bool bAssigned = (channels[a]->messageBuffer != nullptr);
        
        if( bParse || bAssigned ){
            float value = 0.0f;
            
            switch( msg.getArgType( a ) ){
                case OFXOSC_TYPE_INT32:
                    value = msg.getArgAsInt32(a);
                break;
                
                case OFXOSC_TYPE_FLOAT:
                    value = msg.getArgAsFloat(a);
                break;

                case OFXOSC_TYPE_TRUE:
                    value = 1.0f;
                break;
                
                case OFXOSC_TYPE_FALSE:
                    value = 0.0f;
                break;
                
                case OFXOSC_TYPE_STRING:
                    // try to parse string
                    value = ofToFloat(msg.getArgAsString(a));
                break;
                
                default: break;
            }
            
            if( bParse ){
                value = channels[a]->code( value );
            }
            
            if( bAssigned && value != Ignore ){
                channels[a]->messageBuffer->addMessage( value, sample );
            }           
        }
    }    
}

void pdsp::osc::OscParser::clear( bool sendClearMessage ){
    for( size_t i=0; i<channels.size(); ++i ){
        if( channels[i]->messageBuffer != nullptr ){
            channels[i]->messageBuffer->clearMessages();

            if(sendClearMessage){
                channels[i]->messageBuffer->addMessage(0.0f, 0);
            }                
        }        
    }
}

void pdsp::osc::OscParser::processDestinations( int bufferSize ) {     
    for(size_t i=0; i<channels.size(); ++i ){
        if( channels[i]->messageBuffer != nullptr ){
            channels[i]->messageBuffer->processDestination(bufferSize);
        }              
    } 
}
