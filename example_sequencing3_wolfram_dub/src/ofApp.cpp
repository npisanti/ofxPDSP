#include "ofApp.h"

#define MAX_GENERATIONS 8
#define SIDE 16

// before looking at this check out the basics and scoring examples

// this is the most complex example in ofxPDSP, and it shows you an example of workflow

// instead of hardcoding your values you should write your own
// generative sequences or code to parse value from files

// ideally you should have your sequencer classes, your synth and effect classes with their ofParameterGroups 

// patch them together in your app and fine tune them in the ui

// the included impulse response come from:
// dubshot.blogspot.com/2008/10/free-download-60-classic-and-king-tubby.html
// Licensed under a creative commons attribution-noncommercial-share alike 3.0 Netherlands license

//--------------------------------------------------------------
void ofApp::setup(){
    
    //------------------- graphics --------------
    ofBackground(0);
    ofSetFrameRate(60);
    ofDisableAntiAliasing();
    brightColor = ofColor( 255, 95, 95);
    darkColor = ofColor( 90, 35, 35);
    ofSetWindowTitle( "wolfram dub d(-_-)b" );
    caHeight = SIDE*MAX_GENERATIONS;

    //----------------- ---------------------
    // Setting up sequencer
    //engine.sequencer.stop();    // decomment if you want to start the app from stopped state
    engine.sequencer.setTempo(172.0f);
        
    // ----------- PATCHING -----------
    
    // loads reverb impulse response
    reverb.loadIR(ofToDataPath( "kingtubby-fl1.wav" ));
    
    zaps.setup(NUMSYNTHS);
    scopes.resize(NUMSYNTHS + 2);
    
    for ( int i=0; i<NUMSYNTHS; ++i ) {
        wolframSeq.out_trig(i) >> zaps.voices[i]; // patch the sequence outputs to the zaps
        zaps.voices[i] >> scopes[i] >> engine.blackhole();
    }
    
    zaps.fader.ch(0) >> engine.audio_out(0);
    zaps.fader.ch(1) >> engine.audio_out(1);   
    
    // patch the zaps to the reverb input 
    float revGain = -60.0f; // -65dB, this IRs are very loud
    zaps.fader.ch(0) * dB(revGain) >> reverb.ch(0);
    zaps.fader.ch(1) * dB(revGain) >> reverb.ch(1);
    
    // patch the reverb to an high pass filter and then to the engine
    // ( deactivated on rPi as the processor is too slow for IR convolution using FFT )
#ifndef __ARM_ARCH
    reverb.ch(0) >> revCut.ch(0) >> engine.audio_out(0);
    reverb.ch(1) >> revCut.ch(1) >> engine.audio_out(1);
    100.0f >> revCut.in_freq(); // 100hz, we cut the reverb muddy low end 
#endif

    // connect the zaps to the stereo delay
    zaps.fader.ch(0) >> dub.ch(0);
    zaps.fader.ch(1) >> dub.ch(1);
                        dub.ch(0) >> engine.audio_out(0);
                        dub.ch(1) >> engine.audio_out(1);
                        dub.ch(0) * dB(12.0f) >> scopes[NUMSYNTHS]   >> engine.blackhole();
                        dub.ch(1) * dB(12.0f) >> scopes[NUMSYNTHS+1] >> engine.blackhole();

     
    // ------------ GUI ------------
    gui.setup("", "wolframdub.xml", ofGetWidth()-220, 40);
    gui.setName( "WOLFRAM DUB" );
    gui.add( wolframSeq.parameters );
    gui.add( zaps.parameters );
    gui.add( dub.parameters );
    
    //---------------------- audio setup -------------
    engine.listDevices();
    engine.setDeviceID(0); // <--- remember to set your index
    engine.setup( 44100, 512, 3); 
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    
    gui.draw();

    ofPushMatrix();
    ofTranslate( 20, 50 );
    wolframSeq.draw( SIDE, 120, brightColor, darkColor );
    ofPopMatrix();
    
    // draw the scopes
    ofPushMatrix();
        ofTranslate( 20, 370 );
        ofSetColor( brightColor );
        
        for(int y=0; y<2; ++y){
            for( int x=0; x<3; ++x){
                int i = x+y*3;
                string label;
                switch (i){
                    case 0: case 1: case 2: case 3:
                        label = "zap " + ofToString(i);
                    break;
                    
                    case 4: label = "delay L"; break;
                    case 5: label = "delay R"; break;
                }
                int xpos = x*347;
                int ypos = y*120;
                ofDrawBitmapString( label, xpos, ypos);
                scopes[i].draw( xpos, ypos + 10, 330, 80);
            }
        }
    ofPopMatrix();


    ofDrawBitmapString("spacebar: stop/play", 20, ofGetHeight() - 20); 
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == ' '){
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

