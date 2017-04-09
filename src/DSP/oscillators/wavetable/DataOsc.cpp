
#include "DataOsc.h"

pdsp::DataOsc::DataOsc(){
    dataTable = nullptr;
}

pdsp::DataOsc::DataOsc(const pdsp::DataOsc & other) : DataOsc(){
    std::cout<<"[pdsp] warning! DataOsc copy constructed\n"; 
    pdsp_trace();
}
 
pdsp::DataOsc& pdsp::DataOsc::operator=(const DataOsc& other){
    return *this;
    std::cout<<"[pdsp] warning! DataOsc moved constructed\n";
    pdsp_trace();
}

void pdsp::DataOsc::setTable(DataTable& dataTable){
    this->dataTable = &dataTable;
}

void pdsp::DataOsc::prepareOscillator(double sampleRate) {}

void pdsp::DataOsc::oscillate(float* outputBuffer, const float* phaseBuffer, int bufferSize) noexcept {
    
    assert(dataTable!=nullptr && dataTable->buffer!=nullptr && "wavetable not set!!!");

	dataTable->update();
                
    for (int n = 0; n < bufferSize; ++n){
        float index_f = phaseBuffer[n] * static_cast<float>(dataTable->length);
        int index_i = static_cast<int>(index_f);
        float fract = index_f - index_i;
        outputBuffer[n] = interpolate_linear(dataTable->buffer[index_i], dataTable->buffer[index_i + 1], fract);
    }
    
}
    
