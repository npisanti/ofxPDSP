
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"

// this is the code for generating an sequencing values for the DSPs

// pdsp use ScoreSections to sequence basic generative unit called ScoreCells
// a ScoreSection has an internal vector of ScoreCells and their relative
// timing, and CellChanges for knowing what to do when a ScoreCell end its length
// A ScoreProcessor contains many ScoreSections and keeps the global clock.

// pdsp::ScoreCell is the basic sequencing unit, it has a score variable
// that is a std::vector of ScoreMessage
// ScoreMessage has 3 parameters: 
//      time (in bars, relative to the ScoreCell start)
//      value (the value to be output at the right time)
//      lane ( ScoreCell are used by ScoreSection with multiple outs, lane select the out)
// remember to check the pdsp::ScoreCell documentation for more!!!

// this is a basic static ScoreCell
// it adds messages only in the constructor
// so message will not change and will not be generate in realtime
// (if you don't manually change the score, being careful of changing it in a thread-safe way)
struct BleepPattern : public pdsp::ScoreCell{
    BleepPattern(float pitch){
        score.push_back( pdsp::ScoreMessage(0.0, 1.0f,  0) ); // trigger
        score.push_back( pdsp::ScoreMessage(0.0, pitch, 1) ); // pitch
    }
    
    BleepPattern() : BleepPattern(69.0f){}
};

// a CellChange object select the next cell to be launched when a ScoreCell is launched
// each ScoreCell into the ScoreSection can have a linked CellChange
// otherwise the ScoreSection will stop playing after the launched cell
struct BleepSequencer : public pdsp::CellChange {
    BleepSequencer () {
        oneShot.store(false);
    }
    // getNextPattern is the method you have to override 
    // to get the index of the next ScoreCell into the ScoreSection
    // in this case this CellChange can either return -1 (stopping the sequencing of cells)
    // or return the next cell in cycle
    int getNextPattern( int currentPattern, int size ) override{ 
        if(oneShot) {
            return -1;
        }else{
            int nextPat = currentPattern +1;
            if (nextPat == 4) nextPat = 0;
            return nextPat;
        }
    }
    
    atomic<bool> oneShot;
};


// this is a more complex ScoreCell, it generates its score each time is started
struct BassPattern : public pdsp::ScoreCell{
    
    // helper routine to add notes to the score
    // this routin also add a message for stopping the note
    // so we have to be careful that notes durations don't overlap
    void note(double step16, float gate, float pitch, float slew, double duration){
        double time = step16 * 0.0625;
        score.push_back( pdsp::ScoreMessage( time ,           gate,  0 ) ); // adds a trigger on to the gate output
        score.push_back( pdsp::ScoreMessage( time,            pitch, 1 ) ); // adds a value to the pitch output
        score.push_back( pdsp::ScoreMessage( time ,           slew,  2 ) ); // adds a trigger on to the gate output
        score.push_back( pdsp::ScoreMessage( time + duration, 0.0f,  0) );  // adds a trigger off value to the gate output
    }
    
    // this routine shuffles the pitches inside of the sequence vector
    void shuffleSequence(){
        int seqLen = static_cast<int> (sequence.size());
        int index1 = rand()%seqLen;
        int index2 = index1 + rand()%(seqLen-1);
        if(index2 >= seqLen) index2 -= seqLen;
        float temp = sequence[index1];
        sequence[index1] = sequence[index2];
        sequence[index2] = temp;
    }

    //inits the pattern and set the pitches to use
    BassPattern(){
        updateScoreDraw = false;
        counter = 0;
        sequence.resize(8);
        sequence[0] = 29.0f;
        sequence[1] = 31.0f;
        sequence[2] = 34.0f;
        sequence[3] = 36.0f;
        sequence[4] = 38.0f;
        sequence[5] = 41.0f;
        sequence[6] = 43.0f;
        sequence[7] = 29.0f;
    }
    
    // generateScore is ScoreCell method you have to override 
    // to change/generate its messages each time the cell is launched
    void generateScore(const float &length) noexcept override {

        shuffleSequence();            

        score.clear();
        //   step   velo    pitch       slew%    duration
        note(0.0,   1.0f,   29.0f,      0.0f,    gate_long);
        note(2.0,   0.5f,   pfun(0),    0.0f,    gate_long);
        note(4.0,   0.5f,   pfun(1),    1.0f,    gate_short);
        note(6.0,   1.0f,   pfun(2),    0.0f,    gate_long);
        note(8.0,   0.5f,   pfun(3),    1.0f,    gate_long);
        note(10.0,  1.0f,   pfun(4),    0.0f,    gate_short);        
        note(11.0,  0.5f,   pfun(5),    0.0f,    gate_short);        
        note(12.0,  0.5f,   pfun(6),    0.0f,    gate_short);        
        note(13.0,  0.5f,   pfun(7),    0.0f,    gate_short);
        
        if(++counter == 4) counter = 0;

        updateScoreDraw = true; // now is safe to update the bass pattern graphics
    }

