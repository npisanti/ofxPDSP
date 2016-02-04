
// ScoreProcessor.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_SCORE_H_INCLUDED
#define PDSP_SCORE_H_INCLUDED

#include "../messages/header.h"
#include "../DSP/pdspCore.h"
#include "ScoreSection.h"
#include <vector>

namespace pdsp{
    /*!
    @brief Contains all the needed framework for generating and sequencing messages for the DSPs
    
    This class owns a vector of ScoreSection and it is also responsible of updating the global transport for the Clockable units. Remember that the ScoreSections inside a ScoreProcessor are processed from the first to the last so it could be possible for the data generated from the first ScoreSections in the vector to influence the others (in a thread-safe manner).
    */ 
    
class ScoreProcessor : public Preparable {
public:    
    ScoreProcessor();
    
    /*!
    @brief this method have to be called in the audio callback, before the Processor processAndCopy...() method
    
    Update the global transport, update all the ScoreSections and connected GateSequencer, ValueSequencer 
    */ 
    void process(int const &bufferSize) noexcept;

    /*!
    @brief sets the global tempo
    @param tempo tempo to set
    */ 
    void setTempo( float tempo );
    
    /*!
    @brief sets the number of ScoreSections contained in the ScoreProcessor
    @param sectionsNumber number of ScoreSection to set
    
    Sets the number of ScoreSections contained in the ScoreProcessor. If you want just to use the ScoreProcessor for updating the tempo and playhead you can set this to 0.
    */ 
    void setSections(int sectionsNumber);


  
    /*!
    @brief pause the playhead and stop emitting messages and updating the playhead. Thread-safe.
    */ 
    void pause();    
    
    /*!
    @brief reset the playhead to 0.0f and stop emitting messages and updating the playhead. Thread-safe.
    */ 
    void stop();        
  
    /*!
    @brief starts emitting message and updating the playhead again. Thread-safe.
    */ 
    void play();  

    /*!
    @brief returns true if the ScoreProcessor is playing. Thread-safe.
    */     
    bool isPlaying();
    
    /*!
    @brief sets the maximum value the global playhead can take, in bars, after it wraps back
    @param maxBars max value in bars
    
    */ 
    void setMaxBars(double maxBars);
    //void synchronizeInterface(bool active);

    /*!
    @brief returns the playhead position in bars. Thread-safe.
    */ 
    float meter_playhead(); 

    /*!
    @brief You directly access the ScoreSections with this vector.
    */     
    std::vector<ScoreSection> sections;

protected:
    void prepareToPlay( int expectedBufferSize, double sampleRate ) override;
    void releaseResources() override ;
    
private:


    /*!
    @brief sets actual playhead value directly. Thread-safe.
    @param newPlayhead playhead value to set
    */ 
    void setPlayhead(double newPlayhead);
    
    double playHead;
    double playHeadEnd;
    double tempo;
    double barsPerSample;
    double sampleRate;
    
    double maxBars;
    
    std::atomic<bool> playing;
    //bool synchronizeClockable;
 
    std::mutex playheadMutex;
    
    std::atomic<float> playhead_meter;
    int clearToken;
    
};



    
    
}


#endif //PDSP_SCORE_H_INCLUDED
