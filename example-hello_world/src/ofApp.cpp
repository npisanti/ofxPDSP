#include "ofApp.h"

// This is the most basic pdsp example
// we set up everything and check that everything is working
// before running this also check that the basic oF audio output example is working
// ofxx_folder/examples/sound/audioOutputExample/

// also be shure to check out the basics with the ofxPDSP wiki tutorials:
// https://github.com/npisanti/ofxPDSP/wiki

//--------------------------------------------------------------
void ofApp::setup(){
    
    //-------------------GRAPHIC SETUP--------------
    ofBackground(0);
    ofSetFrameRate(30);
    
    //-------------------------PATCHING--------------
    // the basic operation of pdsp is the patching 
    // it is made with the patch operator >>
    // each module has some inputs and outputs you access with in("tag"), in_tag(), out("tag") and out_tag()
    // ofxPDSPEngine is always the last unit in the chain, it also manages the ofSoundStreams
    // and let you patch your modules to the system audio output
    
    // an oscillator with nothing connected to in_pitch() will play A4 = 440hz
    // the * operator when used before patching scales the output signal
    osc.out_sine() * 0.25f >> engine.audio_out(0); // connect to left output channel
    osc.out_sine() * 0.25f >> engine.audio_out(1); // connect to right right channel

    // decomment this for setting the oscillator pitch to another fixed value
    // 81.0f >> osc.in_pitch();

    cout<<"finished patching\n";
    
    //----------------------AUDIO SETUP-------------

    // start your audio engines!
    engine.listDevices();
    engine.setDeviceID(0); // REMEMBER TO SET THIS AT THE RIGHT INDEX!!!!

    engine.setup( 44100, 512, 3); 
    // arguments are : sample rate, buffer size, and how many buffer there are in the audio callback queue
    // 512 is the minimum buffer size for the raspberry Pi to work
    // buffer size is better to be a power of 2, typical buffer sizes are: 128, 256, 512, 1024
    // you need at least 2 buffer queue
    // 3 buffers queue is the minimum for the rPi to work
    // if you are using JACK you have to set this number to the bufferSize you set in JACK
    // on Windows you have to set the sample rate to the system sample rate, usually 44100hz

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
