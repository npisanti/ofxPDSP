
#include "BufferShell.h"

pdsp::BufferShell::BufferShell(){
        buffer = nullptr;
        overSample = 1;

        if(dynamicConstruction){
                prepareToPlay(globalBufferSize, globalSampleRate);
        }
}

pdsp::BufferShell::BufferShell(const BufferShell& other) : BufferShell() {
        if(other.overSample!=this->overSample){
                setOversampleLevel(other.overSample);
        }
}

pdsp::BufferShell& pdsp::BufferShell::operator=(const pdsp::BufferShell &other){
        if(other.overSample!=this->overSample){
                setOversampleLevel(other.overSample);
        }
        return *this;
}

pdsp::BufferShell::~BufferShell(){
        releaseResources();
}


void pdsp::BufferShell::setOversampleLevel(int overSample){
        this->overSample = overSample;
    
        if (buffer!=nullptr) {
                ofx_deallocate_aligned(buffer);
                ofx_allocate_aligned(buffer, (globalBufferSize*overSample*3)/2);
        }
}


void pdsp::BufferShell::prepareToPlay(int expectedBufferSize, double sampleRate){
        if(buffer!=nullptr){
                ofx_deallocate_aligned(buffer);
        }
        ofx_allocate_aligned(buffer, (expectedBufferSize*overSample*PDSP_BUFFERS_EXTRA_DIM)/(PDSP_BUFFERS_EXTRA_DIM-1));
}

void pdsp::BufferShell::releaseResources(){
        if(buffer!=nullptr){
                ofx_deallocate_aligned(buffer);
        }
}
