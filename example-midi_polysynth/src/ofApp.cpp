#include "ofApp.h"

// before looking at this, be shure to check out the basics with the ofxPDSP wiki tutorials:
// https://github.com/npisanti/ofxPDSP/wiki

// midi controlled polysynth example
// you need a midi keyboard for trying this! 
// remember also to select the right midi port and audio output device, as ususal.

//--------------------------------------------------------------
void ofApp::setup(){
    
    //patching-------------------------------
    //get MIDI control
    midiInProcessor.listPorts();
    midiInProcessor.openPort(1); //set the right port !!!

    // set up control
    // you can use setPolyMode(int maxNotes, int unisonVoices) or setMonoMode(int unisonVoices, bool legato, MonoPriority priority)
    midiKeys.setPolyMode(4, 1);
    // activate portamento, in poly mode you can notice portamento only on note stealing
    midiKeys.setPortamento(On, 250.0f, pdsp::Rate);

    // voices is a vector of synth voices, we resize it to the midiKeys voice number     
    voicesNum = midiKeys.getVoicesNumber();
    
    voices.resize(voicesNum);
    
    // midiKeys has a vector for the pitch outputs and one for the trigger outputs
    // we patch them to the voices here
    for(int i=0; i<voicesNum; ++i){
        // connect each voice to a midi pitch and trigger output
        midiKeys.outs_pitch[i] >> voices[i].voicePitch;
        midiKeys.outs_trig[i]  >> voices[i].voiceTrigger;
        // connect each voice to chorus
        voices[i].voiceAmp  >> chorus.in_0();
        voices[i].voiceAmp  >> chorus.in_1();
    }
    
    // set up chorus
    chorus.out_0() >> engine.audio_out(0);
    chorus.out_1() >> engine.audio_out(1);

    chorusSpeed.set("speed (hz)", 0.25f, 0.25f, 0.5f) >> chorus.in_speed();
    chorusDepth.set("depth (ms)", 10.f, 0.05f, 30.0f) >> chorus.in_depth();

  
    // graphic setup---------------------------
    ofSetVerticalSync(true);
    ofDisableAntiAliasing();
    ofBackground(0);
    ofSetColor(ofColor(0,100,100));
    ofNoFill();
    ofSetLineWidth(1.0f);
    ofSetFrameRate(24);

    // GUI -----------------------------------
    gui.setHeaderBackgroundColor(ofColor(0,40,40));
    gui.setBorderColor(ofColor(0,40,40));
    
    gui.setDefaultFillColor(ofColor(0,90,90));
    gui.setDefaultBackgroundColor(ofColor(0,0,0));
    
    synthUI.setName("synth parameters");
    synthUI.add(voices[0].pwUI.parameter);
    synthUI.add(voices[0].pwmUI.parameter);
    synthUI.add(voices[0].pwmSpeedUI.parameter);
    synthUI.add(voices[0].cutoffUI.parameter);
    synthUI.add(voices[0].resoUI.parameter);
    synthUI.add(voices[0].modAttackUI.parameter);
    synthUI.add(voices[0].modReleaseUI.parameter);

    chorusUI.setName("chorus parameters");
    chorusUI.add(chorusSpeed.parameter);
    chorusUI.add(chorusDepth.parameter);

    gui.setup("panel");
    gui.add(synthUI);
    gui.add(chorusUI);
    gui.setPosition(400, 20);
    
    
    // audio setup----------------------------
    engine.addMidiController( midiKeys, midiInProcessor ); // add midi processing to the engine
    engine.listDevices();
    engine.setDeviceID(0); // REMEMBER TO SET THIS AT THE RIGHT INDEX!!!!
    engine.setup( 44100, 512, 3);     
    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // draw pitch meters
    int xBase = 20;
    ofDrawBitmapString("osc pitches", xBase, 26);
    for(int i=0; i<voicesNum; ++i){
        drawMeter( voices[i].oscillator.meter_pitch(), 36.f, 84.f, xBase, 30, 20, 200);
        xBase+=40;
    }
    // draw mod envelope meters
    xBase += 30;
    ofDrawBitmapString("mod envs", xBase, 26);
    for(int i=0; i<voicesNum; ++i){
        drawMeter( voices[i].modEnv.meter_output(), 0.05f, 1.0f, xBase, 30, 20, 200);
        xBase+=40;
    }

    // draw GUI
    gui.draw();
}


//--------------------------------------------------------------
void ofApp::drawMeter(float value, float min, float max, int x, int y, int w, int h){
    
    float range = max - min;
    int levelY = ( (1.0f - ((value - min) / range) ) * static_cast<float>(h) ) + y;
    
    ofDrawRectangle(x, y, w, h);
    if(value > min && value<max ) ofDrawLine(x, levelY, x+w, levelY);
        
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
