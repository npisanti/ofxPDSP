#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    //patching-------------------------------
    //get MIDI control
    midiInProcessor.listPorts();
    midiInProcessor.openPort(0); //set the right port !!!

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
    
    //set up chorus
    chorus.out_0() >> pdspEngine.channels[0];
    chorus.out_1() >> pdspEngine.channels[1];

    chorusSpeed.set("speed (hz)", 0.25f, 0.25f, 0.5f) >> chorus.in_speed();
    chorusDepth.set("depth (ms)", 10.f, 0.05f, 30.0f) >> chorus.in_depth();
    
    //audio setup----------------------------
    bufferSize = 512;
    sampleRate = 44100.0;

    ofxPDSPSetup(bufferSize, sampleRate); // INITS ALL THE UNITS, forget this and you got segfaults
    
    audioStream.setDeviceID(0);       
    audioStream.setup(this, 2, 0, static_cast<int>(sampleRate), bufferSize, 3);
  
    //graphic setup---------------------------
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
    
    //gui.setDefaultHeight(32);


    /*
    gui.add(voices[0].pwUI.parameter);
    gui.add(voices[0].pwmUI.parameter);    
    gui.add(voices[0].pwmSpeedUI.parameter);   
    gui.add(voices[0].cutoffUI.parameter);
    gui.add(voices[0].resoUI.parameter);
    gui.add(voices[0].modAttackUI.parameter);
    gui.add(voices[0].modReleaseUI.parameter);
    */
    chorusUI.setName("chorus parameters");
    chorusUI.add(chorusSpeed.parameter);
    chorusUI.add(chorusDepth.parameter);

    gui.setup("panel");
    gui.add(voices[0].synthUI);
    gui.add(chorusUI);
    gui.setPosition(220, 20);    
}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer &outBuffer) {
    // you have to call those methods before of the pdsp::Processor process method
    midiInProcessor.processMidi(outBuffer.getNumFrames());
    midiKeys.processMidi(midiInProcessor, outBuffer.getNumFrames());
    // now we can process the DSPs
    pdspEngine.processAndCopyInterleaved(outBuffer.getBuffer().data(), outBuffer.getNumChannels(), outBuffer.getNumFrames()); 
}

//--------------------------------------------------------------
void ofApp::exit() {
	audioStream.stop();
    midiInProcessor.closePort(); // remember to close the midi port 
    audioStream.close();
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
        drawMeter( voices[i].oscillator.meter_pitch(), 36.f, 84.f, xBase, 30, 20, 150); 
        xBase+=40;
    }
    // draw mod envelope meters
    xBase = 20;
    ofDrawBitmapString("mod envs", xBase, 206);
    for(int i=0; i<voicesNum; ++i){
        drawMeter( voices[i].modEnv.meter_output(), 0.05f, 1.0f, xBase, 210, 20, 150); 
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
