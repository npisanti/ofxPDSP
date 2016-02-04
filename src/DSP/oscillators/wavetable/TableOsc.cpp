
#include "TableOsc.h"



pdsp::TableOsc::TableOsc(){
    waveTable = nullptr;
}

void pdsp::TableOsc::setTable(SampleBuffer* waveTable){
    this->waveTable = waveTable;
}

void pdsp::TableOsc::prepareOscillator( double sampleRate ) {}
void pdsp::TableOsc::releaseOscillator() {}

void pdsp::TableOsc::oscillate( float* outputBuffer, const float* phaseBuffer, int sizeToProcess, int bufferSize) noexcept{
    
    assert(waveTable!=nullptr && waveTable->buffer!=nullptr && "wavetable not set!!!");
                
    for (int n = 0; n < sizeToProcess; ++n){
        
        float index_f = phaseBuffer[n] * static_cast<float>(waveTable->length);
        int index_i = static_cast<int>(index_f);
        float fract = index_f - index_i;
        outputBuffer[n] = interpolate_linear(waveTable->buffer[0][index_i], waveTable->buffer[0][index_i + 1], fract);
        
    }

}
