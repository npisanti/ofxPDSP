
#include "UsesFFT.h"


audiofft::AudioFFTBase* pdsp::UsesFFT::fftImplementation = nullptr;
int pdsp::UsesFFT::lastBufferSize = -1;
int pdsp::UsesFFT::unitsCount = 0;
int pdsp::UsesFFT::complexSize = 0;
int pdsp::UsesFFT::blockSize = 0;


pdsp::UsesFFT::UsesFFT(){
    if(unitsCount == 0){
        fftImplementation = new audiofft::OouraFFT();
    }
    unitsCount++;
}

pdsp::UsesFFT::~UsesFFT(){
    if(unitsCount == 1){
        delete fftImplementation;     
        fftImplementation = nullptr;   
    }
    unitsCount--;
}

void pdsp::UsesFFT::initFFT(int bufferSize){
    
    if(fftImplementation != nullptr && bufferSize!=lastBufferSize){
        
        lastBufferSize = bufferSize;

        int signalBlockSize = 4;
        while(signalBlockSize<=lastBufferSize){
            signalBlockSize *=2;
        }

        fftImplementation->init(signalBlockSize);
        complexSize = audiofft::AudioFFT::ComplexSize(signalBlockSize);
        blockSize = signalBlockSize;
    }

}

int pdsp::UsesFFT::getFFTComplexSize() const{
        return complexSize;
}
  
int pdsp::UsesFFT::getFFTBlockSize() const{
        return blockSize;
}


void pdsp::UsesFFT::FFT (const float* inSignal, float* re, float* im){
        fftImplementation->fft(inSignal, re, im);
}

void pdsp::UsesFFT::iFFT (float* outSignal, const float* re, const float* im){
        fftImplementation->ifft(outSignal, re, im);
}
