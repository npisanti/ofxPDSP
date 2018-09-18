#include "ofApp.h"

// This example teach you the basics of using pdsp::Parameter inside ofParameter based UIs

//--------------------------------------------------------------
void ofApp::setup(){
    
    //--------PATCHING-------
    // pdsp::Parameter/s can be patched to pdsp modules
    // can be changed in a thread-safe way
    // and let you return ofParameter to control it from GUISs or serialize/deserialize
    
    
    // we patch the pdsp::Parameter to control pitch and amp
    // and then patch the oscillator to the engine outs
    osc1_pitch_ctrl >> osc1.in_pitch();
    osc2_pitch_ctrl >> osc2.in_pitch();
    osc3_pitch_ctrl >> osc3.in_pitch();


    // pdsp::ParameterGain can be added to gui like an ofParameter
    // and has an input and output for signals
    // it is used to control volume as it has control in deciBel
    // pdsp::ParameterAmp instead just multiply the input for the parameter value
    // it is usefule for scaling modulation signals 
    osc1 >> osc1_amp  >> engine.audio_out(0);
            osc1_amp  >> engine.audio_out(1);
    osc2 >> osc2_gain >> engine.audio_out(0);
            osc2_gain >> engine.audio_out(1);
    osc3 >> osc3_gain >> engine.audio_out(0);
            osc3_gain >> engine.audio_out(1);


    // GUI setup -----------------------------------
    // the API to get ofParameters is the same for pdsp::Parameter, 
    // pdsp::ParameterAmp and pdsp::ParameterGain
    
    gui.setup("3-osc", "settings.xml, 20, 20");
    osc1_group.setName("oscillator 1");
    // if you use float values in the set() method the ofParameter<float> is returned and used
    osc1_group.add( osc1_pitch_ctrl.set( "pitch", 60.0f, 24.0f, 96.0f ) ); 
    osc1_group.add( osc1_amp.set( "amp", 0.25f,  0.0f, 1.0f ) ); 
    gui.add( osc1_group );
   
    osc2_group.setName("oscillator 2");
    // if you use int values in the set() method the ofParameter<int> is returned and used
    osc2_group.add( osc2_pitch_ctrl.set( "pitch", 60, 24, 96 ) ); 
    // you can also use an ofParameter<bool> with values for false/true
    osc2_group.add( osc2_gain.set( "active", false, -48.0f, -12.0f )); // -48.0f or less mute the signal
    gui.add( osc2_group );
    
    osc3_group.setName("oscillator 3");
    // you can combine both to have both a control in int and another in float for fine-tuning
    osc3_group.add( osc3_pitch_ctrl.set( "pitch coarse",  60,   24,    96  ) ); // only int arguments
    osc3_group.add( osc3_pitch_ctrl.set( "pitch fine  ", 0.0f, -0.5f, 0.5f ) );  // only float arguments
    osc3_group.add( osc3_gain.set( "gain", -24.f,  -48.0f, 0.0f )  );  // -48.0f or less mute the signal
    gui.add( osc3_group );

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
        // sets the parameter to a random value 
        osc2_pitch_ctrl.set( ofRandom(48.0f, 96.0f) );
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
