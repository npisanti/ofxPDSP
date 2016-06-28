#include "ofApp.h"

// This example teach you the basics of using ofxPDSPValue inside ofParameter based UIs

//--------------------------------------------------------------
void ofApp::setup(){
    
    //--------PATCHING-------

    // we patch the ofxPDSPValues to control pitch and amp
    // and then patch the oscillator to the engine outs
    osc1_pitch_ctrl >> osc1.in_pitch();
                       osc1 >> amp1          >> engine.audio_out(0);
              osc1_amp_ctrl >> amp1.in_mod() >> engine.audio_out(1);
    osc2_pitch_ctrl >> osc2.in_pitch();
                       osc2 >> amp2          >> engine.audio_out(0);
              osc2_amp_ctrl >> amp2.in_mod() >> engine.audio_out(1);
    osc3_pitch_ctrl >> osc3.in_pitch();
                       osc3 >> amp3          >> engine.audio_out(0);
              osc3_amp_ctrl >> amp3.in_mod() >> engine.audio_out(1);


    // GUI setup -----------------------------------
    gui.setup("3-osc");
    
    // you can use ofxPDSPValue as they were ofParameters, but with some difference
    // inside ofxPDSPValues there is an ofParameter<float> and an ofParameter<int> both set to zero
    // when they change the the ofxPDSPValue is set as the sum of both
    
    osc1_group.setName("oscillator 1");
    // if you use float values in the set() method the ofParameter<float> is returned and used
    osc1_group.add( osc1_pitch_ctrl.set( "pitch", 60.0f, 24.0f, 96.0f ) ); 
    osc1_group.add( osc1_amp_ctrl.set(   "amp",  0.05f,  0.0f, 0.33f ) );
    gui.add( osc1_group );
   
    osc2_group.setName("oscillator 2");
    // if you use int values in the set() method the ofParameter<int> is returned and used
    osc2_group.add( osc2_pitch_ctrl.set( "pitch", 60, 24, 96 ) ); 
    osc2_group.add( osc2_amp_ctrl.set(   "amp",  0.05f,  0.0f, 0.33f ) );
    gui.add( osc2_group );
    
    osc3_group.setName("oscillator 3");
    // you can combine both to have both a control in int and another in float for fine-tuning
    osc3_group.add( osc3_pitch_ctrl.set( "pitch coarse",  60,   24,    96  ) ); 
    osc3_group.add( osc3_pitch_ctrl.set( "pitch fine  ", 0.0f, -0.5f, 0.5f ) ); 
    osc3_group.add( osc3_amp_ctrl.set(   "amp",  0.05f,  0.0f, 0.33f ) );
    gui.add( osc3_group );
    
    gui.setPosition(15, 20);
    ofSetWindowShape(230,350);
    ofBackground(0); 
    
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
    gui.draw();
    ofDrawBitmapString( "press 't' to set osc2\nto a random number\n(ui won't change)", 20, ofGetHeight() - 37);
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
    if(key=='t'){
        // when you use setv the ofParameter value is not updated
        // this is for sparing cpu time when you are not using ofxPDSPValue for GUIs
        // also: even if we are using an int UI for osc2 
        // the setv method can set it to float values
        osc2_pitch_ctrl.setv( ofRandom(48.0f, 96.0f) );
    }
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
