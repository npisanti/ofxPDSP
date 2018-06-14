#include "ofApp.h"

// this is a basic ios example,
// it is just a little more than a "hello world" program to test that everything is working

// before looking at this check out the basics examples

//--------------------------------------------------------------
void ofApp::setup(){
    //-------------------GRAPHIC SETUP--------------
    ofBackground(0);
    ofSetFrameRate(30);
    
    //-------------------------PATCHING--------------
    // an oscillator with nothing connected to in_pitch() will play A4 = 440hz
    // the * operator when used before patching scales the output signal
    osc.out_sine() >> amp * 0.20f >> engine.audio_out(0); // connect to left output channel
                      amp * 0.20f >> engine.audio_out(1); // connect to right right channel
   
    0.0f >> amp.in_mod();
    ofLogNotice()<<"finished patching\n";
    
    //for some reason on the iphone simulator 256 doesn't work - it comes in as 512!
    //so we do 512 - otherwise we crash
    // also some phones force the engine to 48000hz, so it's better to use 48000 as sample rate
    // the num buffers is ininfluent on ios, you can use any number
    engine.setup( 48000, 512, 4 );
    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofDrawBitmapString("touch the screen for testing!", 20, ofGetScreenHeight()*0.5f);
}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch){
    if(touch.id==0){
        1.0f >> amp.in_mod();
    }
}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs & touch){
    if(touch.id==0){
        0.0f >> amp.in_mod();
    }
}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchCancelled(ofTouchEventArgs & touch){
    
}

//--------------------------------------------------------------
void ofApp::lostFocus(){

}

//--------------------------------------------------------------
void ofApp::gotFocus(){

}

//--------------------------------------------------------------
void ofApp::gotMemoryWarning(){

}

//--------------------------------------------------------------
void ofApp::deviceOrientationChanged(int newOrientation){

}
