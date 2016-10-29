
#include "WaveTableOsc.h"


pdsp::WaveTableOsc::WaveTableOsc(){
    waveTable = nullptr;
}

pdsp::WaveTableOsc::WaveTableOsc(const pdsp::WaveTableOsc & other) : WaveTableOsc(){
    std::cout<<"[pdsp] warning! WaveTableOsc copy constructed\n"; 
    pdsp_trace();
}
 
pdsp::WaveTableOsc& pdsp::WaveTableOsc::operator=(const WaveTableOsc& other){
    return *this;
    std::cout<<"[pdsp] warning! WaveTableOsc moved constructed\n";
    pdsp_trace();
}


void pdsp::WaveTableOsc::setTable(WaveTable& waveTable){
    this->waveTable = &waveTable;
    input_shape.enableBoundaries(0.0f, (float)(waveTable.tableSize -1) );
}



void pdsp::WaveTableOsc::prepareOscillator(double sampleRate) {
    input_shape.enableBoundaries(0.0f, (float)(waveTable->tableSize -1) );    
}

void pdsp::WaveTableOsc::oscillateShapeCR(float* outputBuffer, const float* phaseBuffer, const float shape, int bufferSize) noexcept {
    
    assert(waveTable!=nullptr && waveTable->buffer!=nullptr && "wavetable not set!!!");
           
    int shapeA = static_cast<int>(shape);
    float shape_fract = shape - shapeA;
    int shapeB = shapeA +1;
                
    for (int n = 0; n < bufferSize; ++n){
        
        float index_f = phaseBuffer[n] * static_cast<float>(waveTable->length);
        int index_i = static_cast<int>(index_f);
        float fract = index_f - index_i;
        
        float waveA = interpolate_linear(waveTable->buffer[ shapeA ][index_i], waveTable->buffer[ shapeA ][index_i + 1], fract);
        float waveB = interpolate_linear(waveTable->buffer[ shapeB ][index_i], waveTable->buffer[ shapeB ][index_i + 1], fract);
        
        outputBuffer[n] = interpolate_linear(waveA, waveB, shape_fract);
        
    }

}


void pdsp::WaveTableOsc::oscillateShapeAR(float* outputBuffer, const float* phaseBuffer, const float* shapeBuffer, int bufferSize) noexcept {
   
    assert(waveTable!=nullptr && waveTable->buffer!=nullptr && "wavetable not set!!!");
           
    for (int n = 0; n < bufferSize; ++n){
        
        float shape = shapeBuffer[n];
        int shapeA = static_cast<int>(shape);
        float shape_fract = shape - shapeA;
        int shapeB = shapeA +1;
        
        float index_f = phaseBuffer[n] * static_cast<float>(waveTable->length);
        int index_i = static_cast<int>(index_f);
        float fract = index_f - index_i;
        
        float waveA = interpolate_linear(waveTable->buffer[ shapeA ][index_i], waveTable->buffer[ shapeA ][index_i + 1], fract);
        float waveB = interpolate_linear(waveTable->buffer[ shapeB ][index_i], waveTable->buffer[ shapeB ][index_i + 1], fract);
        
        outputBuffer[n] = interpolate_linear(waveA, waveB, shape_fract);
        
    }
    
}
