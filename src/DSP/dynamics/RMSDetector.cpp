
#include "RMSDetector.h"



pdsp::RMSDetector::RMSDetector(float window_ms){
    addInput("signal", input);
    addOutput("signal", output);
    updateOutputNodes();

    this->window_ms = window_ms;
    squaresDividedBuffer = nullptr;
    
    if(dynamicConstruction){
        prepareToPlay(globalBufferSize, globalSampleRate);
    }  
}


pdsp::RMSDetector::RMSDetector() : RMSDetector(50.0f) {}


pdsp::Patchable& pdsp::RMSDetector::in_signal(){
    return in("signal");
}

pdsp::Patchable& pdsp::RMSDetector::out_signal(){
    return out("signal");
}


float pdsp::RMSDetector::meter_output() const{
    return meter.load();
}

pdsp::Patchable& pdsp::RMSDetector::set(float window_ms){
    setWindowTime(window_ms);
    return *this;
}

void pdsp::RMSDetector::prepareUnit( int expectedBufferSize, double sampleRate ) {
    sampleRateMultiplier = sampleRate * 0.001f;
    
    updateWindowBuffer();
}


void pdsp::RMSDetector::releaseResources () {
    if(squaresDividedBuffer != nullptr){
        ofx_deallocate_aligned( squaresDividedBuffer );
    }
}

void pdsp::RMSDetector::setWindowTime(float window_ms){
    this->window_ms = window_ms;
    
    if(dynamicConstruction){
        updateWindowBuffer();
    }
}


void pdsp::RMSDetector::updateWindowBuffer(){
    
        bufferLength = static_cast<int>(window_ms * sampleRateMultiplier);
    
        if(squaresDividedBuffer != nullptr){
            ofx_deallocate_aligned( squaresDividedBuffer );
        }
        ofx_allocate_aligned ( squaresDividedBuffer, bufferLength );
        
        readIndex = 0;
        actualSum = 0.0f;
        
        windowDivisor = 1.0f / static_cast<float>(bufferLength);    

}


void pdsp::RMSDetector::process (int bufferSize) noexcept {
        
        int inputState;
        const float* inputBuffer = processInput(input, inputState);

        if(inputState==AudioRate){
              
                float* outputBuffer = getOutputBufferToFill(output);
                
                ofx_Aeq_BmulC(outputBuffer, inputBuffer, inputBuffer, bufferSize); //squaring the input
                ofx_Aeq_BmulS(outputBuffer, outputBuffer, windowDivisor, bufferSize); //dividing the input for the number of meaned samples
                
                for(int n=0; n<bufferSize; ++n){ //writing the sums to the buffer sample by sample
                        
                        actualSum -= squaresDividedBuffer[readIndex];
                        actualSum += outputBuffer[n];
                        squaresDividedBuffer[readIndex] = outputBuffer[n];
                        
                        outputBuffer[n] = actualSum;
                        
                        readIndex++;
                        if(readIndex==bufferLength){
                                readIndex = 0;
                        }

                }
                
                ofx_Aeq_sqrtB(outputBuffer, outputBuffer, bufferSize); //square root the sums;
                meter.store(outputBuffer[0]);
                
        }else{
                setOutputToZero(output);
                meter.store(0.0f);
        }
}
