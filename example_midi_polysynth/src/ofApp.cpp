#include "ofApp.h"

// before looking at this check out the basics examples

// midi controlled polysynth example
// you need a midi keyboard for trying this! 
// remember also to select the right midi port and audio output device, as ususal.

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetWindowTitle( "midi polysynth example" );
    
    //patching-------------------------------

    // set up control
    // you can use setPolyMode(int maxNotes, int unisonVoices) or setMonoMode(int unisonVoices, bool legato, MonoPriority priority)
    midiKeys.setPolyMode(4, 1);
    // activate portamento, in poly mode you can notice portamento only on note stealing
    midiKeys.setPortamento( pdsp::On, 250.0f, pdsp::Rate );

    //midiCCs.setCCSlew( 100.0f ); // enable for changing the midi CCs slew time, default is 50.0f = 50ms

    // voices is a vector of synth voices, we resize it to the midiKeys voice number     
    voicesNum = midiKeys.getVoicesNumber();
    
    synth.setup( voicesNum );
    
    // midiKeys has a vector for the pitch outputs and one for the trigger outputs
    // we patch them to the voices here
    for(int i=0; i<voicesNum; ++i){
        // connect each voice to a midi pitch and trigger output
        midiKeys.out_trig(i)  >> synth.voices[i].in("trig");
        midiKeys.out_pitch(i) >> synth.voices[i].in("pitch");

        // controls a bit the cutoff using the modulation wheel
        // adds up to 36 semitones to the filter cutoff pitch
        midiCCs.out(1) * 36.0f >> synth.voices[i].in("cutoff");

        // connect each voice to chorus
        synth.voices[i] >> chorus.ch(0);
        synth.voices[i] >> chorus.ch(1);
    }
    
    // set up chorus
    chorus.ch(0) >> gain.ch(0) >> engine.audio_out(0);
    chorus.ch(1) >> gain.ch(1) >> engine.audio_out(1);

    chorusSpeed >> chorus.in_speed();
    chorusDepth >> chorus.in_depth();


  
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
    
    chorusUI.setName("chorus parameters");
    chorusUI.add(chorusSpeed.set("speed (hz)", 0.25f, 0.25f, 1.0f));
    chorusUI.add(chorusDepth.set("depth (ms)", 3.5f, 1.0f, 10.0f));

    gui.setup("panel");
    gui.add( gain.set("gain", 0, -48, 12) ); 
    gain.enableSmoothing(50.f);
    gui.add( synth.ui );
    gui.add( chorusUI );
    gui.setPosition(420, 20);
    
    // audio / midi setup----------------------------
    
    //get MIDI control
    midiIn.listPorts();
    midiIn.openPort(0); //set the right port !!!
    // for our midi controllers to work we have to add them to the engine, so it know it has to process them
    engine.addMidiController( midiKeys, midiIn ); // add midi processing to the engine
    engine.addMidiController( midiCCs, midiIn );  // add midi processing to the engine
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
    int xBase = 10;
    ofDrawBitmapString("osc pitches", xBase, 26);
    for(int i=0; i<voicesNum; ++i){
        drawMeter( synth.voices[i].meter_pitch(), 36.f, 84.f, xBase, 30, 20, 200);
        xBase+=40;
    }
    // draw mod envelope meters
    xBase += 20;
    ofDrawBitmapString("mod envs", xBase, 26);
    for(int i=0; i<voicesNum; ++i){
        drawMeter( synth.voices[i].meter_mod_env(), 0.05f, 1.0f, xBase, 30, 20, 200);
        xBase+=40;
    }
    
    xBase+=20;
    ofDrawBitmapString("MW", xBase, 26);
    drawMeter( midiCCs.out(1).meter_output(), 0.05f, 1.0f, xBase, 30, 20, 200);
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
