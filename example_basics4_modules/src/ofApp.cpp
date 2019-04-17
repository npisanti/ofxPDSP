#include "ofApp.h"

// This example teach you to create your own synth / modules
// you can also check the src/modules folder in the ofxPDSP source code for other examples
// check the custom synth in the synth.h file

//--------------------------------------------------------------
void ofApp::setup(){

    //--------PATCHING-------

    pitch_ctrl >> synth.in("pitch"); // patching with in("tag")
    amp_ctrl   >> synth.in("amp"); // patching with custom in_tag()
                  synth * dB(-12.0f) >> engine.audio_out(0);
                  synth * dB(-12.0f) >> engine.audio_out(1);


    pitch_ctrl.set(60.0f);
    pitch_ctrl.enableSmoothing(50.0f); // 50ms smoothing
    amp_ctrl.set(0.0f);
    amp_ctrl.enableSmoothing(50.0f); // 50ms smoothing


    //------------SETUPS AND START AUDIO-------------
    engine.listDevices();
    engine.setDeviceID(0); // REMEMBER TO SET THIS AT THE RIGHT INDEX!!!!
    engine.setup( 44100, 512, 3); 
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    ofSetColor(255);
    ofDrawBitmapString("hover the mouse in the window, \nx = pitch, y = amp/filter", 50, 50);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    float pitch = ofMap(x, 0, ofGetWidth(), 36.0f, 72.0f);
    pitch_ctrl.set(pitch);
    float amp = ofMap(y, 0, ofGetHeight(), 1.0f, 0.0f);
    amp_ctrl.set(amp);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    float pitch = ofMap(x, 0, ofGetWidth(), 36.0f, 72.0f);
    pitch_ctrl.set(pitch);
    float amp = ofMap(y, 0, ofGetHeight(), 1.0f, 0.0f);
    amp_ctrl.set(amp);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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
