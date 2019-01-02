#include "ofApp.h"

// use this example together with example-osc-input-sender

#define PORT 12345

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetWindowShape(640, 360); // we don't need fancy graphics at the moment
    ofSetWindowTitle("example-osc-input");
    
    //--------PATCHING-------
    oscInput.setVerbose( true );
    oscInput.openPort( PORT );
    
    // string address / argument index 
    oscInput.out_trig( "/gate", 0 )>> env.set(0.0f, 50.0f, 0.5f, 500.0f);
                         
    env * dB(-12.0f) >> amp2.in_mod();

    // argument is 0 if not given    
    oscInput.out_value( "/amp" ) >> amp1.in_mod();
    //1.0f >> amp1.in_mod();
        
    oscInput.out_value( "/pitch" ) >> osc.in_pitch();
                                      osc >> amp1 >> amp2 >> engine.audio_out(0);
                                                     amp2 >> engine.audio_out(1);

    oscInput.out_value( "/amp", 0 ).enableSmoothing( 50.0f );
    oscInput.out_value( "/pitch", 0 ).enableSmoothing( 50.0f );
    
    //------------SETUPS AND START AUDIO-------------
    engine.addOscInput ( oscInput );
    engine.listDevices();
    engine.setDeviceID(0); // REMEMBER TO SET THIS AT THE RIGHT INDEX!!!!
    engine.setup( 44100, 512, 3); 
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground( env.meter_output() * amp1.meter_mod() * 255 );
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
