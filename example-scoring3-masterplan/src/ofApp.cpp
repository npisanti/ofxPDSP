#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    // ---------------------SEQUENCING------------------------
    engine.score.init( 3, 5, 200.0f); // sections, sequences, tempo

    engine.score.sections[0].label(0, "masterplan" );
    engine.score.sections[0].resizeCells(1);
    
    engine.score.sections[0].sequence(0).setLength( 1.0 / 8.0);
    
    states.resize(16);
    
    // a good tecnique for arranging with pdsp is using the first sequence as "masterplan"
    // as the sequence codes are executed from the first to the last a sequence can launch the successive sequence
    // remember to launch the other sequences with the same quantization of the first sequence's length
    // this example runs the first sequence each 8th, but you can also use slower masterplan
    // the masterplan has just to be as fast as the granularity of control you want to have on the sequencing
    // in this case the masterplan controls the beatslicing, so it has to be fast
    // you can also more than one sequence to control other sequence, it just need to have a lower index than 
    // the other sequences you want to control.
    // Ideally you select between different generative sequence or set sequence paramenters in the masterplan
        
    // in arranging the counter() and resetCount() methods of pdsp::Sequence are really useful, check them out
    
    // another advice: NEVER USE variables local to the setup() method inside the sequence code lambda function!
    // the life of those variable will end when setup() ends, so you will get UNDEFINED BEHAVIOR
    // you will be fine using only variable you have defined the .h file

    engine.score.sections[0].sequence(0).code = [&] () noexcept { // masterplan sequence
        
        pdsp::Sequence & seq = engine.score.sections[0].sequence(0); // defining variable into the lambda is fine
        
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
        engine.score.sections[1].launchCell( states[index], true, seq.length());
        
        // on the first six bars the main riff will run
        if(seq.counter()==0)   engine.score.sections[2].launchCell( 0, true, seq.length());
        // on the last two bars we will use a fill choosen at random
        if(seq.counter()==48)  engine.score.sections[2].launchCell( pdsp::dice(1, 5), true, seq.length()); 
    };
    
    // P.S. obviously we could have done all the beatslicing by coding a  single sequence,
    //                                                       but this is not the point of this example

    
    float o = -1.0f;
    
    // drums sequence "cells"
    engine.score.sections[1].label(0, "kick" );
    engine.score.sections[1].sequence(0).set( { { 0.7f, o   },
                                                { 0.0f, o } }, 16.0, 1.0 );
    engine.score.sections[1].label(1, "snare" );
    engine.score.sections[1].sequence(1).set( { { 0.7f, o },
                                                { 1.0f, o } }, 16.0, 1.0 );
    engine.score.sections[1].label(2, "hh" );
    engine.score.sections[1].sequence(2).set( { {  0.5f, o },
                                                {  2.0f, o } }, 16.0, 1.0 );
    engine.score.sections[1].label(3, "hh soft" );
    engine.score.sections[1].sequence(3).set( { {  1.0f, o },
                                                {  2.0f, o } }, 16.0, 1.0 );
    engine.score.sections[1].label(4, "shuffle" );
    engine.score.sections[1].sequence(4).set( { { 0.7f, 0.7f },
                                                { 2.0f, 3.0f } }, 16.0, 1.0 );

    // reese bass loops 
    engine.score.sections[2].label(0, "main" );
    engine.score.sections[2].sequence(0).set( { { 1.0f,  o,  o,  o,  o,  o,   o,   0.f },
                                                { 29.0f, o,  o,  o,  o,  o,  31.f,  o   } }, 4.0, 2.0 );
    engine.score.sections[2].label(1, "fill 1" );
    engine.score.sections[2].sequence(1).set( { { 1.0f,  o,  o,  o, 0.0f },
                                                { 29.0f, o,  o,  o,  o    } }, 4.0, 2.0 );
    engine.score.sections[2].label(2, "fill 2" ); 
    engine.score.sections[2].sequence(2).set( { { 0.0f,  o, 1.0f,  o,  o,  o,   o,  o},
                                                { o,     o,  32.f,  o, 31.f,  o,  30.f,  o   } }, 4.0, 2.0 );
    engine.score.sections[2].label(3, "fill 3" );  
    engine.score.sections[2].sequence(3).set( { { 1.0f,  o,  0.f,   1.f,  o,   o,   0.0f,  o },
                                                { 29.0f, o,  o,  o, 34.f, o,  33.0f,   o   } }, 8.0, 2.0 );
    engine.score.sections[2].label(4, "fill 4" ); 
    engine.score.sections[2].sequence(4).set( { { 1.0f,  o,  o,  o,  o,  o,   o,   0.f },
                                                { 32.0f, o,  o,  o,  o,  o,  31.f,  o   } }, 4.0, 2.0 );
 
    // launch masterplan
    engine.score.sections[0].launchCell(0);
    
    
    // ---------------------PATCHING------------------------
    
    drums.add("data/break_kick.wav");
    drums.add("data/break_snare.wav");
    drums.add("data/break_hh.wav");
    drums.add("data/break_ghost.wav");
    3.0f >> drums.in("pitch");

    engine.score.sections[1].out_trig(0)  >> drums.in("trig");
    engine.score.sections[1].out_value(1) >> drums.in("select");
    drums * dB(9.0f) >> engine.audio_out(0);
    drums * dB(9.0f)  >> engine.audio_out(1);
    
    engine.score.sections[2].out_trig(0)  >> reese.in("trig");
    engine.score.sections[2].out_value(1) >> reese.in("pitch");
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
        if(engine.score.isPlaying()){
            engine.score.stop();
        }else{
            engine.score.play();
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
