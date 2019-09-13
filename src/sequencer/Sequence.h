
// Sequence.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016 - 2018

#ifndef PDSP_SEQUENCE_H_INCLUDED
#define PDSP_SEQUENCE_H_INCLUDED

#include "SequencerMessage.h"
#include <functional>
#include <atomic>
#include <iostream>
#include "../DSP/pdspCore.h"

namespace pdsp{
    
    /*!
    @brief powerful class for sequencing message. 
    Sequence is a class and container for values to be sequenced. You can set the value to sequence from an inline array or a std::vector. It also has an assignable lambda function that is executed each time the Sequence starts.
    */    
    class Sequence {
        friend class SequencerSection;
        friend class SequencerProcessor;
    public:
        Sequence( double stepDivision );
        Sequence();
        Sequence(const Sequence & other);
        Sequence(Sequence && other);
        Sequence& operator= (const Sequence & other);
        Sequence& operator= (Sequence && other);
      
      
        /*!
        @brief public access to read and set the sequence label.
        */     
        std::string label;
        
        /*!
        @brief public access to read and set the duration of the sequence in bars, initializated to 1.0 (one bar loop)
        */     
        std::atomic<double> timing;
        
        /*!
        @brief returns how many time this Sequence has been restarted (0 if it is the first time it starts). This internal counter can be reset to zero with resetCount().
        */
        int counter() const;

        /*!
        @brief resets the internal counter you can get with counter(). This counter is resetted automatically when there is a change from a sequence to another or when a Sequence is launched manually.
        */
        void resetCount();
            
        /*!
        @brief you call begin() before calling message, this prepare the Sequence for the message() method, clearing the buffers.
        */
        void begin() noexcept;

        /*!
        @brief you call end() when you have finished adding value with Message(). When the Sequence restarts the new sequence will be played.
        */
        void end() noexcept;
        
        /*!
        @brief sets the output for the next banged message
        @param[in] tag the output's name
        */        
        pdsp::Sequence & out( std::string tag );

        /*!
        @brief sets the output for the next banged message
        @param[in] value the output's index
        */    
        pdsp::Sequence & out( int value );

        /*!
        @brief sets the delay from the start of the sequence of the next message, in fraction of the sequence's timing
        @param[in] value the delay of the message
        */            
        pdsp::Sequence &  delay( double value );
        
        /*!
        @brief outputs a message. You can use out() to choose the output or delay() to change the timing
        @param[in] value the message value
        */ 
        pdsp::Sequence &  bang( float value );

        /*!
        @brief outputs a legato message. You can use out() to choose the output or delay() to change the timing. This message won't retrigger envelopes like ADSR, instead the envelope will decay to the given value. Only works if the selected output is patched as trigger output.
        @param[in] value the message value
        */ 
        pdsp::Sequence &  legato( float value );
        
        /*!
        @brief outputs a slew message. You can use out() to choose the output or delay() to change the timing. Smooths out next messages with the give time in milliseconds. Only works if the selected output is patched as value output.
        @param[in] value the message value
        */ 
        pdsp::Sequence &  slew( float milliseconds );

        /*!
        @brief this lambda function is executed each time the Sequence starts from the begin. Assign your own functions to it.
        This lambda function is executed each time the Sequence starts from the begin. Usually is empty, but you can assign your own lambdas to generate new values each time the Sequence starts. Remember that the code executed can make a previosly called set() method ininfluent. Remember that this function will be executed into the audio-thread so the access to some variable used also into the main thread could cause race conditions.
        */
        std::function<void()> code;

        /*!
        @brief returns the percentual of completion of this sequence. When the sequence is not playing it will return the last value. Thread-safe.
        */ 
        float meter_percent() const; 


/*!
    @cond HIDDEN_SYMBOLS
*/
        // --------------------------- API TO DEPRECATE --------------
    
        /*!
        @brief with this method you can manually add timed values to the sequence. You have to call begin() before adding messages and end() when you've done. Also note that the old Sequence values are not kept so you are adding values to an empty sequence.
        @param[in] step the step index 
        @param[in] value the value of the step
        @param[in] outputIndex the output of the SequencerSection that will operate with the message, 0 if not given
        */
        void message(double step, float value, int outputIndex=0) noexcept;
                
        /*!
        @brief sets the sequence from an inlined array. Negative values produce no messages.
        @param[in] init an inline array, for example {1.0f, 0.0f, -1.0f, -1.0f, 0.25f, 0.5f, 0.0f, -1.0f }
        
        If outputs>1 then consecutive values are send to sequentially numerated outputs, see lessons in the wiki for a pratical example.
        */
        void set( std::initializer_list<float> init ) noexcept;

        /*!
        @brief sets the sequence from an inlined array. Negative values produce no messages.
        @param[in] init an inline array 2d, for example { {1.0f, 0.0f, 1.0f, 0.0f}, {43.f, 24.f, 43.5f, 55.f } }
        @param[in] outputs how many outputs has the sequence, 1 if the argument is not given.
        
        */
        void set( std::initializer_list<std::initializer_list<float> >  init ) noexcept;

        /*!
        @brief sets the sequence from an inlined array. Negative values produce no messages.
        @param[in] init an inline array, for example {1.0f, 0.0f, -1.0f, -1.0f, 0.25f, 0.5f, 0.0f, -1.0f }
        @param[in] division time division
        @param[in] length time before the next Sequence/SequencerCell will be started
        
        */
        void set( std::initializer_list<float> init, double division, double length  ) noexcept;

