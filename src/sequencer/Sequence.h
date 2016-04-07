
// Sequence.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_SEQUENCE_H_INCLUDED
#define PDSP_SEQUENCE_H_INCLUDED

#include "ScoreCell.h"
#include <functional>
#include <atomic>
#include <iostream>
#include "../DSP/pdspCore.h"

namespace pdsp{
    /*!
    @brief powerful class for sequencing message. 
    Sequence is a class and container for values to be sequenced. You can set the value to sequence from an inline array or a std::vector. It also has an assignable lambda function that is executed each time the Sequence starts.
    */    
    class Sequence : public ScoreCell {

    public:
        Sequence( double stepDivision );
        Sequence();
        Sequence(const Sequence & other);
        Sequence(Sequence && other);
        Sequence& operator= (const Sequence & other);
        Sequence& operator= (Sequence && other);
        
        /*!
        @brief sets the time division to be used for each step. For example 8.0f means each step is 1/8th. Default division is 1/16th.
        @param[in] value time division
        */
        void setDivision( double value );
        
        /*!
        @brief sets the time in bars before the next ScoreCell/Sequence will be launched. Messages with a time greater than this won't be played.
        @param[in] value new length
        */
        void setLength( double value );
        
        /*!
        @brief sets the sequence from an inlined array. Negative values produce no messages.
        @param[in] init an inline array, for example {1.0f, 0.0f, -1.0f, -1.0f, 0.25f, 0.5f, 0.0f, -1.0f }
        */
        void set( std::initializer_list<float> init ) noexcept;

        /*!
        @brief sets the sequence from an inlined array. Negative values produce no messages.
        @param[in] init an inline array, for example {1.0f, 0.0f, -1.0f, -1.0f, 0.25f, 0.5f, 0.0f, -1.0f }
        @param[in] value time division
        @param[in] length time before the next Sequence/ScoreCell will be started
        */
        void set( std::initializer_list<float> init, double division, double length ) noexcept;

        /*!
        @brief sets the sequence from a float vector. Negative values produce no messages.
        @param[in] init a std:vector<float> for setting the Sequence
        */
        void set(const std::vector<float> &init ) noexcept;
        
        /*!
        @brief sets the sequence from a float vector. Negative values produce no messages.
        @param[in] init a std:vector<float> for setting the Sequence
        @param[in] value time division
        @param[in] length time before the next Sequence/ScoreCell will be started
        */
        void set(const std::vector<float> &init, double division, double length  ) noexcept;
        
        
        /*!
        @brief you call begin() before calling message, this prepare the Sequence for the message() method, clearing the buffers.
        */
        void begin() noexcept;

        /*!
        @brief with this method you can manually add timed values to the sequence. You have to call begin() before adding messages and end() when you've done. Also note that the old Sequence values are not kept so you are adding values to an empty sequence.
        @param[in] step the step index 
        @param[in] value the value of the step
        @param[in] outputIndex when using message Sequence can transmit values to more than one GateSequencer/ValueSequencer, see ScoreCell for more
        */
        void message(double step, float value, int outputIndex=0) noexcept;
        
        /*!
        @brief you call end() when you have finished adding value with Message(). When the Sequence restarts the new sequence will be played.
        */
        void end() noexcept;

        /*!
        @brief this lambda function is executed each time the Sequence starts from the begin. Assign your own functions to it.
        This lambda function is executed each time the Sequence starts from the begin. Usually is empty, but you can assign your own lambdas to generate new values each time the Sequence starts. Remember that the code executed can make a previosly called set() method ininfluent. Remember that this function will be executed into the audio-thread so the access to some variable used also into the main thread could cause race conditions.
        */
        std::function<void()> code;

    private:
        std::vector< pdsp::ScoreMessage > nextScore;
        std::atomic<bool> modified;

        void generateScore() noexcept override ;    
        double div;
        double divMult;

    };
    
    /*!
    @brief class for managing sequencing of Sequence
    This class has a assignable lambda function code() that returns an integer for the next ScoreSection index to be started. By default it returns the last used index, looping the Sequence or ScoreCell
    */    
    class SeqChange : public CellChange{
    
    public:
    
        SeqChange();
        /*!
        @brief lambda function to assign for selecting the next Sequencer / ScoreCell to start
        Remember that this function will be executed into the audio-thread so the access to some variable used also into the main thread could cause race conditions.
        */
        std::function<int()> code;
    
    private:
        int getNextPattern( int currentPattern, int size ) noexcept override;
        int self;
    };

}




#endif // PDSP_SEQUENCE_H_INCLUDED
