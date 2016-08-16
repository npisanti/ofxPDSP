
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"


struct BassPattern : public pdsp::Sequence{
    
    // helper routine to add notes to the score
    // this routin also add a message for stopping the note
    // so we have to be careful that notes durations don't overlap
    void note(double step16, float gate, float pitch, float slew, double duration){    
        message( step16 ,           gate,  0  ); // adds a trigger on to the gate output
        message( step16,            pitch, 1  ); // adds a value to the pitch output
        message( step16 ,           slew,  2  ); // adds a trigger on to the gate output
        message( step16 + duration, 0.0f,  0 );  // adds a trigger off value to the gate output
    }
    
    // this routine shuffles the pitches inside of the sequence vector
    void shuffleSequence(){
        int seqLen = static_cast<int> (sequence.size());
        int index1 = pdspDice(seqLen);
        int index2 = index1 + pdspDice(seqLen-1);
        while(index2 >= seqLen){ index2 -= seqLen; }
        float temp = sequence[index1];
        sequence[index1] = sequence[index2];
        sequence[index2] = temp;
    }

    
    // this returns the pitches for the generative routine
    // returns the pitches from the sequence the first, second and third bar
    // on the fourth bar the second part of the returned pitches will be random values
    // counter() returns the value of an internal counter that measure how many time the sequence has been looped
    float pfun(int index){
        if(index>4 && counter() == 3){
            float nextPitch = static_cast<float> (pdspDice(12) + 41.0f); 
            return nextPitch;            
        }else{
            return sequence[index];
        }
    }     
    

    //inits the pattern and set the pitches to use
    BassPattern(){

        sequence.resize(8);
        sequence[0] = 29.0f;
        sequence[1] = 31.0f;
        sequence[2] = 34.0f;
        sequence[3] = 36.0f;
        sequence[4] = 38.0f;
        sequence[5] = 41.0f;
        sequence[6] = 43.0f;
        sequence[7] = 29.0f;
        
        code = [&] () noexcept {
            if (counter() == 4 ) resetCount(); // pdsp::Sequence has an internal counter that you can reset with resetCount() 
                                            // this counter is automatically reset on Sequence change or launch

            shuffleSequence();            

            begin(16.0, 1.0);
         
            //   step   velo    pitch       slew%    duration
            note(0.0,   1.0f,   29.0f,      0.0f,    gate_long);
            note(2.0,   0.5f,   pfun(0),    0.0f,    gate_long);
            note(4.0,   0.5f,   pfun(1),    1.0f,    gate_short);
            note(6.0,   1.0f,   pfun(2),    0.0f,    gate_long);
            note(8.0,   0.5f,   pfun(3),    2.0f,    gate_long);
            note(10.0,  1.0f,   pfun(4),    0.0f,    gate_short);        
            note(11.0,  0.5f,   pfun(5),    0.0f,    gate_short);        
            note(12.0,  0.5f,   pfun(6),    0.0f,    gate_short);        
            note(13.0,  0.5f,   pfun(7),    0.0f,    gate_short);
           
            end();
                      
        };
        
    }

    const double gate_long =  0.95;  // a bit more than 1/16       
    const double gate_short = 0.4; // almost 1/32th
    vector<float> sequence;
    
};



