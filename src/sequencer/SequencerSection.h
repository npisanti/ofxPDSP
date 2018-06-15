
// SequencerSection.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016 - 2018

#ifndef PDSP_SCOREROW_H_INCLUDED
#define PDSP_SCOREROW_H_INCLUDED

#include "SequencerMessage.h"
#include "Sequence.h"
#include "stockBehaviors.h"
#include <vector>
#include "../messages/header.h"
#include "../DSP/control/SequencerBridge.h"
#include "../DSP/control/SequencerGateOutput.h"
#include "../DSP/control/SequencerValueOutput.h"
#include "../DSP/pdspCore.h"
#include <mutex>
#include "../flags.h"

namespace pdsp{
    
    /*!
    @brief Plays a single pdsp::Sequence at time, queue Sequences
    
    This class plays a single Spdsp::Sequence at time, sequences Sequences and has multiple outputs to connect to SequencerGateOutput or SequencerValueOutput. SequencerProcessor owns a vector of SequencerSection. Remember that the SequencerSections inside a SequencerProcessor are processed from the first to the last so it could be possible for the data generated from the first SequencerSections in the vector to influence the others (in a thread-safe manner). In this doc, we refere to "Cell" as a combination of a pdsp::Sequence pointer and its associated pdsp::SeqChange behavior (for example pdsp::Behavior::Loop , pdsp::pdsp::Behavior::Random or pdsp::Behavior::Stop ),
    */

class SequencerSection { 
    friend class SequencerProcessor;   

    
private:

    //---------------------------------------INNER CLASS------------------------
    /*!
        @cond HIDDEN_SYMBOLS
    */
    class SeqCell{
    public:
        SeqCell() : sequence(nullptr), nextCell(Behavior::Loop), quantizeLaunch(false), quantizeGrid(0.0) {};
            
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

    SequencerSection();
    ~SequencerSection();
    SequencerSection(const SequencerSection &other);



    /*!
    @brief Sets the number of Cells contained by this SequencerSection
    @param[in] size number of Sequence contained by this SequencerSection

    */ 
    void resizeCells(int size);

    /*!
    @brief returns the number of Cells contained by this SequencerSection

    */ 
    int getCellsNumber() const;    


    /*!
    @brief returns the sequence at the given index, trigger an assert if the sequence is not set so it's nullptr
    */ 
    Sequence& sequence( int index );


    /*!
    @brief Sets the SeqChange that determine what cell will be played after this, this is an alias for setChange()
    
    Mostly you'd like to use setLooping( int index ) or setOneShot( int index ) if you don't need more complex behaviors 
    */ 
    void behavior (int index, pdsp::SeqChange* newBehavior );
    
    /*!
    @brief enables quantization of next cell launch.
    @param[in] index index of the patter to set inside the SequencerSection. This has to be a valid index.
    @param[in] quantizeGrid if the launch is quantized this is the grid division for quantizing, in bars.
    */     
    void quantize (int index, double quantizeTime);