        /*!
        @brief sets the sequence from an inlined 2d array. Negative values produce no messages.
        @param[in] init an inline array 2d, for example { {1.0f, 0.0f, 1.0f, 0.0f}, {43.f, 24.f, 43.5f, 55.f } }
        @param[in] division time division
        @param[in] length time before the next Sequence/SequencerCell will be started
        
        */
        void set( std::initializer_list<std::initializer_list<float> >  init , double division, double length  ) noexcept;

        /*!
        @brief returns the step index, calculated with the current steplen and bars. When the sequence is not playing it will return the last value. Thread-safe.
        */ 
        int meter_step() const; 

        /*!
        @brief sets the time division to be used for each step. For example 8.0f means each step is 1/8th. Default division is 1/16th.
        @param[in] value time division
        */
        void setDivision( double value );
        
        /*!
        @brief sets the time in bars before the next Sequence will be launched. Messages with a time greater than this won't be played. 
        @param[in] value new length
        */
        void setLength( double bars );
        
        /*!
        @brief returns the time in bars before the next Sequence will be launched. 
        */
        double length() const;

        /*!
        @brief returns the currently set time division, for example 8.0 is 1/8th. 
        */
        double division() const;

        /*!
        @brief sets the time division and length of the sequence.
        @param[in] division time division
        @param[in] bars new length
        */
        void setTiming( double division, double bars );

        /*!
        @brief you call begin() before calling message, this prepare the Sequence for the message() method, clearing the buffers. Also set division and length.
        @param[in] division time division
        @param[in] length time before the next Sequence/SequencerCell will be started
        */
        void begin( double division, double length ) noexcept;

        /*!
        @brief call begin() before calling this function. This function will add all the values of the array with the given division.
        @param[in] vect std::vector<float> of values to add to the sequence, negative values will be ignored
        @param[in] outputIndex the output of the SequencerSection that will operate with the message, 0 if not given
        */
        void messageVector( std::vector<float> vect, int outputIndex = 0);

        /*!
        @brief call begin() before calling this function. This function will add all the values of the array with the given division and will also add a 0.0f value after each value. The distange of the 0.0f value is given by gateLength. Only values greater than 0.0f are used.
        @param[in] vect std::vector<float> of values to add to the sequence, negative values will be ignored
        @param[in] gateLength length of gate = division * gateLength
        @param[in] outputIndex the output of the SequencerSection that will operate with the message, 0 if not given
        */
        void trigVector( std::vector<float> vect, double gateLength, int outputIndex = 0);

        /*!
        @brief call begin() before calling this function. This function will add all the values of the array with the given division and will also add a 0.0f value after each value. The distange of the 0.0f value is given by gateLength. Only values greater than 0.0f are used. All the values are multiplied for multiply.
        @param[in] vect std::vector<float> of values to add to the sequence, negative values will be ignored
        @param[in] gateLength length of gate = division * gateLength
        @param[in] outputIndex the output of the SequencerSection that will operate with the message, 0 if not given
        @param[in] multiply multiply all the messages for this value, useful for scaling for external sequencers
        */
        void trigVector( std::vector<float> vect, double gateLength, int outputIndex, float multiply );

        /*!
        @brief call begin() before calling this function. This function will add all the values. This function will add values ranging from start to right before stop value, from the stepStart to the stepStop
        @param[in] stepStart step of the first value
        @param[in] valueStart start value, this is the value of the first step
        @param[in] stepStopExclusive step of the last value, exclusive
        @param[in] valueStopExclusive stop value, the destination value that is not reached
        @param[in] output the output of the SequencerSection that will operate with the message, 0 if not given
        @param[in] granularity the number of intermediate messages used from a step to another, 1 is the default value. An higher number will make smoother value transition, 1 is staircase-like and is good just for percussive rolls.
        */
        void line(double stepStart, float valueStart, double stepStopExclusive, float valueStopExclusive, int output = 0, int granularity=1 );

        /*!
        @brief returns a read-only reference to the internal SequencerMessage vector
        */        
        const std::vector<SequencerMessage> & getScore() const;
        
        /*!
        @brief if the returned number is different than the last one you've got, then a change happened in the sequence and now is safe to access the score
        */
        int getChangeID() const;
        

        /*!
        @brief public access to read and set the step length, initializated to 1.0/16.0 ( one sixteenth ).
        */     
        std::atomic<double> steplen;

        // THIS IS DEPRECATED, but adding a deprecated message will always trigger messages from backward compatibility
        std::atomic<double> & bars;
/*!
    @endcond 
*/

    private:
        std::atomic<float> atomic_meter_percent;
        std::atomic<int> loopCounter;
        std::atomic<bool> modified;
        
        int id;
        int currentOutput;
        double currentDelay;
                
        std::vector<SequencerMessage> score;   
        std::vector<SequencerMessage> nextScore;
        
        void executeGenerateScore() noexcept;
                
        void message(double step, float value, int outputIndex, MessageType mtype ) noexcept;

        static double defaultSteplen;
        
    };
    
    
    // ---------------------------------------------------------------
    
    /*!
    @brief class for managing sequencing of Sequence
    This class has a assignable lambda function code() that returns an integer for the next SequencerSection index to be started. By default it returns the last used index, looping the Sequence or SequencerCell
    */    
    class SeqChange {
    
    public:
    
        SeqChange();
        /*!
        @brief lambda function to assign for selecting the next Sequencer / SeqCell to start
        Remember that this function will be executed into the audio-thread so the access to some variable used also into the main thread could cause race conditions.
        */
        std::function<int()> code;
    
    
        virtual int getNextPattern( int currentPattern, int size ) noexcept;
    
    private:
        int self;
    };

}




#endif // PDSP_SEQUENCE_H_INCLUDED
