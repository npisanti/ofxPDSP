#include "ofApp.h"

// this is a basic android example,
// it is just a little more than a "hello world" program to test that everything is working

// before looking at this check out the basics examples

// at the moment android phones have a really high latency,
// and are really hard to use for realtime audio interaction
// also android studio runs in debug variant as default, making the code slower
// anyway i hope things will improve in the future

//--------------------------------------------------------------
void ofApp::setup(){

    ofBackground(0);

    //-------------------------PATCHING--------------
    // an oscillator with nothing connected to in_pitch() will play A4 = 440hz
    // the * operator when used before patching scales the output signal
    osc.out_sine() >> amp * 0.5f >> engine.audio_out(0); // connect to left output channel
                      amp * 0.5f >> engine.audio_out(1); // connect to right right channel

    0.0f >> amp.in_mod();

    engine.setup( 44100, 512, 3);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofDrawBitmapString("touch the screen for testing!", 20, ofGetScreenHeight()*0.5f);
}

//--------------------------------------------------------------
void ofApp::exit(){
    
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::touchDown(int x, int y, int id){
    if(id==0){
        1.0f >> amp.in_mod();
    }
}


//--------------------------------------------------------------
void ofApp::touchMoved(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchUp(int x, int y, int id){
    if(id==0){
        0.0f >> amp.in_mod();
    }
}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchCancelled(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::swipe(ofxAndroidSwipeDir swipeDir, int id){

}

//--------------------------------------------------------------
void ofApp::pause(){

}

//--------------------------------------------------------------
void ofApp::stop(){

}

//--------------------------------------------------------------
void ofApp::resume(){

}

//--------------------------------------------------------------
void ofApp::reloadTextures(){

}

//--------------------------------------------------------------
bool ofApp::backPressed(){
    return false;
}

//--------------------------------------------------------------
void ofApp::okPressed(){

}

//--------------------------------------------------------------
void ofApp::cancelPressed(){

}

