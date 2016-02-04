#include "ofApp.h"

// midi out example, everything is managed in the struct defined into music.h

//--------------------------------------------------------------
void ofApp::setup(){
    
    //-------------------GRAPHIC SETUP--------------
    ofBackground(0);
    ofSetFrameRate(30);
    
    //----------------------AUDIO SETUP-------------
    expectedBufferSize = 512;
    sampleRate = 44100.0;   //on windows you have to set this to system sample rate

    ofxPDSPSetup(expectedBufferSize, sampleRate);
  
    ofSoundStreamListDevices();
    audioStream.setDeviceID(0);
    audioStream.setup(this, 2, 0, static_cast<int>(sampleRate), expectedBufferSize, 2);
    
    music.scoregen.play();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer &outBuffer) {
    
    music.process(outBuffer.getNumFrames());

}

//--------------------------------------------------------------
void ofApp::exit() {
    music.release();
    
	audioStream.stop();
	audioStream.close();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key){
        case '1':
            music.scoregen.sections[0].launchPattern(0, false);
            break;
        case '2':
            music.scoregen.sections[0].launchPattern(1, false);        
            break;
        case '3':
            music.scoregen.sections[0].launchPattern(2, false);
            break;
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
