#include "ofApp.h"

// This example teach you the basics of controlling the modules by oF
// for info about ADSR envelopes 
// https://en.wikipedia.org/wiki/Synthesizer#Attack_Decay_Sustain_Release_.28ADSR.29_envelope

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowShape(640, 360); // we don't need fancy graphics at the moment

    //--------PATCHING-------
    
    // a pdsp::ADSR is an envelope that makes a one-shot modulation when triggered
    // pdsp::ADSR require an output sending trigger signals
    // a pdsp::Amp multiply in_signal() and in_mod()      
    // remember, in pdsp out_trig() always have to be connected to in_trig()
    // in_trig() is the default pdsp::ADSR input signal
    
    gate_ctrl.out_trig() >> env.set(0.0f, 50.0f, 0.5f, 500.0f);
    // when available, the set() methods make you set the default values that are used when nothing is patched to an input
    // in this case you set the values for in_attack(), in_decay(), in_sustain(), and in_release()
                         
    env * dB(-12.0f) >> amp.in_mod();
        
    pitch_ctrl >> osc.in_pitch();
                  osc >> amp >> engine.audio_out(0);
                         amp >> engine.audio_out(1);

    pitch_ctrl.setv(72.0f); // we control the value of an ofxPDSPValue directly with the setv function
    
    // you can smooth out an ofxPDSPValue changes, decomment this for less "grainy" pitch changes
    // pitch_ctrl.enableSmoothing(50.0f); // 50ms smoothing


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
    ofBackground( env.meter_output() * 255 );
    // you can use the meter_xxx() methods to monitor some pdsp values in a thread-safe manner
    ofDrawBitmapString("click with the mouse in the window and then drag, \ndrag x = pitch, click y = envelope intensity", 50, 50);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    float pitch = ofMap(x, 0, ofGetWidth(), 36.0f, 72.0f);
    pitch_ctrl.setv(pitch);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    float pitch = ofMap(x, 0, ofGetWidth(), 36.0f, 72.0f);
    pitch_ctrl.setv(pitch);
    
    // y value controls the trigger intensity
    float trig = ofMap(y, 0, ofGetHeight(), 1.0f, 0.000001f); 
    gate_ctrl.trigger(trig); // we send a trigger to the envelope
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    gate_ctrl.off(); // we send an "off" trigger to the envelope
    // this is the same as writing
    // gate_ctrl.trigger( 0.0f ); 
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
