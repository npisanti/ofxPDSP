
#include "SampleBuffer.h"
#include <iostream>

pdsp::SampleBuffer::SampleBuffer()  {
    filePath = "file not loaded";
    buffer = nullptr;
    channels = 0;
    length = 0;
    fileSampleRate = 11050.0f;
    verbose = false;
    mono = 0;
}


pdsp::SampleBuffer::~SampleBuffer(){
    if(buffer!=nullptr){
        unLoad();
    }
}

void pdsp::SampleBuffer::setVerbose(bool verbose){
    this->verbose = verbose;
}
        
void pdsp::SampleBuffer::setMonoChannel(int monoChannelIndex){
    if (monoChannelIndex<0) {
        mono = 0;
    }else if(monoChannelIndex>=channels){
        mono = channels-1;
    }
}


void pdsp::SampleBuffer::unLoad(){

    for(int i=0; i<channels; ++i){
        ofx_deallocate_aligned(buffer[i]);
    }
    delete [] buffer;
    
    filePath = "file not loaded";
    buffer = nullptr;
    channels = 0;
    length = 0;
    fileSampleRate = 11050.0f;
    //ratesRatio = 0.0f;
}

bool pdsp::SampleBuffer::loaded(){
	if (buffer != nullptr) {
		return true;
	}
	else {
		return false;
	}
}


void pdsp::SampleBuffer::load( float* interleavedBuffer, double sampleRate, int channelLength, int channels ){
    
    float ** toReturn;
    toReturn = new float*[channels];
    
    for(int i=0; i<channels; ++i){
        ofx_allocate_aligned(toReturn[i], channelLength+1); //guard point for linear interpolation
        
        if(toReturn[i] == nullptr){
            for(int j = 0; j<i; j++){ //deallocate already allocated vectors
                ofx_deallocate_aligned(toReturn[j]);
            }
            delete [] toReturn;
            std::string error = "[pdsp] memory low, impossible to load sample\n";
            if (verbose) std::cout << "[pdsp] memory low, impossible to load sample\n";
            this->filePath = error;
            return; //abort
        }
    }
    
    if(verbose) std::cout <<"[pdsp] done array allocations for file memorization\n";

    for(int n=0; n<channelLength; ++n){
        for(int k=0; k<channels; ++k){
            toReturn[k][n] = interleavedBuffer[n*channels + k];
        }
    }
    if(verbose) std::cout <<"[pdsp] copying file to SampleBuffer\n";
    
    
    if(buffer!=nullptr){ // changing samples
            unLoad();
    }
    
    this->buffer = toReturn;
    this->channels = channels;
    this->length = channelLength;
    this->fileSampleRate = sampleRate;

    
    if(verbose) std::cout <<"[pdsp] file correctly loaded into SampleBuffer\n";
    if(verbose) std::cout << "[pdsp] sample rate: "<<this->fileSampleRate<<" | length: "<<this->length<<" | channels: "<<this->channels<<"\n";
}



void pdsp::SampleBuffer::load( std::string filePath ){
    
#if (defined(PDSP_USE_LIBSNDFILE) || defined(__linux))
   
    const char * filePath_c = filePath.c_str();  
 
    SndfileHandle fileHandle = SndfileHandle(filePath_c); 
    
    if(verbose) std::cout<< "[pdps] loading audio file: "<<filePath<<"\n";
    
    if( strcmp(fileHandle.strError(), "No Error.") ==0 ){
        
        double sampleRate = fileHandle.samplerate();
        int waveLength = int(fileHandle.frames());
        int channels = fileHandle.channels();
       
        float* samples;
        
        try
        {
            samples = new float [channels * waveLength];
        }
        catch (std::bad_alloc& ba)
        {
            std::string error = "error loading file: bad_alloc caught: ";
            error += ba.what();
            error +="\n";
            if(verbose) std::cout<<"[pdsp]"<< error<<"\n";
            this->filePath = error;
            return; //abort
        }
        
        fileHandle.read (samples, channels * waveLength);
    
        this->load(samples, sampleRate, waveLength, channels);
        this->filePath = filePath;
        
        delete [] samples;

    }else{
        std::string error =  std::string(fileHandle.strError());
        if(verbose) std::cout<<"[pdsp] "<<error<<"\n";
        this->filePath = error;
    }

#elif defined(__APPLE__) 
    
    AudioDecoder * fileHandle = new AudioDecoder(filePath);
    
    int loadState = fileHandle->open();
    
    if(loadState == AUDIODECODER_OK){
        double sampleRate = static_cast<double>(fileHandle->sampleRate());

        std::cout<< " testing channels:"<<fileHandle->channels()<<"\n";
        std::cout<< " testing length:"<<fileHandle->numSamples()<<"\n";
        int channels = fileHandle->channels();
        int waveLength = fileHandle->numSamples() / channels;
        
        float* samples;
        
        try
        {
            samples = new float [channels * waveLength];
        }
        catch (std::bad_alloc& ba)
        {
            std::string error = "error loading file: bad_alloc caught: ";
            error += ba.what();
            error +="\n";
            if(verbose) std::cout<<"[pdsp]"<< error<<"\n";
            this->filePath = error;
            delete fileHandle;
            return; //abort
        }
        
        fileHandle->read ( channels * waveLength, samples);
        
        this->load(samples, sampleRate, waveLength, channels);
        this->filePath = filePath;
        
        delete [] samples;
        
    }else{
        std::string error =  "error opening file";
        if(verbose) std::cout<<"[pdsp] "<<error<<"\n";
        this->filePath = error;
        delete fileHandle;
        return;
    }
    
#else 

    std::cout<<"[pdsp] loading file unsuccessful, libsndfile.h not included when compiling\n";
	std::cout << "[pdsp] if you want direct load function, #define PDSP_USE_LIBSNDFILE in flags.h and then include and link libsndfile in your project\n";
    this->filePath = "impossible to load file";
    
#endif
    
}
