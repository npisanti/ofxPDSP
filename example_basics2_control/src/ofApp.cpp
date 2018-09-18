#include "ofApp.h"

// This example teach you the basics of controlling the modules by oF
// for info about ADSR envelopes 
// https://en.wikipedia.org/wiki/Synthesizer#Attack_Decay_Sustain_Release_.28ADSR.29_envelope

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowShape(640, 360); // we don't need fancy graphics at the moment

    //--------PATCHING-------
    
    // a pdsp::ADSR is an ADSR envelope that makes a one-shot modulation when triggered
    // pdsp::ADSR require an output sending trigger signals
    // remember, in pdsp out_trig() always have to be connected to in_trig()
    // in_trig() is the default pdsp::ADSR input signal

    // a pdsp::Amp multiply in_signal() and in_mod()      
    
    gate_ctrl.out_trig() >> env;
                            env >> amp.in_mod();
        
    pitch_ctrl >> osc.in_pitch();
                  osc >> amp * dB(-12.0f) >> engine.audio_out(0);
                         amp * dB(-12.0f) >> engine.audio_out(1);

    0.0f    >> env.in_attack();
    50.0f   >> env.in_decay();
    0.5f    >> env.in_sustain();
    500.0f  >> env.in_release();

    pitch_ctrl.set(72.0f); // we control the value of an pdsp::ValueControl directly with the set function
    
    // you can smooth out an pdsp::ValueControl changes, decomment this for less "grainy" pitch changes
    pitch_ctrl.enableSmoothing(50.0f); // 50ms smoothing


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
    pitch_ctrl.set(pitch);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    float pitch = ofMap(x, 0, ofGetWidth(), 36.0f, 72.0f);
    pitch_ctrl.set(pitch);
    
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
