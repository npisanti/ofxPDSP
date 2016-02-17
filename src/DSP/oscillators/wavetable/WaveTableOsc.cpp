
#include "WaveTableOsc.h"



pdsp::WaveTableOsc::WaveTableOsc(){
    waveTable = nullptr;
}

void pdsp::WaveTableOsc::setTable(SampleBuffer* waveTable){
    this->waveTable = waveTable;
    input_shape.enableBoundaries(0.0f, waveTable->channels-1);
}


void pdsp::WaveTableOsc::oscillateShapeCR(float* outputBuffer, const float* phaseBuffer, const float shape, int bufferSize) noexcept {
    
    assert(waveTable!=nullptr && waveTable->buffer!=nullptr && "wavetable not set!!!");
           
    int shape_i = static_cast<int>(shape);
    float shape_fract = shape - shape_i;
    int shapeB = shape_i +1;
                
    for (int n = 0; n < bufferSize; ++n){
        
        float index_f = phaseBuffer[n] * static_cast<float>(waveTable->length);
        int index_i = static_cast<int>(index_f);
        float fract = index_f - index_i;
        
        float waveA = interpolate_linear(waveTable->buffer[shape_i][index_i], waveTable->buffer[shape_i][index_i + 1], fract);
        float waveB = interpolate_linear(waveTable->buffer[shapeB][index_i], waveTable->buffer[shapeB][index_i + 1], fract);
        
        outputBuffer[n] = interpolate_linear(waveA, waveB, shape_fract);
        
    }

}



void pdsp::WaveTableOsc::oscillateShapeAR(float* outputBuffer, const float* phaseBuffer, const float* shapeBuffer, int bufferSize) noexcept {
   
    assert(waveTable!=nullptr && waveTable->buffer!=nullptr && "wavetable not set!!!");
           
    for (int n = 0; n < bufferSize; ++n){
        
        float shape = shapeBuffer[n];
        int shape_i = static_cast<int>(shape);
        float shape_fract = shape - shape_i;
        int shapeB = shape_i +1;
        
        float index_f = phaseBuffer[n] * static_cast<float>(waveTable->length);
        int index_i = static_cast<int>(index_f);
        float fract = index_f - index_i;
        
        float waveA = interpolate_linear(waveTable->buffer[shape_i][index_i], waveTable->buffer[shape_i][index_i + 1], fract);
        float waveB = interpolate_linear(waveTable->buffer[shapeB][index_i], waveTable->buffer[shapeB][index_i + 1], fract);
        
        outputBuffer[n] = interpolate_linear(waveA, waveB, shape_fract);
        
    }
    
}
