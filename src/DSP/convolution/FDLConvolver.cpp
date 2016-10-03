
#include "FDLConvolver.h"

pdsp::FFTWorker pdsp::FDLConvolver::fftWorker = FFTWorker();


pdsp::FDLConvolver::FDLConvolver(){
        
        addInput("signal", input);
        addOutput("signal", output);
        updateOutputNodes();
        
        signalBlock    = 0;
        complexSize    = 0;
        silenceCount   = 30000;
        processingSize = 0;
        processingOffset = 0;
                
        impulseResponse = nullptr;
        IRLoaded = false;
        IRChannel = 0;
        
        impulseR    = nullptr;
        impulseI    = nullptr;
        circularR   = nullptr;
        circularI   = nullptr;
        
        addR        = nullptr;
        addI        = nullptr;           
        paddedInput = nullptr;
        overlapAdd  = nullptr;
        
        if(dynamicConstruction){
                prepareUnit(globalBufferSize, globalSampleRate);
        }
}

pdsp::Patchable& pdsp::FDLConvolver::in_signal(){
    return in("signal");   
}

pdsp::Patchable& pdsp::FDLConvolver::out_signal(){
    return out("signal");   
}

void pdsp::FDLConvolver::prepareUnit( int expectedBufferSize, double sampleRate ) {
        this->sampleRate=sampleRate;

        fftWorker.initFFT(expectedBufferSize);

        prepareIR();
}

void pdsp::FDLConvolver::releaseResources () {
 
        deallocateBlocksBuffers(); 
            
}       


void pdsp::FDLConvolver::loadIR ( SampleBuffer* impulseResponse, int channel){
    loadIR( *impulseResponse, channel);
}

void pdsp::FDLConvolver::loadIR ( SampleBuffer & impulseResponse, int channel){
    this->IRChannel = channel;
    this->impulseResponse = &impulseResponse;
    
    if(dynamicConstruction){
        prepareIR();
    }
}


void pdsp::FDLConvolver::prepareIR ( ){
    
        IRLoaded = false;
        deallocateBlocksBuffers();
        
        if(impulseResponse!=nullptr ){
  
                int impulseLength = impulseResponse->length;
                
                //cut out zeroes from tail
                while(impulseLength>0 && (impulseResponse->buffer[IRChannel][impulseLength-1] == 0.0f )){
                        impulseLength--;
                }
  
                float inc;
                int convertedLen;
                if(sampleRate!=impulseResponse->fileSampleRate){
                        inc    =  impulseResponse->fileSampleRate / sampleRate;
                        convertedLen = static_cast<int>( static_cast<float>(impulseLength) / inc ) + 1;
                }else{
                        inc    =  1.0f;
                        convertedLen = (impulseLength);     
                }
                
                signalBlock = fftWorker.getFFTBlockSize();
                processingSize = signalBlock/2;
                complexSize = fftWorker.getFFTComplexSize();               
                numBlocks = (convertedLen / processingSize) + 1;
                
                if( allocateBlocksBuffers() && loadImpulseResponseSegments(inc, convertedLen) ){
                        IRLoaded = true;
                }else{
                        IRLoaded = false;
                }
                
                blockIndex = 0;
                
        }
}


