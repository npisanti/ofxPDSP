
#ifndef MUSIC_H_INCLUDED
#define MUSIC_H_INCLUDED

#include "ofxPDSP.h"

// this is a test patter with some gate on/off messages
struct TestPattern : public pdsp::ScoreCell{
    
    TestPattern(double division){
        for(int i=0; i<8; ++i){
            double time = i/division ;
            score.push_back( pdsp::ScoreMessage( time,           0.75f ) );
            score.push_back( pdsp::ScoreMessage( time + 0.035,   0.0f  ) );
        } 
    }
    TestPattern() : TestPattern(32.0f){}
};


struct MusicTest{

    MusicTest() : 
        testPattern1(8.0), 
        testPattern2(16.0)
    {
        //SETUP
        // setup as usual, check example-scoring for more info
        scoregen.setTempo(172.0);
        
        scoregen.sections[0].setOutputsNumber(3); // gate + pitch + cc
        scoregen.sections[0].setPattern(0, &testPattern1, pdsp::Behavior::Self);
        scoregen.sections[0].setCellTiming( 0, 1.0, true, 1.0 );
        scoregen.sections[0].setPattern(1, &testPattern2, pdsp::Behavior::Self);
        scoregen.sections[0].setCellTiming( 1, 2.0, false, 2.0 );
        scoregen.sections[0].setPattern(2, &testPattern3, pdsp::Behavior::Self);
        scoregen.sections[0].setCellTiming( 2, 1.0, true, 2.0 );


        // you connect the ScoreSections to midiOut like this
        // outputs a midi on on a message with value > 0.0f and a midi off message for a value <= 0.0f
        // first argument of gate() is midi channel, second the default note
        scoregen.sections[0].out(0) >> midiOut.gate(1, 60); 
        // if you patch an out to a ,note() this out will control the note number of the last patched gate()
        // so beware! use gate() and note() always one after the other
        scoregen.sections[0].out(1) >> midiOut.note();
        // you can also connect the out to a midi cc like this
        // first argument of cc() is midi channel, second the  midi CC number to send
        scoregen.sections[0].out(2) >> midiOut.cc(1, 2);
        
        // in this example we are sending messages just to gate, so they will always be on the same note
        
        midiOut.setVerbose(true);
        midiOut.listPorts();
        midiOut.openPort(0);
        
        scoregen.sections[0].launchCell(0);
    };
    
    void release(){
        midiOut.closePort();
    }
    
    void process(int bufferSize){
        scoregen.process(bufferSize);
        midiOut.process();
    }
    
    TestPattern     testPattern1;
    TestPattern     testPattern2;
    TestPattern     testPattern3;
    
    pdsp::ScoreProcessor scoregen;  
    pdsp::GateSequencer gate;
    ofxMidiOutProcessor midiOut;
};




#endif // MUSIC_H_INCLUDED
