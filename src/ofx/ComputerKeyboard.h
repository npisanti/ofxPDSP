
// ComputerKeyboard.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef OFXPDSP_COMPUTERKEYBOARD_H_INCLUDED
#define OFXPDSP_COMPUTERKEYBOARD_H_INCLUDED

#include "../messages/header.h"
#include "Parameter.h"
#include "../DSP/control/TriggerControl.h"
#include "helper/MidiKeysBuffers.h"
/*!
@brief utility class to use your computer keyboard as an 1-octave musical keyboard
*/

namespace pdsp{

class ComputerKeyboard {
public:
        ComputerKeyboard();
           
        void keyPressed  ( int key );
        void keyReleased ( int key );

        /*!
        @brief set the mode to Poly, with the given max notes
        @param[in] maxNotes max note for the polyphony, 4 is the minimum
        
        Note stealing will steal the oldest note that are not the highest or the lowest playing notes
        */              
        void setPolyMode( int voices );
     

        /*!
        @brief set the mode to Mono, with the given control parameters
        @param[in] legato true if you want to retrigger the envelopes only when all the key are released
        @param[in] priority you can use Last, Low or High as parameter
        */       
        void setMonoMode( bool legato, pdsp::MonoPriority priority = pdsp::Last );

        /*!
        @brief returns the actual voice number 
        */
        int getVoicesNumber() const;
                
        /*!
        @brief sets the portamento parameters
        @param[in] portamentoMode Off or On
        @param[in] timeMs slew time in milliseconds
        */     
        void setPortamento( pdsp::PortamentoMode mode, float timeMs );

        /*!
        @brief draws some graphics and infos about the state of the object
        
        @param[in] x x coordinate
        @param[in] y y coordinate
        @param[in] w width
        @param[in] h height
        */    
        void draw(int x, int y, int w, int h);

        /*!
        @brief this vector holds 12 int values that rapresents the keys that triggers the notes. By default it is initalized to the keys of a qwerty keyboard, but you can change that manually 
        */   
        vector<int> keys;

        /*!
        @brief returns the trigger output of the given voice
        @param[in] voice voice number. It won't be clamped, so use a valid voice number to avoid segfaults.
        */
        pdsp::TriggerControl & out_trig( int voice );

        /*!
        @brief returns the pitch output of the given voice
        @param[in] voice voice number. It won't be clamped, so use a valid voice number to avoid segfaults.
        */
        pdsp::Parameter &  out_pitch( int voice );

/*!
    @cond HIDDEN_SYMBOLS
*/ 
        /*!
        @brief vector of trigger outputs
        */                
        vector<pdsp::TriggerControl> outs_trig;
        
        /*!
        @brief vector for the pitch outputs
        */
        vector<pdsp::Parameter>        outs_pitch;
/*!
    @endcond
*/ 

private:
        void keysControl( int key );

        void setVoiceMode( pdsp::VoiceMode mode);
        void setMonoPriority( pdsp::MonoPriority mode);
        void setMaxNotes(int notes);
        

        void processPolyNoteOn( int pitch ) noexcept;
        void processPolyNoteOff( int pitch ) noexcept;
        void processMonoNoteOn( int pitch ) noexcept;
        void processMonoNoteOff( int pitch ) noexcept;
        
        int noteSteal();
        int getHighestPriorityMono();

        void checkIdGeneration();
        void clearNotes();
        

        int     eventIdGenerator;
        int     activeNotes;
        int     maxNotes;
        int     monoNoteIndex;
        
        std::vector<int> keysPressed;
        std::vector<int> keysWhite;
        
        std::vector<helper::NoteState_t>             notes;
        
        bool legato;
  
        VoiceMode               voiceMode;
        MonoPriority            monoMode;

        int basePitch;
        float dynamic;
};       
    
}
    

#endif // OFXPDSP_COMPUTERKEYBOARD_H_INCLUDED
