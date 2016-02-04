

#include "GrainTable.h"


pdsp::GrainTable::GrainTable(){
    buffer = nullptr;
    length = 0;
}

pdsp::GrainTable::GrainTable(const GrainTable& other) : GrainTable(){
    if(other.buffer!=nullptr){
        setTable(other.type, other.length);
    }
}

pdsp::GrainTable& pdsp::GrainTable::operator= (const GrainTable& other){
    if(other.buffer!=nullptr){
        setTable(other.type, other.length);
    }else{
        if(buffer!=nullptr){
            length=0;
            ofx_deallocate_aligned(buffer);
        }
    }
    return *this;
}

pdsp::GrainTable::~GrainTable(){
    if(buffer!=nullptr){
        ofx_deallocate_aligned(buffer);
    }
}


void pdsp::GrainTable::setTable(Window_t type, int length){
    this->type = type;
    this->length = length;
    this->length_f = static_cast<float>(length);
    
    buffer = window( type, length);
    int half = length/2;
    for (int i = half; i<length+1; ++i) { //lenght +1 so we init also the guard point
        buffer[i] = 1.0f - buffer[i-half];
    }
     
}

bool pdsp::GrainTable::empty(){
    return (buffer==nullptr);
}
