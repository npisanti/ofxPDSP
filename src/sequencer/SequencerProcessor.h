
// SequencerProcessor.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016 - 2018

#ifndef PDSP_SCORE_H_INCLUDED
#define PDSP_SCORE_H_INCLUDED

#include "../messages/header.h"
#include "../DSP/pdspCore.h"
#include "SequencerSection.h"
#include <vector>


namespace pdsp{
    /*!
    @brief Contains all the needed framework for generating and sequencing messages for the DSPs
    
    This class owns a vector of SequencerSection and it is also responsible of updating the global transport for the Clockable units. Remember that the SequencerSections inside a SequencerProcessor are processed from the first to the last so it could be possible for the data generated from the first SequencerSections in the vector to influence the others (in a thread-safe manner).
    */ 
    
class SequencerProcessor : public Preparable {
public:    
    SequencerProcessor();
    
    /*!
    @brief sets the global tempo
    @param tempo tempo to set
    */ 
    void setTempo( double tempo );
    
    /*!
    @brief returns the global tempo
    */ 
    double getTempo() const;
    
/*!
    @cond HIDDEN_SYMBOLS
*/
    /*!
    @brief this method have to be called in the audio callback, before the Processor processAndCopy...() method
    
    Update the global transport, update all the SequencerSections and connected GateSequencer, ValueSequencer 
    */ 
    void process(int const &bufferSize) noexcept;

    [[deprecated("this method is deprecated, directly use sections.resize(int size) instead ")]]
    void setSections(int sectionsNumber);
/*!
    @endcond
*/

  
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
    @brief returns true if the SequencerProcessor is playing. Thread-safe.
    */     
    bool isPlaying();
    
    /*!
    @brief sets the maximum value the global playhead can take, in bars, after it wraps back
    @param maxBars max value in bars
    
    */ 
    void setMaxBars(double maxBars);
    //void synchronizeInterface(bool active);


    /*!
    @brief gets the maximum value the global playhead can take, in bars, after it wraps back
    
    */ 
    double getMaxBars() const;

    /*!
    @brief sets the default step lenght for initializing sequences
    #param[in] steplen default step lenght
    */ 
    void setDefaultSteplen( double steplen );



    /*!
    @brief returns the playhead position in bars. Thread-safe.
    */ 
    float meter_playhead(); 

    /*!
    @brief You directly access the SequencerSections with this vector.
    */     
    std::vector<SequencerSection> sections;


    /*!
    @brief sets the sections number and the number of cells for each section, and assign a fresh new sequence to each cell. It also the tempo of the music. All the behaviors for each cells are set to pdsp::Behavior::Loop, the quantizing is deactivated by default. Using this method is not mandatory, you can set the tempo with setTempo(), directly resize the sections array and set your own cells with setCell()
    @param sections number of sections
    @param sequences number of cells for each section
    @param tempo tempo to set
    */ 
    void init ( int sections, int sequences, float tempo );

    
    /*!
    @brief Immediately launch the cells at the selected index of each section, with the given launch timings options. Thread-safe. 
    @param[in] index index of the patter to set inside the SequencerSection. This has to be a valid index. A negative index stops the playing of this section (you can perform quantized stopping if quantizeLaunch = true ).
    @param[in] quantizeLaunch if true the next pattern launch is quantized to the bars, if false the pattern is lanched as soon as possible.  If not given as argument, it is assumed false.
    @param[in] quantizeGrid if the launch is quantized this is the grid division for quantizing, in bars.  If not given as argument, it is assumed 1.0 (one bar).
    
    */         
    void launchMultipleCells(int index, bool quantizeLaunch=false, double quantizeGrid=1.0);


protected:
    void prepareToPlay( int expectedBufferSize, double sampleRate ) override;
    void releaseResources() override ;
    
private:


    /*!
    @brief sets actual playhead value directly. Thread-safe.
    @param newPlayhead playhead value to set
    */ 
    void setPlayHead(float newPlayHead);
    
    double playHead;
    double playHeadEnd;
    std::atomic<float> newPlayHead;
    
    double tempo;
    double barsPerSample;
    double sampleRate;
    
    double maxBars;
    
    std::atomic<bool> playing;
    //bool synchronizeClockable;
 
    std::mutex playheadMutex;
    
    std::atomic<float> playhead_meter;
    int clearToken;
    
    std::atomic<double> tempoControl;
    
};



    
    
}


#endif //PDSP_SCORE_H_INCLUDED
