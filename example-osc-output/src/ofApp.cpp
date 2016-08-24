#include "ofApp.h"


// you can test this using the oF standard ofxReceiveExample
// this is the example-serial-output modified to test OSC

#define HOST "localhost"
#define PORT 12345

//--------------------------------------------------------------
void ofApp::setup(){
    
    //-------------------GRAPHIC SETUP--------------
    ofBackground(0);
    ofSetFrameRate(30);
    
    // PATCHING
    oscOut.setVerbose(true); // when compiling in debug this also activates messages to the console when sending OSC data
    oscOut.openPort( HOST, PORT ); // set the right OSC port
    
    // setup as usual, check example-scoring for more info
    engine.score.setTempo(145.0);
    engine.score.sections.resize(1); // we need just one section
    engine.score.sections[0].setCell(0, &sequence, pdsp::Behavior::Self);
    engine.score.sections[0].launchCell(0);
    engine.score.play();  
    
    // sequence

    float o = -1.0f; // using 'o' as graphical aid for empty steps

    vector<float> channel1 
        = { 1.0f,  o  ,   o  ,  0.8f ,  o,    o  ,   o  ,    o,     o,    o,   1.0f,  o,     o  ,    o,     o ,    o  };
    vector<float> channel2 
        = { 1.0f,  o  , 0.75f,   o  ,  0.75f,  o  , 1.0f,   o  ,  0.75f,  o  , 0.75f,   o  , 1.0f,   o  ,  0.75f,  o  };
    // those are the value we pass to the OSC
    // all values are converted to int and scaled in the 0<-->127 range


    float duration = 0.25; // duration before a 0.0f value is sent, in steps, so this is 25% of a 1/16th
                           // set it to the right value for your tempo for having a consistent control 
                           // it useful to have a short duration to control solenoids on/off pwm
                           // so you don't have to use delay() in the arduino loop
                           // that will cause bad timing
  
    sequence.begin(16.0, 1.0);    
    sequence.trigVector( channel1, duration, 0); // this will add all the values of an std::vector<float> as messages
    sequence.trigVector( channel2, duration, 1); // and also generate trigger off signals (0.0f values)
    sequence.end();
    
    // you can send the output of the section to differente addresses
    // you can also use outputs that are already patched to the DSP system with out_trig() or out_value()
    engine.score.sections[0].out_message( 0 ) >> oscOut.address( "/message0" );
    engine.score.sections[0].out_message( 1 ) >> oscOut.address( "/message1" ); 
       
    
    //----------------------AUDIO SETUP-------------
    engine.addExternalOut( oscOut ); // remember to add your OSC output to the engine
    engine.listDevices();
    engine.setDeviceID(0); // REMEMBER TO SET THIS AT THE RIGHT INDEX!!!!
    engine.setup( 44100, 512, 3); 

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key){
        case '1':
            engine.score.sections[0].launchCell(0, false);
            break;
        case '2':
            engine.score.sections[0].launchCell(1, false);        
            break;
        case '3':
            engine.score.sections[0].launchCell(2, false);
            break;
    }
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