    /*!
    @brief sets the label for the given cell
    @param[in] index index of the patter to set inside the SequencerSection. This has to be a valid index.
    @param[in] name new label
    */       
    void label ( int index, std::string name ); 

    
    /*!
    @brief Set the output with the given index as selected output and return a MessageBuffer for patching to external outs (like midi or serial)
    @param[in] index index of the out to patch, 0 if not given

    Set the output with the given index as selected output and return this SequencerSection for patching. Usually you patch the result to an ofxPDSPMidiOut using the >> operator, but you can also patch it to a SequencerGateOutput or SequencerValueOutput. Also eventually resize the outputs number if needed.
    */     
    pdsp::MessageBuffer& out_message( int index = 0 );
    
    
    /*!
    @brief Set the output with the given index as a gate output and return the SequencerGateOutput for patching
    @param[in] index index of the out to patch, 0 if not given
    */     
    SequencerGateOutput& out_trig( int index = 0 );
    
    
    /*!
    @brief Set the output with the given index as a value output and return the SequencerValueOutput reference. You can use the result for patching or for set the value slew time (slew is deactivated by default).
    @param[in] index index of the out to patch, 0 if not given
    */   
    SequencerValueOutput& out_value( int index = 0 );
    
    
    /*!
    @brief after the linking the messages sent to the slewControlIndex output will be used to scale the slew time of the SequencerValueOutput set at out_value(valueOutIndex). 
    @param[in] valueOutIndex
    @param[in] slewControlIndex
    For setting the base slew time you use out_value(int index).setSlewTime(float slewTimeMs, SlewMode_t mode = Time);
    */   
    void linkSlewControl( int valueOutIndex, int slewControlIndex ); 
    
    
    /*!
    @brief Sets the Cell at the given index to the given one
    @param[in] index index of the Sequence (or Sequence) to set inside the SequencerSection. If the size is too little the SequencerSection is automatically resized.
    @param[in] sequence pointer to a Sequence, you can also set it to nullptr
    @param[in] behavior pointer to a SeqChange, you can also set it to nullptr, pdsp::Behavior::Loop if not given
    @param[in] label an optional string to identify this cell

    Sets the sequence pattern at a given index. If Sequence is set to nullptr then nothing is played, If SeqChange is set to nullptr the sequencing is stopped after playing this Pattern. Sequence is a subclass of Sequence easiear to manage.
    */ 
    void setCell( int index, Sequence* sequence, SeqChange* behavior = Behavior::Loop, std::string label = "" );
    
    
    /*!
    @brief Sets the label for the cell at the given index
    @param[in] index index of the Sequence (or Sequence) for setting the label
    @param[in] label an optional string to identify this cell

    */ 
    void setLabel( int index, std::string label );
    
    /*!
    @brief Gets the label for the cell at the given index
    @param[in] index index of the Sequence (or Sequence) for setting the label

    */ 
    std::string  getLabel( int index);
    

    /*!
    @brief Sets the SeqChange that determine what cell will be played after this
    @param[in] index index of the patter to set inside the SequencerSection. If the size is too little the SequencerSection is automatically resized.

    */ 
    void setChange( int index, SeqChange* behavior );
   
   
    /*!
    @brief Sets this Cell to Loop on itself, it is the standard behavior
    @param[in] index index of the patter to set inside the SequencerSection. If the size is too little the SequencerSection is automatically resized.
    @param[in] behavior pointer to a SeqChange

    */ 
    void setLooping( int index );
   
    /*!
    @brief Sets this Cell to stop after being launched.
    @param[in] index index of the patter to set inside the SequencerSection. If the size is too little the SequencerSection is automatically resized.

    */ 
    void setOneShot( int index );

    
    /*!
    @brief Sets some values for the pattern quantized launch.
    @param[in] index index of the patter to set inside the SequencerSection. This has to be a valid index.
    @param[in] quantizeLaunch if true the next pattern launch is quantized to the bars, if false is executed when the given length expires. 
    @param[in] quantizeGrid if the launch is quantized this is the grid division for quantizing, in bars.

    Sets the score pattern timing options.
    */     
    void setCellQuantization(int index, bool quantizeLaunch = false, double quantizeGrid = 1.0f );

    /*!
    @brief enables quantization of next cell launch.
    @param[in] index index of the patter to set inside the SequencerSection. This has to be a valid index.
    @param[in] quantizeGrid if the launch is quantized this is the grid division for quantizing, in bars.

    */     
    void enableQuantization(int index, double quantizeGrid = 1.0f );
    

