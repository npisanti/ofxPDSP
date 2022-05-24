
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

pdsp::SampleBuffer::SampleBuffer(const pdsp::SampleBuffer& other) {
    init( other.length, other.channels );
    
    if( buffer!=nullptr || other.buffer==nullptr ){
        this->filePath = other.filePath;
        this->channels = other.channels;
        this->length = other.length;
        this->fileSampleRate = other.fileSampleRate;
        this->verbose = other.verbose;
        this->mono = other.mono;
        
        for(int i=0; i<channels; ++i){
            for( int n=0; n<length; ++n ){
                this->buffer[i][n] = other.buffer[i][n];
            }
        }
    }
}

pdsp::SampleBuffer& pdsp::SampleBuffer::operator= (const pdsp::SampleBuffer& other) {
    unLoad();
  
    init( other.length, other.channels );
    
    if( buffer!=nullptr || other.buffer==nullptr ){
        this->filePath = other.filePath;
        this->channels = other.channels;
        this->length = other.length;
        this->fileSampleRate = other.fileSampleRate;
        this->verbose = other.verbose;
        this->mono = other.mono;
        
        for(int i=0; i<channels; ++i){
            for( int n=0; n<length; ++n ){
                this->buffer[i][n] = other.buffer[i][n];
            }
        }
    }
    return *this;
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


void pdsp::SampleBuffer::load( float* interleavedBuffer, double sampleRate, long channelLength, int channels ){
    
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
    for(int k=0; k<channels; ++k){
        toReturn[k][channelLength] = 0.0f; // set guard point to zero
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
    
    if(verbose) std::cout<< "[pdsp] loading audio file: "<<filePath<<"\n";
    
    ofxAudioFile audiofile;
    audiofile.setVerbose( verbose );
    audiofile.load( filePath ); 
    
    if( audiofile.loaded() ){
        double sampleRate = audiofile.samplerate();
        long waveLength = audiofile.length();
        int channels = audiofile.channels();
        this->load( audiofile.data(), sampleRate, waveLength, channels);
        this->filePath = filePath;
    }else{
        std::string error =  "error loading sample";
        if(verbose) std::cout<<"[pdsp] "<<error<<"\n";
        this->filePath = error;
    }

    audiofile.free();
    
}


void pdsp::SampleBuffer::init( long tableLen, int numTables ){
    
    float ** newTables;
    newTables = new float*[numTables];
    
    for(int i=0; i<numTables; ++i){
        ofx_allocate_aligned(newTables[i], tableLen+1); //guard point for linear interpolation
        
        if(newTables[i] == nullptr){
            for(int j = 0; j<i; j++){ //deallocate already allocated vectors
                ofx_deallocate_aligned(newTables[j]);
            }
            delete [] newTables;
            std::string error = "[pdsp] memory low, impossible to init tables\n";
            if (verbose) std::cout << error;
            this->filePath = error;
            return; //abort
        }
    }
    
    if(verbose) std::cout <<"[pdsp] done array allocations for SampleBuffer init\n";
   
    for(int i=0; i<numTables; ++i){
        for(long n=0; n<=tableLen; ++n){
            newTables[i][n] = 0.0f;
        }       
    }

    if(verbose) std::cout <<"[pdsp] SampleBuffer buffer initialized\n";  
    
    this->buffer = newTables;
    this->channels = numTables;
    this->length = tableLen;
    this->fileSampleRate = Preparable::getGlobalSampleRate();
    
    std::string info = std::to_string(tableLen);
    info += " samples buffer\n";
    this->filePath = info;
}


void pdsp::SampleBuffer::normalize( ){
    
    if (buffer!=nullptr){
        
        double max = -1.0f;
        for(int c=0; c<channels; ++c){
            for(long n=0; n<length; ++n){
                double absolute = std::abs(buffer[c][n]);
                if( absolute > max){
                    max = absolute;
                }
            }
        }

        float scale = 1.0 / max;
        
        for(int c=0; c<channels; ++c){
            ofx_Aeq_BmulS( buffer[c], buffer[c], scale, length);
        }
        
    }else{
        std::cout <<"[pdsp] impossible to normalize, sample buffer empty\n";
        pdsp_trace();
    }
    
}