    // this returns the pitches for the generative routine
    // returns the pitches from the sequence the first, second and third bar
    // on the fourth bar the second part of the returned pitches will be random values
    float pfun(int index){
        if(index>4 && counter == 3){
            float nextPitch = static_cast<float> (pdsp::randomUnipolarInt(12) + 41.0f); 
            return nextPitch;            
        }else{
            return sequence[index];
        }
    }     
    
    bool updateScoreDraw; // flag for thread-safe pattern graphics visualization
    
    int counter;
    const double gate_long =  0.08;  // a bit more than 1/16       
    const double gate_short = 0.035; // almost 1/32th
    vector<float> sequence;
};


// this struct set up the ScoreProcessor and ScoreSection and inits the Cells
struct MusicTest{

    MusicTest() : 
        bleep1(77.0f),
        bleep2(80.0f),
        bleep3(84.0f),
        bleep4(89.0f)
    {
        // ScoreProcessor setup
        scoregen.setTempo(120.0);
        scoregen.setMaxBars(16.0);    
        scoregen.setSections(2);
        
        // adding the bleep patterns and settings their timings
        scoregen.sections[0].setPattern(0, &bleep1, &bleepSeq );
        scoregen.sections[0].setPattern(1, &bleep2, &bleepSeq );
        scoregen.sections[0].setPattern(2, &bleep3, &bleepSeq );
        scoregen.sections[0].setPattern(3, &bleep4, &bleepSeq );
        //set up launch timigs           index length quant grid  
        scoregen.sections[0].setCellTiming(0,   0.25, true, 0.25 );
        scoregen.sections[0].setCellTiming(1,   0.25, true, 0.25 );
        scoregen.sections[0].setCellTiming(2,   0.25, true, 0.25 );
        scoregen.sections[0].setCellTiming(3,   0.25, true, 0.25 );
        // quant = quantized launch, grid = grid to quantize, 0.25 is 1/4th bars 
        // so they will trigger one after the other each 1/4th bars
        
        scoregen.sections[0].setOutputsNumber(2); // two outputs, 0 = gate, 1 = pitch
        scoregen.sections[0].out(0) >> bleepGate; // a GateSequencer turns the score messages into trig values for the DSPs
        scoregen.sections[0].out(1) >> bleepPitch;// a ValueSequencer turns the score messages into values for the DSPs
        // GateSequencer and ValueSequencer are a sample-accurate bridge between scored messages and DSPs
    

        scoregen.sections[1].setPattern(0, &bassPattern, pdsp::Behavior::Self); //pdsp::Behavior contains some ready-made CellChange
        scoregen.sections[1].setCellTiming( 0, 1.0, true, 1.0 ); // lenght = 1bar, nextCell quantized to the next bar
        scoregen.sections[1].setPattern(1, nullptr, nullptr); 
        scoregen.sections[1].setOutputsNumber(3); // 0 = gate, 1 = pitch, 2 = slew control
        scoregen.sections[1].out(0) >> bassGate;      
        scoregen.sections[1].out(1) >> bassPitch;        
        scoregen.sections[1].out(2) >> bassPitch.in_slew(); // this way you can use message to control ValueSequencer slew time 
                                                            // slew time is multiplied by the message value
                                                            // so for example 0.0f deativates slew and 2.0f doubles it
                                                            // (the effect is subtle in this example but it's there)
        bassPitch.setSlewTime(80.0f); // 50ms slew

        // we launch the cells
        scoregen.sections[0].launchCell(0);
        scoregen.sections[1].launchCell(0);
    };
    
    BassPattern     bassPattern;
    
    BleepPattern    bleep1;
    BleepPattern    bleep2;
    BleepPattern    bleep3;
    BleepPattern    bleep4;
    
    pdsp::ScoreProcessor scoregen;  

    pdsp::GateSequencer  bassGate;
    pdsp::ValueSequencer bassPitch;
        
    pdsp::GateSequencer  bleepGate;
    pdsp::ValueSequencer bleepPitch;
    
    BleepSequencer       bleepSeq;

};

