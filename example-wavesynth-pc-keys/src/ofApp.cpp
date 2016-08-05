
#include "ofApp.h"

// before looking at this check out the basics examples

// wavetable synth controlled with your computer keyboard
// remember also to select the right audio output device, as ususal.

// the code here in ofApp.cpp is similar to the midi_polysynth example, but with ofxPDSPComputerKeyboard
// most of the work is done in synth.h and synth.cpp


//--------------------------------------------------------------
void ofApp::setup(){
    
    //patching-------------------------------
    
    // set up control
    // you can use setPolyMode(int maxNotes) or setMonoMode(bool legato, MonoPriority priority)
    keyboard.setPolyMode( 8 );
    
    /*
    // keyboard is set up froa a qwerty keyboard, but you can define your own key accessing to the keys vector (it's public)
    keyboard.keys[0]  = 'a';
    keyboard.keys[1]  = 'w';
    keyboard.keys[2]  = 's';
    keyboard.keys[3]  = 'e';
    keyboard.keys[4]  = 'd';
    keyboard.keys[5]  = 'f';
    keyboard.keys[6]  = 't';
    keyboard.keys[7]  = 'g';
    keyboard.keys[8]  = 'y';
    keyboard.keys[9]  = 'h';
    keyboard.keys[10] = 'u';
    keyboard.keys[11] = 'j';
    keyboard.keys[12] = 'k';
    // remember that 12345567890 are already assigned to dynamic and octave control
    */
    
    // activate portamento, in poly mode you can notice portamento only on note stealing
    //keyboard.setPortamento(On, 250.0f);

    // voices is a vector of synth voices, we resize it to the midiKeys voice number     
    voicesNum = keyboard.getVoicesNumber();
    
    voices.resize(voicesNum);
    
    // keyboard has a vector for the pitch outputs and one for the trigger outputs
    // we patch them to the voices here
    for(int i=0; i<voicesNum; ++i){
        // setup voice
        voices[i].setup( synthGlobal );

        // connect each voice to a pitch and trigger output
        keyboard.outs_trig[i]  >> voices[i].in("trig");
        keyboard.outs_pitch[i] >> voices[i].in("pitch");

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
    //ofSetWindowShape(230,465);
    ofSetWindowShape(230,665);

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
    ofSetColor( 255, 95, 95 );
    
    // draw some keyboard keys and infos
    keyboard.draw(14, 465, 200, 180);
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    // sends key messages to ofxPDSPComputerKeyboard
    keyboard.keyPressed( key ); 
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    // sends key messages to ofxPDSPComputerKeyboard
    keyboard.keyReleased( key );
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
