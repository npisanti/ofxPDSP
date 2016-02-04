#include "ofApp.h"

// this is a basic ios example,
// it is just a little more than a "hello world" program to test that everything is working

//--------------------------------------------------------------
void ofApp::setup(){
    //-------------------GRAPHIC SETUP--------------
    ofBackground(0);
    ofSetFrameRate(30);
    
    //-------------------------PATCHING--------------
    // an oscillator with nothing connected to in_pitch() will play A4 = 440hz
    // the * operator when used before patching scales the output signal
    osc.out_sine() >> amp * 0.20f >> pdspEngine.channels[0]; // connect to left output channel
                      amp * 0.20f >> pdspEngine.channels[1]; // connect to right right channel
   
    0.0f >> amp.in_mod();
    cout<<"finished patching\n";
    
    sampleRate = 44100;
    //for some reason on the iphone simulator 256 doesn't work - it comes in as 512!
    //so we do 512 - otherwise we crash
    expectedBufferSize = 512;
    
    ofxPDSPSetup(expectedBufferSize, sampleRate);
	ofSoundStreamSetup(2, 0, this, sampleRate, expectedBufferSize, 4);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofDrawBitmapString("touch the screen for testing!", 20, 20);
}

//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels){
    pdspEngine.processAndCopyInterleaved(output, nChannels, bufferSize);
}

//--------------------------------------------------------------
void ofApp::exit(){
    ofSoundStreamStop();
    ofSoundStreamClose();

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
