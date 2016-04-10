
// ScoreSection.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_SCOREROW_H_INCLUDED
#define PDSP_SCOREROW_H_INCLUDED

#include "ScoreMessage.h"
#include "Sequence.h"
#include <vector>
#include "../messages/header.h"
#include "../DSP/control/Sequencer.h"
#include "../DSP/control/GateSequencer.h"
#include "../DSP/control/ValueSequencer.h"
#include "../DSP/pdspCore.h"
#include <mutex>
#include "../flags.h"

namespace pdsp{
    
    /*!
    @brief Plays a single Sequence at time, sequences Sequences
    
    This class plays a single Sequence at time, sequences Sequences and has multiple outputs to connect to GateSequencer or ValueSequencer. ScoreProcessor owns a vector of ScoreSection. Remember that the ScoreSections inside a ScoreProcessor are processed from the first to the last so it could be possible for the data generated from the first ScoreSections in the vector to influence the others (in a thread-safe manner).
    */

class ScoreSection { 
    friend class ScoreProcessor;   
    friend Patchable& linkSelectedOutputToSequencer (ScoreSection& scoreSection, Sequencer& input);
    friend void linkSelectedOutputToExtSequencer (ScoreSection& scoreSection, ExtSequencer& ext);
    friend void unlinkSelectedOutputToExtSequencer (ScoreSection& scoreSection, ExtSequencer& ext);
    
private:

    //---------------------------------------INNER CLASS------------------------
    /*!
        @cond HIDDEN_SYMBOLS
    */
    class SeqCell{
    public:
        SeqCell() : sequence(nullptr), nextCell(nullptr), quantizeLaunch(false), quantizeGrid(0.0) {};
            
        Sequence*       sequence;
        SeqChange*      nextCell;
        bool            quantizeLaunch;
        double          quantizeGrid;
    };    
    /*!
        @endcond
    */    

    //--------------------------------------------------------------------------
    
public:

    ScoreSection();
    ~ScoreSection();
    ScoreSection(const ScoreSection &other);


    /*!
    @brief Sets the number of patterns contained by this ScoreSection
    @param[in] size number of Sequence contained by this ScoreSection

    */ 
    void resizePatterns(int size);
    

    
    
    /*!
    @brief Set the output with the given index as selected output and return this ScoreSection for patching to midi outs
    @param[in] index index of the out to patch, 0 if not given

    Set the output with the given index as selected output and return this ScoreSection for patching. Usually you patch the result to an ofxPDSPMidiOut using the >> operator, but you can also patch it to a GateSequencer or ValueSequencer. Also eventually resize the outputs number if needed.
    */     
    ScoreSection& out_message( int index = 0 );
    
    
    /*!
    @brief Set the output with the given index as a gate output and return the GateSequencer for patching
    @param[in] index index of the out to patch, 0 if not given
    */     
    GateSequencer& out_trig( int index = 0 );
    
    
    /*!
    @brief Set the output with the given index as a value output and return the ValueSequencer reference. You can use the result for patching or for set the value slew time (slew is deactivated by default).
    @param[in] index index of the out to patch, 0 if not given
    */   
    ValueSequencer& out_value( int index = 0 );
    
    
    /*!
    @brief after the linking the messages sent to the slewControlIndex output will be used to scale the slew time of the ValueSequencer set at out_value(valueOutIndex). 
    @param[in] valueOutIndex
    @param[in] slewControlIndex
    For setting the base slew time you use out_value(int index).setSlewTime(float slewTimeMs, SlewMode_t mode = Time);
    */   
    void linkSlewControl( int valueOutIndex, int slewControlIndex ); 
    
    
    /*!
    @brief Sets the Cell at the given index to the given one
    @param[in] index index of the Sequence (or Sequence) to set inside the ScoreSection. If the size is too little the ScoreSection is automatically resized.
    @param[in] sequence pointer to a Sequence, you can also set it to nullptr
    @param[in] behavior pointer to a SeqChange, you can also set it to nullptr, nullptr if not given

    Sets the score pattern at a given index. If Sequence is set to nullptr then nothing is played, If SeqChange is set to nullptr the sequencing is stopped after playing this Pattern. Sequence is a subclass of Sequence easiear to manage.
    */ 
    void setCell( int index, Sequence* sequence, SeqChange* behavior = nullptr );
    

    /*!
    @brief Sets the SeqChange that determine what cell will be played after this
    @param[in] index index of the patter to set inside the ScoreSection. If the size is too little the ScoreSection is automatically resized.
    @param[in] behavior pointer to a SeqChange

    */ 
    void setChange( int index, SeqChange* behavior );
    
    
    /*!
    @brief Sets some values for the pattern quantized launch, Thread-safe.
    @param[in] index index of the patter to set inside the ScoreSection. This has to be a valid index.
    @param[in] quantizeLaunch if true the next pattern launch is quantized to the bars, if false is executed when the given length expires. 
    @param[in] quantizeGrid if the launch is quantized this is the grid division for quantizing, in bars.

    Sets the score pattern timing options.
    */     
    void setCellQuantization(int index, bool quantizeLaunch = false, double quantizeGrid = 1.0f );

