#include "ofApp.h"

// This is the most basic pdsp example
// we set up everything and check that everything is working
// before running this also check that the basic oF audio output example is working
// ofxx_folder/examples/sound/audioOutputExample/


//--------------------------------------------------------------
void ofApp::setup(){
    
    //-------------------GRAPHIC SETUP--------------
    ofBackground(0);
    ofSetFrameRate(30);
    
    //-------------------------PATCHING--------------
    // the basic operation of pdsp is the patching 
    // it is made with the patch operator >>
    // each module has some inputs and outputs you access with in("tag"), in_tag(), out("tag") and out_tag()
    // pdsp::Processor is always the last unit in the chain, it is usually used as final output
    // see the audioOut callback for more
    
    // an oscillator with nothing connected to in_pitch() will play A4 = 440hz
    // the * operator when used before patching scales the output signal
    osc.out_sine() * 0.20f >> pdspEngine.channels[0]; // connect to left output channel
    osc.out_sine() * 0.20f >> pdspEngine.channels[1]; // connect to right right channel

    // decomment this for setting the oscillator pitch to another fixed value
    // 81.0f >> osc.in_pitch();

    cout<<"finished patching\n";
    
    //----------------------AUDIO SETUP-------------
    int expectedBufferSize = 512;
    double sampleRate = 44100.0;   
    
    // this set all the modules buffers at the right dimension
    // never forget this call, or you will get a segfault as soon as pdsp start processing
    ofxPDSPSetup( expectedBufferSize, sampleRate ); 
  
    // standard oF calls
    ofSoundStreamListDevices();
    audioStream.setDeviceID(0);
    audioStream.setup(this, 2, 0, static_cast<int>(sampleRate), expectedBufferSize, 2);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer &outBuffer) {
    // when the audioOut callback is executed this method recursively process the connected module network
    // and then copy the result to the output buffer
    pdspEngine.processAndCopyInterleaved(outBuffer.getBuffer().data(), outBuffer.getNumChannels(), outBuffer.getNumFrames());
}

//--------------------------------------------------------------
void ofApp::exit() {
    // we clean everything before shutting down
	audioStream.stop();
	audioStream.close();
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