    /*!
    @brief disables quantization of next cell launch.
    @param[in] index index of the patter to set inside the SequencerSection. This has to be a valid index.
    @param[in] quantizeGrid if the launch is quantized this is the grid division for quantizing, in bars.

    */     
    void disableQuantization(int index);
    
    
    /*!
    @brief Immediately launch the selected cell, with the given launch timings options. Thread-safe. 
    @param[in] index index of the patter to set inside the SequencerSection. This has to be a valid index. A negative index stops the playing of this section (you can perform quantized stopping if quantizeLaunch = true ).
    @param[in] quantizeLaunch if true the next pattern launch is quantized to the bars, if false the pattern is lanched as soon as possible.  If not given as argument, it is assumed false.
    @param[in] quantizeGrid if the launch is quantized this is the grid division for quantizing, in bars.  If not given as argument, it is assumed 1.0 (one bar).
    
    */         
    void launch(int index, bool quantizeLaunch=false, double quantizeGrid=1.0);
    
    
    /*!
    @brief Set the behavior of connected SequencerGateOutput on Sequence changes, if true a trigger off is sent between Sequences. true is the standard behavior.
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
    @brief returns the percentual of completion of the playing sequence. Thread-safe.
    */ 
    float meter_percent() const; 
    
    
    /*!
    @brief returns the current sequence length in bars. Thread-safe.
    */ 
    float meter_length() const; 
    
    /*!
    @brief returns the sequence at the given index, read only
    */ 
    const Sequence & getSequence( int i ) const;
    

    /*!
    @brief create and assign a fresh sequence to each Cell, sets the change to pdsp::Behavior::Loop
    */     
    void autoInitCells();
    
/*!
    @cond HIDDEN_SYMBOLS
*/
    [[deprecated("deprecated function, replaced by resizeCells(int size)")]]
    void resizePatterns(int size);

    [[deprecated("deprecated function, replaced by getCellsNumber()")]]
    int getPatternsNumber() const;

    [[deprecated("deprecated function, replaced by out_message(int index)")]]
    MessageBuffer& out( int index = 0 );
    
    [[deprecated("Replaced by setCell() for a less ambigous nomenclature")]]
    void setPattern( int index, Sequence* sequence, SeqChange* behavior = nullptr );
    
    [[deprecated("Replaced by setChange() for a less ambigous nomenclature")]]
    void setBehavior( int index, SeqChange* behavior );

    inline void launchCell(int index, bool quantizeLaunch=false, double quantizeGrid=1.0){
        launch(index, quantizeLaunch, quantizeGrid );
    }    
/*!
    @endcond
*/
private:

    
    /*!
    @brief Sets the number of outputs of this SequencerSection, default is 1.
    @param[in] size number of outputs for connection

    Set the number of outputs that can be connected to SequencerGateOutput or SequencerValueOutput, default is 1.
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
    void resetCounterOnStop();
    
    std::vector<SeqCell>     patterns;
    
    int                         scheduledPattern;
    double                      scheduledTime;
    
    int                         launchedPattern;
    int                         launchedPattern2;
    double                      launchSchedule;
    double                      launchQuantization;
    std::atomic<bool>           launchingCell;
    bool                        quantizedLaunch;

    
    int                         patternIndex;
    
    double                      scorePlayHead; //position inside the actual clip
    int                         scoreIndex;
  
    bool                        run;
    bool                        clear;

    bool                        clearOnChangeFlag;

    
    
    std::vector<SequencerGateOutput*>     gates;
    std::vector<SequencerValueOutput*>    values;
    
    std::vector<MessageBuffer*>  outputs;
    
    
    //std::mutex                  patternMutex;
    
    std::atomic<int>            atomic_meter_current;
    std::atomic<int>            atomic_meter_next; 
    std::atomic<float>          atomic_meter_playhead;
    std::atomic<float>          atomic_meter_length;
    std::atomic<float>          atomic_meter_percent;

    std::vector<pdsp::Sequence> seqs;
    
    static SequencerGateOutput        invalidGate;
    static SequencerValueOutput       invalidValue;
    
    static Sequence             dummySeq;
    static std::string          emptyLabel;
    
};// END SequencerSection


}//END NAMESPACE

#endif //PDSP_SCOREROW_H_INCLUDED