    /*!
    @brief enables quantization of next cell launch. Thread-safe.
    @param[in] index index of the patter to set inside the ScoreSection. This has to be a valid index.
    @param[in] quantizeGrid if the launch is quantized this is the grid division for quantizing, in bars.

    */     
    void enableQuantization(int index, double quantizeGrid = 1.0f );
    

    /*!
    @brief disables quantization of next cell launch. Thread-safe.
    @param[in] index index of the patter to set inside the ScoreSection. This has to be a valid index.
    @param[in] quantizeGrid if the launch is quantized this is the grid division for quantizing, in bars.

    */     
    void disableQuantization(int index);
    
    
    /*!
    @brief Immediately launch the selected cell, with the given launch timings options. Thread-safe. 
    @param[in] index index of the patter to set inside the ScoreSection. This has to be a valid index. A negative index stops the playing of this section (you can perform quantized stopping if quantizeLaunch = true ).
    @param[in] quantizeLaunch if true the next pattern launch is quantized to the bars, if false the pattern is lanched as soon as possible.  If not given as argument, it is assumed false.
    @param[in] quantizeGrid if the launch is quantized this is the grid division for quantizing, in bars.  If not given as argument, it is assumed 1.0 (one bar).
    
    */         
    void launchCell(int index, bool quantizeLaunch=false, double quantizeGrid=1.0);
    
    
    /*!
    @brief Set the behavior of connected GateSequencer on Sequence changes, if true a trigger off is sent between Sequences. true is the standard behavior.
    @param[in] active activate if true, deactivate if false
    
    */  
    void clearOnChange(bool active);
    
    /*!
    @brief returns the current cell index. If you use it just for visualization you can consider it thread-safe.
    */ 
    int meter_current() const; 

    /*!
    @brief returns the next playing cell index. If you use it just for visualization you can consider it thread-safe.
    */ 
    int meter_next() const;     
    
    /*!
    @brief returns the playhead position in bars. Thread-safe.
    */ 
    float meter_playhead() const; 
    
    
    /*!
    @brief returns the current sequence length in bars. Thread-safe.
    */ 
    float meter_length() const; 
    
    
/*!
    @cond HIDDEN_SYMBOLS
*/
    [[deprecated("deprecated function, replaced by out_message(int index)")]]
    ScoreSection& out( int index = 0 );
    
    [[deprecated("Replaced by setCell() for a less ambigous nomenclature")]]
    void setPattern( int index, Sequence* sequence, SeqChange* behavior = nullptr );
    
    [[deprecated("Replaced by setChange() for a less ambigous nomenclature")]]
    void setBehavior( int index, SeqChange* behavior );
/*!
    @endcond
*/
private:

    
    /*!
    @brief Sets the number of outputs of this ScoreSection, default is 1.
    @param[in] size number of outputs for connection

    Set the number of outputs that can be connected to GateSequencer or ValueSequencer, default is 1.
    */ 
    void setOutputsNumber(int size);

    void processSection(const double    &startPlayHead, 
                    const double    &endPlayHead, 
                    const double    &playHeadDifference, 
                    const double    &maxBars,
                    const double    &barsPerSample, 
                    const int       &bufferSize) noexcept;
                       
    

    void playScore(double const &range, double const &offset, const double &oneSlashBarsPerSample) noexcept;
    void allNoteOff(double const &offset, const double &oneSlashBarsPerSample) noexcept;
    void onSchedule() noexcept;
    void clearBuffers() noexcept;
    void processBuffersDestinations(const int &bufferSize) noexcept;
    
    std::vector<SeqCell>     patterns;
    
    int                         scheduledPattern;
    double                      scheduledTime;
    
    int                         launchedPattern;
    double                      launchSchedule;
    double                      launchQuantization;
    bool                        launchingCell;
    bool                        quantizedLaunch;

    
    int                         patternIndex;
    
    double                      scorePlayHead; //position inside the actual clip
    int                         scoreIndex;
  
    bool                        run;
    bool                        clear;

    bool                        clearOnChangeFlag;

    
    
    std::vector<GateSequencer*>     gates;
    std::vector<ValueSequencer*>    values;
    
    std::vector<MessageBuffer>  outputs;
    //int                         outputSize;
    
    MessageBuffer*              selectedMessageBuffer;
    
    std::mutex                  patternMutex;
    
    std::atomic<int>            atomic_meter_current;
    std::atomic<int>            atomic_meter_next; 
    std::atomic<float>          atomic_meter_playhead;
    std::atomic<float>          atomic_meter_length;
    
    static GateSequencer        invalidGate;
    static ValueSequencer       invalidValue;
    
};// END ScoreSection

Patchable& linkSelectedOutputToSequencer (ScoreSection& scoreSection, Sequencer& input);
Patchable& operator>> (ScoreSection& scoreSection, Sequencer& input);

void linkSelectedOutputToExtSequencer (ScoreSection& scoreSection, ExtSequencer& ext);
void operator>> (ScoreSection& scoreSection, ExtSequencer& ext);

void unlinkSelectedOutputToExtSequencer (ScoreSection& scoreSection, ExtSequencer& ext);
void operator!= (ScoreSection& scoreSection, ExtSequencer& ext);

}//END NAMESPACE

#endif //PDSP_SCOREROW_H_INCLUDED
