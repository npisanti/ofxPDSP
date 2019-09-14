
// Sequence.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016 - 2019

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
        std::atomic<double> bars;
        
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
        pdsp::Sequence & begin() noexcept;

        /*!
        @brief you call end() when you have finished adding value with Message(). When the Sequence restarts the new sequence will be played.
        */
        pdsp::Sequence & end() noexcept;
        
        /*!
        @brief sets the output for the next banged message
        @param[in] value the output's index
        */    
        pdsp::Sequence & out( int value ) noexcept;

        /*!
        @brief sets the delay from the start of the sequence of the next message, in bars
        @param[in] value the delay of the message
        */            
        pdsp::Sequence &  delay( double value ) noexcept;
        
        /*!
        @brief outputs a message. You can use out() to choose the output or delay() to change the timing
        @param[in] value the message value
        */ 
        pdsp::Sequence &  bang( float value ) noexcept;

        /*!
        @brief outputs a legato message. You can use out() to choose the output or delay() to change the timing. This message won't retrigger envelopes like ADSR, instead the envelope will decay to the given value. Only works if the selected output is patched as trigger output.
        @param[in] value the message value
        */ 
        pdsp::Sequence &  legato( float value ) noexcept;
        
        /*!
        @brief outputs a slew message. You can use out() to choose the output or delay() to change the timing. Smooths out next messages with the give time in milliseconds. Only works if the selected output is patched as value output.
        @param[in] value the message value
        */ 
        pdsp::Sequence &  slew( float milliseconds ) noexcept;

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
    
        //[[deprecated("deprecated, use the new sequence API as in the examples")]]
        [[deprecated("deprecated, use the new sequence API as in the examples")]]      
        void message(double step, float value, int outputIndex=0) noexcept;
                
        [[deprecated("deprecated, use the new sequence API as in the examples")]]        
        void set( std::initializer_list<float> init ) noexcept;

        [[deprecated("deprecated, use the new sequence API as in the examples")]]
        void set( std::initializer_list<std::initializer_list<float> >  init ) noexcept;

        [[deprecated("deprecated, use the new sequence API as in the examples")]]
        void set( std::initializer_list<float> init, double division, double length  ) noexcept;

        [[deprecated("deprecated, use the new sequence API as in the examples")]]
        void set( std::initializer_list<std::initializer_list<float> >  init , double division, double length  ) noexcept;

        [[deprecated("deprecated, better not to use this anymore")]]
        int meter_step() const; 

        [[deprecated("deprecated, use the new sequence API as in the examples")]]
        void setDivision( double value );
        
        [[deprecated("deprecated, use the new sequence API as in the examples")]]
        void setLength( double bars );
        
        [[deprecated("deprecated, use the new sequence API as in the examples")]]
        double length() const;

        [[deprecated("deprecated, use the new sequence API as in the examples")]]
        double division() const;

        [[deprecated("deprecated, use the new sequence API as in the examples")]]
        void setTiming( double division, double bars );

        [[deprecated("deprecated, use the new sequence API as in the examples")]]
        void begin( double division, double length ) noexcept;

        [[deprecated("deprecated, use the new sequence API as in the examples")]]
        void messageVector( std::vector<float> vect, int outputIndex = 0);

         [[deprecated("deprecated, use the new sequence API as in the examples")]]
        void trigVector( std::vector<float> vect, double gateLength, int outputIndex = 0);

        [[deprecated("deprecated, use the new sequence API as in the examples")]]
        void trigVector( std::vector<float> vect, double gateLength, int outputIndex, float multiply );

        [[deprecated("deprecated, use the new sequence API as in the examples")]]
        void line(double stepStart, float valueStart, double stepStopExclusive, float valueStopExclusive, int output = 0, int granularity=1 );
      
        [[deprecated("deprecated, better not to use this anymore")]]
        const std::vector<SequencerMessage> & getScore() const;
        
        
        // this is deprecated but still used for backward compatibility
        // a deprecate message will generate unwanted warnings
        std::atomic<double> steplen;
        

        // this is not deprecated, just hidden from docs!
        int getChangeID() const;
        
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
        std::vector<const char*> * tags;
                
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
