#include "ofApp.h"

// before looking at this check out the basics examples

// midi controlled wavetable synth example
// you need a midi keyboard for trying this! 
// remember also to select the right midi port and audio output device, as ususal.

// the code here in ofApp.cpp is similar to the midi_polysynth example
// most of the work is done in synth.h and synth.cpp

//--------------------------------------------------------------
void ofApp::setup(){
    
    //patching-------------------------------

    // set up control
    // you can use setPolyMode(int maxNotes, int unisonVoices) or setMonoMode(int unisonVoices, bool legato, MonoPriority priority)
    midiKeys.setPolyMode(8, 1);
    
    // activate portamento, in poly mode you can notice portamento only on note stealing
    //midiKeys.setPortamento(On, 250.0f, pdsp::Rate);

    // voices is a vector of synth voices, we resize it to the midiKeys voice number     
    voicesNum = midiKeys.getVoicesNumber();
    
    voices.resize(voicesNum);
    
    // midiKeys has a vector for the pitch outputs and one for the trigger outputs
    // we patch them to the voices here
    for(int i=0; i<voicesNum; ++i){
        // setup voice
        voices[i].setup( synthGlobal );

        // connect each voice to a midi pitch and trigger output
        midiKeys.outs_trig[i]  >> voices[i].in("trig");
        midiKeys.outs_pitch[i] >> voices[i].in("pitch");

    }
    
    // set up chorus
    synthGlobal.out_L() >> engine.audio_out(0);
    synthGlobal.out_R() >> engine.audio_out(1);

  
    // graphic setup---------------------------
    ofSetVerticalSync(true);
    ofDisableAntiAliasing();
    ofBackground(0);
    ofSetColor(ofColor(0,100,100));
    ofNoFill();
    ofSetLineWidth(1.0f);
    ofSetFrameRate(24);
    ofSetWindowShape(230,465);

    // GUI -----------------------------------
    ofColor mainColor = ofColor(255, 95, 95);
    gui.setHeaderBackgroundColor(mainColor);
    gui.setBorderColor(ofColor(40,40,40));
    gui.setDefaultFillColor(mainColor.getLerped(ofColor::black, 0.4f));
    gui.setDefaultBackgroundColor(ofColor(0,0,0));
    
    gui.setup("wavetable synth");
    synthGlobal.addToGUI( gui );
    gui.setPosition(15, 20);
    
    // audio / midi setup----------------------------
    
    //get MIDI control
    midiIn.listPorts();
    midiIn.openPort(1); //set the right port !!!
    // for our midi controllers to work we have to add them to the engine, so it know it has to process them
    engine.addMidiController( midiKeys, midiIn ); // add midi processing to the engine
    engine.listDevices();
    engine.setDeviceID(0); // REMEMBER TO SET THIS AT THE RIGHT INDEX!!!!
    engine.setup( 44100, 512, 3);     
    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    // draw GUI
    gui.draw();
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
