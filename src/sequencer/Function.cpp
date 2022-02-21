
#include "Function.h"

#include <string>

pdsp::SequencerGateOutput pdsp::Function::invalidGate = pdsp::SequencerGateOutput();

pdsp::SequencerValueOutput pdsp::Function::invalidValue = pdsp::SequencerValueOutput();

std::vector<pdsp::Function*> pdsp::Function::instances = std::vector<pdsp::Function*>();

pdsp::Function::Function(){
    sample = 0;
    timing = 16;
    active = true;
    seed = rand();
    
    instances.push_back( this );
}

pdsp::Function::~Function(){
    for( std::size_t i=0; i<instances.size(); ++i ){
        if( instances[i] == this ){
            instances.erase(instances.begin() + i);
            break;
        }
    }
}

pdsp::SequencerGateOutput& pdsp::Function::out_trig( std::string name ){
    
    for( std::size_t i=0; i<outputs.size(); ++i ){
        if( outputs[i].name == name ){
            if(outputs[i].value_out!=nullptr){
                std::cout<<"[pdsp] warning! this lambda output name was already used as value output, returning dummy gate output\n";
                pdsp::pdsp_trace();
                return invalidGate;
            }else{
                return *(outputs[i].gate_out);
            }
        }
    }
    
    outputs.emplace_back();
    outputs.back().name = name;
    outputs.back().number = -1;
    outputs.back().messageBuffer = new pdsp::MessageBuffer();
    outputs.back().gate_out = new pdsp::SequencerGateOutput();
    outputs.back().gate_out->link( *(outputs.back().messageBuffer) );
    
    return *(outputs.back().gate_out);
    
}

pdsp::SequencerGateOutput& pdsp::Function::out_trig( int number ){
    if( number < 0 ){
        std::cout<<"[pdsp] warning! pdsp::Function number id given is less than 0, returning dummy gate output\n";
        pdsp::pdsp_trace();
        return invalidGate;
    }
    
    for( std::size_t i=0; i<outputs.size(); ++i ){
        if( outputs[i].number == number ){
            if(outputs[i].value_out!=nullptr){
                std::cout<<"[pdsp] warning! this lambda output number id was already used as value output, returning dummy gate output\n";
                pdsp::pdsp_trace();
                return invalidGate;
            }else{
                return *(outputs[i].gate_out);
            }
        }
    }
    
    outputs.emplace_back();
    outputs.back().name = std::to_string( number );
    outputs.back().number = number;
    outputs.back().messageBuffer = new pdsp::MessageBuffer();
    outputs.back().gate_out = new pdsp::SequencerGateOutput();
    outputs.back().gate_out->link( *(outputs.back().messageBuffer) );
    
    return *(outputs.back().gate_out);
}

pdsp::SequencerValueOutput& pdsp::Function::out_value( std::string name ){
    for( std::size_t i=0; i<outputs.size(); ++i ){
        if( outputs[i].name == name ){
            if(outputs[i].gate_out!=nullptr){
                std::cout<<"[pdsp] warning! this lambda output name was already used as value output, returning dummy gate output\n";
                pdsp::pdsp_trace();
                return invalidValue;
            }else{
                return *(outputs[i].value_out);
            }
        }
    }
    
    outputs.emplace_back();
    outputs.back().name = name;
    outputs.back().number = -1;
    outputs.back().messageBuffer = new pdsp::MessageBuffer();
    outputs.back().value_out = new pdsp::SequencerValueOutput();
    outputs.back().value_out->link( *(outputs.back().messageBuffer) );
    
    return *(outputs.back().value_out);
}

pdsp::SequencerValueOutput& pdsp::Function::out_value( int number ){
    if( number < 0 ){
        std::cout<<"[pdsp] warning! pdsp::Function number id given is less than 0, returning dummy gate output\n";
        pdsp::pdsp_trace();
        return invalidValue;
    }
    
    for( std::size_t i=0; i<outputs.size(); ++i ){
        if( outputs[i].number == number ){
            if(outputs[i].gate_out!=nullptr){
                std::cout<<"[pdsp] warning! this lambda output number id was already used as value output, returning dummy gate output\n";
                pdsp::pdsp_trace();
                return invalidValue;
            }else{
                return *(outputs[i].value_out);
            }
        }
    }
    
    outputs.emplace_back();
    outputs.back().name = std::to_string(number);
    outputs.back().number = number;
    outputs.back().messageBuffer = new pdsp::MessageBuffer();
    outputs.back().value_out = new pdsp::SequencerValueOutput();
    outputs.back().value_out->link( *(outputs.back().messageBuffer) );
    
    return *(outputs.back().value_out);
}

void pdsp::Function::send( std::string name, float value ) noexcept{
    for( std::size_t i=0; i<outputs.size(); ++i ){
        if( outputs[i].name == name ){
            outputs[i].messageBuffer->addMessage( value, sample );
            return;
        }
    }
    std::cout<<"[pdsp] warning! sending message to non-existent output into pdsp::Function.code\n";
    pdsp::pdsp_trace();
}

void pdsp::Function::send( int number, float value ) noexcept{
    for( std::size_t i=0; i<outputs.size(); ++i ){
        if( outputs[i].number == number ){
            outputs[i].messageBuffer->addMessage( value, sample );
            return;
        }
    }
    std::cout<<"[pdsp] warning! sending message to non-existent output into pdsp::Function.code\n";
    pdsp::pdsp_trace();
}

void pdsp::Function::process( double playhead, double barsPerSample, int bufferSize ){
    
    for(std::size_t i=0; i<outputs.size(); ++i ){
        if( outputs[i].messageBuffer != nullptr ){
            outputs[i].messageBuffer->clearMessages();
        }              
    } 

    if( active ){
        double t = timing;
        double f = playhead * t; 
        double step = barsPerSample * t;
        
        // this loop could probably be unrolled 
        for( int i=0; i<bufferSize; ++i ){
            double next = int( f + 0.5 );
            double delta = f-next;
            if ( delta >= 0.0 && delta < step ){
                sample = i;
                fi = f;
                code();
            }
            f += step;
        }
    }
    
    for(std::size_t i=0; i<outputs.size(); ++i ){
        if( outputs[i].messageBuffer != nullptr ){
            outputs[i].messageBuffer->processDestination( bufferSize );
        }              
    } 
    sample = 0;
}


void pdsp::Function::clear( int bufferSize ){
    for(std::size_t i=0; i<outputs.size(); ++i ){
        if( outputs[i].messageBuffer != nullptr ){
            outputs[i].messageBuffer->clearMessages();
        }              
    } 
    for(std::size_t i=0; i<outputs.size(); ++i ){
        if( outputs[i].messageBuffer != nullptr ){
            outputs[i].messageBuffer->processDestination( bufferSize );
        }              
    } 
}

// 32766.f is the MAX - 1

bool pdsp::Function::chance( float probablility ){
    int t = probablility * 32767.f;
    int r = fastrand();
    return (r<t) ? true : false;
}

int pdsp::Function::dice( int max ){
    int r = fastrand();
    return r%max;
}

float pdsp::Function::urand(){
    int r = fastrand();
    float fr = float(r) / 32766.0f;
    return fr;
}

float pdsp::Function::brand(){
    int r = fastrand();
    float fr = float(r) / 32766.0f;
    return (fr*2.0f)-1.0f;
}
