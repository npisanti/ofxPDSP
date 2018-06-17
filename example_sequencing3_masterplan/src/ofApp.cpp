#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    // ---------------------SEQUENCING------------------------
    engine.sequencer.init( 3, 5, 200.0f); // sections, sequences, tempo
    
    states.resize(16);
    
    // a good tecnique for arranging with pdsp is using the first sequence as "masterplan"
    // as the sequence codes are executed from the first to the last a sequence can launch the successive sequence
    // remember to launch the other sequences with some quantization for correct timing
    // this example runs the first sequence each 1/8th, but you can also use slower masterplan
    // the masterplan has just to be as fast as the granularity of control you want to have on the sequencing
    // (in this case the masterplan controls the beatslicing, so it has to be fast)
        
    // pdsp::Sequence counter() returns how many time the sequence has restarted
    // and pdsp::Sequence resetCount() resets the counter

    engine.sequencer.sections[0].resizeCells(1);        
    engine.sequencer.sections[0].sequence(0).label = "masterplan";
    
    engine.sequencer.sections[0].sequence(0).bars =  1.0 / 8.0;
    engine.sequencer.sections[0].sequence(0).code = [&] () noexcept { // masterplan sequence
        
        pdsp::Sequence & seq = engine.sequencer.sections[0].sequence(0); // defining variable into the lambda is fine
        
        if(seq.counter()==64){ // 64 8th = 8 bars
            seq.resetCount();
        }
        
        if(seq.counter()==0){  // here we generate some random numbers to select the drums cells
            states[0] = 0; // the first state is always a kick
            for(size_t i=1; i<states.size();++i){
                states[i] = pdsp::dice(5);
            }
        }
        
        int index = seq.counter()%16; // we repeat the same states four time, so each pattern is 2 bars

        // launch the drum sequence, quantized to this masterplan
        engine.sequencer.sections[1].launchCell( states[index], true, seq.length());
        
        // on the first six bars the main riff will run
        if(seq.counter()==0)   engine.sequencer.sections[2].launch( 0, true, seq.length());
        // on the last two bars we will use a fill choosen at random
        if(seq.counter()==48)  engine.sequencer.sections[2].launch( pdsp::dice(1, 5), true, seq.length()); 
    };
    
    // P.S. obviously we could have done all the beatslicing by coding a single sequence,
    //                                                       but this is not the point of this example

    float o = -1.0f;
    
    // drums sequence "cells" 
    // first out is trigger, second is sample index select
    
    engine.sequencer.sections[1].sequence(0).label = "kick";
    engine.sequencer.sections[1].sequence(0).set( { { 0.7f, o   },
                                                    { 0.0f, o } });
    engine.sequencer.sections[1].sequence(1).label = "snare";
    engine.sequencer.sections[1].sequence(1).set( { { 0.7f, o },
                                                    { 1.0f, o } });
    engine.sequencer.sections[1].sequence(2).label = "hh";
    engine.sequencer.sections[1].sequence(2).set( { {  1.0f, o },
                                                    {  2.0f, o } });
    engine.sequencer.sections[1].sequence(3).label = "hh soft";
    engine.sequencer.sections[1].sequence(3).set( { {  0.5f, o },
                                                    {  2.0f, o } } );
    engine.sequencer.sections[1].sequence(4).label = "shuffle";
    engine.sequencer.sections[1].sequence(4).set( { { 0.7f, 0.7f },
                                                    { 2.0f, 3.0f } });

    // reese bass loops 
    engine.sequencer.sections[2].sequence(0).label   = "main";
    engine.sequencer.sections[2].sequence(0).steplen = 1.0/4.0;
    engine.sequencer.sections[2].sequence(0).bars    = 2.0;
    engine.sequencer.sections[2].sequence(0).set( { { 1.0f,  o,  o,  o,  o,  o,   o,   0.f },
                                                    { 29.0f, o,  o,  o,  o,  o,  31.f,  o   } });
                                                    
    engine.sequencer.sections[2].sequence(1).label   = "fill 1";
    engine.sequencer.sections[2].sequence(1).steplen = 1.0/4.0;
    engine.sequencer.sections[2].sequence(1).bars    = 2.0;
    engine.sequencer.sections[2].sequence(1).set( { { 1.0f,  o,  o,  o, 0.0f },
                                                   {  29.0f, o,  o,  o,  o } });
                                                
    engine.sequencer.sections[2].sequence(2).label   = "fill 2"; 
    engine.sequencer.sections[2].sequence(2).steplen = 1.0/4.0;
    engine.sequencer.sections[2].sequence(2).bars    = 2.0;
    engine.sequencer.sections[2].sequence(2).set( { { 0.0f,  o, 1.0f,  o,  o,  o,   o,  o},
                                                     { o,    o,  32.f,  o, 31.f,  o,  30.f,  o   } } );
                                                
    engine.sequencer.sections[2].sequence(3).label   ="fill 3";  
    engine.sequencer.sections[2].sequence(3).steplen = 1.0/8.0;
    engine.sequencer.sections[2].sequence(3).bars    = 2.0;
    engine.sequencer.sections[2].sequence(3).set( { { 1.0f,  o,  0.f,   1.f,  o,   o,   0.0f,  o },
                                                    { 29.0f, o,  o,  o, 34.f, o,  33.0f,   o   } } );
                                                
    engine.sequencer.sections[2].sequence(4).label   = "fill 4"; 
    engine.sequencer.sections[2].sequence(2).steplen = 1.0/4.0;
    engine.sequencer.sections[2].sequence(2).bars    = 2.0;
    engine.sequencer.sections[2].sequence(4).set( { { 1.0f,  o,  o,  o,  o,  o,   o,   0.f },
                                                    { 32.0f, o,  o,  o,  o,  o,  31.f,  o   } } );
 
    // launch masterplan
    engine.sequencer.sections[0].launchCell(0);
    
    
    // ---------------------PATCHING------------------------
    drums.add( ofToDataPath("break_kick.wav") );
    drums.add( ofToDataPath("break_snare.wav") );
    drums.add( ofToDataPath("break_hh.wav") );
    drums.add( ofToDataPath("break_ghost.wav") );
    3.0f >> drums.in("pitch");

    engine.sequencer.sections[1].out_trig(0)  >> drums.in("trig");
    engine.sequencer.sections[1].out_value(1) >> drums.in("select");
    drums * dB(9.0f) >> engine.audio_out(0);
    drums * dB(9.0f)  >> engine.audio_out(1);
    
    engine.sequencer.sections[2].out_trig(0)  >> reese.in("trig");
    engine.sequencer.sections[2].out_value(1) >> reese.in("pitch");
    reese * dB(-8.0f) >> engine.audio_out(0);
    reese * dB(-8.0f) >> engine.audio_out(1);

    reeseScope.set(4096); // this will make the scope buffersize longer    
    drums * dB(9.0f)  >> drumScope  >> engine.blackhole(); // scopes need to be patched to engine.blackhole()
    reese * dB(-6.0f) >> reeseScope >> engine.blackhole(); // blackhole() process module without outputting sound
    
    // -------------------GRAPHIC SETUP--------------
    ofBackground(0);
    ofSetFrameRate(60);
    ofDisableAntiAliasing();
    
    engine.graphics.setup( 600, 600,  { 0,   0 }, 
                                      { 0,   0 }  );
    engine.graphics.setPosition( 10, 10 );
    engine.graphics.setColor (ofColor( 255 ) );
    
    ofSetWindowTitle("pdsp example-scoring3-masterplan");
    
    // ------------SETUPS AND START AUDIO-------------
    engine.listDevices();
    engine.setDeviceID(0); // REMEMBER TO SET THIS AT THE RIGHT INDEX!!!!
    engine.setup( 44100, 512, 3); 
    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    engine.graphics.draw();
    
    drumScope.draw(  10,  100, 290, 80);
    reeseScope.draw( 320, 100, 290, 80);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if( key == ' '){
        if(engine.sequencer.isPlaying()){
            engine.sequencer.stop();
        }else{
            engine.sequencer.play();
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