bool pdsp::FDLConvolver::allocateBlocksBuffers(){

    if(paddedInput != nullptr){
        ofx_deallocate_aligned(paddedInput);
    }
    ofx_allocate_aligned(paddedInput, signalBlock);    
    if(paddedInput == nullptr){
        deallocateBlocksBuffers();
        return false;
    }

    if(overlapAdd != nullptr){
        ofx_deallocate_aligned(overlapAdd);
    }
    ofx_allocate_aligned(overlapAdd, signalBlock/2);
    if(overlapAdd == nullptr){
        deallocateBlocksBuffers();
        return false;
    }
    
    if(addR != nullptr){
        ofx_deallocate_aligned(addR);
    }
    ofx_allocate_aligned(addR, complexSize);
    if(addR == nullptr){
        deallocateBlocksBuffers();
        return false;
    }

    if(addI != nullptr){
        ofx_deallocate_aligned(addI);
    }
    ofx_allocate_aligned(addI, complexSize);
    if(addI == nullptr){
        deallocateBlocksBuffers();
        return false;
    }

    impulseR = new float* [numBlocks];
    for(int i=0; i<numBlocks; ++i){
        ofx_allocate_aligned(impulseR[i], complexSize);   
        if(impulseR[i]==nullptr){
            for(int j=0; j<i; j++){
                ofx_deallocate_aligned(impulseR[i]);
            }
            delete[] impulseR;
            deallocateBlocksBuffers();
            return false;
        }
    }

    impulseI = new float* [numBlocks];
    for(int i=0; i<numBlocks; ++i){
        ofx_allocate_aligned(impulseI[i], complexSize);   
        if(impulseI[i]==nullptr){
            for(int j=0; j<i; j++){
                ofx_deallocate_aligned(impulseI[i]);
            }
            delete[] impulseI;
            deallocateBlocksBuffers();
            return false;
        } 
    }

    circularR = new float* [numBlocks];
    for(int i=0; i<numBlocks; ++i){
        ofx_allocate_aligned(circularR[i], complexSize);  
        if(circularR[i]==nullptr){
            for(int j=0; j<i; j++){
                ofx_deallocate_aligned(circularR[i]);
            }
            delete[] impulseI;
            deallocateBlocksBuffers();
            return false;
        } 
          
    }

    circularI = new float* [numBlocks];
    for(int i=0; i<numBlocks; ++i){
        ofx_allocate_aligned(circularI[i], complexSize);
        if(circularI[i]==nullptr){
            for(int j=0; j<i; j++){
                ofx_deallocate_aligned(circularI[i]);
            }
            delete[] impulseI;
            deallocateBlocksBuffers();
            return false;
        } 
    }

    return true;
}


void pdsp::FDLConvolver::deallocateBlocksBuffers(){
      
        if(paddedInput != nullptr){
                ofx_deallocate_aligned(paddedInput);
        }
        if(overlapAdd != nullptr){
                ofx_deallocate_aligned(overlapAdd);
        }
        
        if(addR != nullptr){
                ofx_deallocate_aligned(addR);
        }
        if(addI != nullptr){
                ofx_deallocate_aligned(addI);
        }
       
        if(impulseR != nullptr){
                for(int i=0; i<numBlocks; ++i){
                    ofx_deallocate_aligned(impulseR[i]);    
                }
                delete[] impulseR;
                impulseR = nullptr;
        }
        
        if(impulseI != nullptr){
                for(int i=0; i<numBlocks; ++i){
                    ofx_deallocate_aligned(impulseI[i]);    
                }
                delete[] impulseI;  
                impulseI = nullptr;
        }
        
        if(circularR != nullptr){
                for(int i=0; i<numBlocks; ++i){
                    ofx_deallocate_aligned(circularR[i]);    
                }
                delete[] circularR;
                circularR = nullptr;
        }
        
        if(circularI != nullptr){
                for(int i=0; i<numBlocks; ++i){
                    ofx_deallocate_aligned(circularI[i]);    
                }
                delete[] circularI;
                circularI = nullptr;
        }
}


