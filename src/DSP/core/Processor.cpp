
#include "Processor.h"
#include <iostream>

pdsp::Processor::Processor(int channels){
        this->channels.resize(channels);
        for(int i=0; i<channels; ++i){
                this->channels[i].disableAutomaticProcessing();
        }
}

pdsp::Processor::Processor() : Processor(PDSP_MAX_OUTPUT_CHANNELS) {} 

void pdsp::Processor::process(const int &bufferSize) noexcept{
        OutputNode::nextTurn();
        Preparable::setTurnBufferSize(bufferSize);

        for(int i=0; i<channels.size(); ++i){
                channels[i].process(bufferSize);
        }
}

void pdsp::Processor::resize( int channelsNum ){
    this->channels.resize(channelsNum);
        for(int i=0; i<channelsNum; ++i){
                this->channels[i].disableAutomaticProcessing();
    }    
}

void pdsp::Processor::processAndCopyOutput(float** bufferToFill, const int &channelsNum, const int &bufferSize) noexcept{
     
        OutputNode::nextTurn();
        Preparable::setTurnBufferSize(bufferSize);
        
        for(int i=0; i<channels.size(); ++i){
                
                channels[i].process(bufferSize);
                
                if(i<channelsNum){
                        if(channels[i].getState()==AudioRate){
                                //std::cout<<"audiorate ";
                                const float* processed = channels[i].getBuffer();
                                for (int n = 0; n < bufferSize; ++n){
                                        bufferToFill[i][n] = processed[n];
                                }
                        }else{
                                for (int n = 0; n < bufferSize; ++n){
                                        bufferToFill[i][n] = 0.0f;;
                                }   
                        }
                }
        }
        
        blackhole.process(bufferSize);
}


void pdsp::Processor::processAndCopyInterleaved(float* bufferToFill, const int &channelsNum, const int &bufferSize) noexcept{
      
        OutputNode::nextTurn();
        Preparable::setTurnBufferSize(bufferSize);
        
        int min;
        if(channels.size() < channelsNum){
            min = channels.size();
        }else{
            min = channelsNum;
        }
        
        for(int i=0; i<min; ++i){
                
                channels[i].process(bufferSize);
                
                if(i<channelsNum){
                        if(channels[i].getState()==AudioRate){
                                const float* processed = channels[i].getBuffer();
                                for (int n = 0; n < bufferSize; ++n){
                                        bufferToFill[n*channelsNum + i] = processed[n];
                                }
                        }else{
                                for (int n = 0; n < bufferSize; ++n){
                                        bufferToFill[n*channelsNum + i] = 0.0f;;
                                }   
                        }
                }
        }
 
        blackhole.process(bufferSize);

}

