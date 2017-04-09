
#include "FFTWorker.h"



pdsp::FFTWorker::FFTWorker(){
    lastBufferSize = -1;
    complexSize = 0;
    blockSize = 0;
    fftImplementation = new audiofft::AudioFFT();
}

pdsp::FFTWorker::~FFTWorker(){

    delete fftImplementation;     
    fftImplementation = nullptr;   

}

void pdsp::FFTWorker::initFFT(int bufferSize){
    
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

int pdsp::FFTWorker::getFFTComplexSize() const{
        return complexSize;
}
  
int pdsp::FFTWorker::getFFTBlockSize() const{
        return blockSize;
}


void pdsp::FFTWorker::FFT (const float* inSignal, float* re, float* im){
        fftImplementation->fft(inSignal, re, im);
}

void pdsp::FFTWorker::iFFT (float* outSignal, const float* re, const float* im){
        fftImplementation->ifft(outSignal, re, im);
}