bool pdsp::FDLConvolver::loadImpulseResponseSegments(const float &inc, const int &convertedLen){
                        
        float index = 0.0f;
        
        float* tempBuffer;
        
        try
        {
            tempBuffer = new float[signalBlock];
        }
        catch (std::bad_alloc& ba)
        {
            deallocateBlocksBuffers();
            return false;
        }

        
        for(int i=0; i<numBlocks; ++i){ 

                //load the segment from the wave buffer (a bit ugly but works, and we don't need speed here)
                if(sampleRate!=impulseResponse->fileSampleRate){
                        for(int n=0; n<processingSize; ++n){
                                
                                if(index < static_cast<float>(convertedLen)){ //check if we are after the IR length
                                        tempBuffer[n] = SmoothInterpolator::smoothInterpolator.interpolate(
                                                impulseResponse->buffer[IRChannel], index, impulseResponse->length );   
                                }else{
                                        tempBuffer[n] = 0.0f;
                                }
                                index = (index + inc);
                        }
                }else{
                        for(int n=0; n<processingSize; ++n){
                                
                                if( ( i*processingSize + n )< convertedLen){ //check if we are after the IR length
                                        tempBuffer[n] = impulseResponse->buffer[IRChannel][i*processingSize + n];
                                }else{
                                        tempBuffer[n] = 0.0f;
                                }
                        }
                }
                
                //pad with zeroes
                for(int n=0; n<processingSize; ++n){
                        tempBuffer[n + processingSize] = 0.0f;
                }
                
                //fft and put into IR block buffer
                fftWorker.FFT( tempBuffer, impulseR[i], impulseI[i] );
        } 
        
        delete [] tempBuffer;
        return true;
}


void pdsp::FDLConvolver::process (int bufferSize) noexcept {
                 
        int inputState;
        const float* inputBuffer = processInput(input, inputState);

        if( IRLoaded && (inputState==AudioRate || silenceCount <= (numBlocks+4) ) ){

                int stillToProcess = bufferSize;
                
                while(stillToProcess>0){
                        //processing padding for non-power-of-two bufferSizes
                        int maxPadded;
                        int minPadded;
                        
                        if(stillToProcess <= processingSize ) { 
                                minPadded = 0;
                                maxPadded = stillToProcess;
                                processingOffset = processingSize - stillToProcess;
                                stillToProcess -= processingSize;
                                
                                blockIndex--;
                                if(blockIndex<0){ blockIndex = numBlocks-1; } 
                        }else{
                                minPadded = processingOffset;
                                maxPadded = processingSize;
                                stillToProcess -= (processingSize-processingOffset);
                        }
                        
                        //ACTUAL IR PARTITIONED CONVOLUTION
                        //FFT input
                        switch (inputState){
                        case AudioRate: //we have input signal for forward FFT
                        {
                                int n=0;
                                for(; n<minPadded; ++n){
                                        paddedInput[n] = 0.0f;
                                }
                                for(; n<maxPadded; ++n){
                                        paddedInput[n] = inputBuffer[n];
                                }
                                for(; n<signalBlock; ++n){
                                        paddedInput[n] = 0.0f;
                                }
                                fftWorker.FFT(paddedInput, circularR[blockIndex], circularI[blockIndex]);
                                silenceCount=0;
                        }
                                break;
                        default: //silence, we simply set the complex buffer to zero
                        {
                                ofx_Aeq_Zero(circularR[blockIndex], complexSize);
                                ofx_Aeq_Zero(circularI[blockIndex], complexSize);
                                silenceCount++;
                        }
                                break;
                        }
                        
                        //set add buffer to zero 
                        ofx_Aeq_Zero(addR, complexSize);
                        ofx_Aeq_Zero(addI, complexSize);
                        
                        //complex multiply and add
                        int k = blockIndex;
                        for(int i=0; i<numBlocks; ++i){
                                vect_cmadd( addR, addI, 
                                                  impulseR [i], impulseI [i],
                                                  circularR [k], circularI [k],
                                                  complexSize);
                                k++; 
                                if (k>=numBlocks) { k = 0; }
                        }

                        //inverse FFT
                        float* outputBuffer = getOutputBufferToFill(output);
                        
                        fftWorker.iFFT(paddedInput, addR, addI);  
                        
                        
                        for(int n=0; n<processingSize; ++n){
                                outputBuffer[n] = paddedInput[n];
                                outputBuffer[n]+= overlapAdd[n];
                        }
                        
                        for(int n=0; n<processingSize; ++n){
                                overlapAdd[n] = paddedInput[ processingSize +n ];
                        }           
                }
        }else{
                setOutputToZero(output);  
        }
}


