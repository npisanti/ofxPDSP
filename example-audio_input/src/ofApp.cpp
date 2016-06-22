#include "ofApp.h"

// before looking at this, be shure to check out the basics with the ofxPDSP wiki tutorials:
// https://github.com/npisanti/ofxPDSP/wiki

// audio input example, and UI example
// at the moment on some systems the input comes in distorted, 
// this may be a problem of audio input underruns on some systems
// as on OSX and on Linux (with JACK) i've had no problems

//--------------------------------------------------------------
void ofApp::setup(){
    
    //-------------------GRAPHIC SETUP--------------
    ofSetWindowShape(230,300);
    ofDisableAntiAliasing();
    ofBackground(0);
    ofSetFrameRate(30);
    
    //-------------------------PATCHING--------------
    engine.setChannels(2, 2); // activate 2 input channels
    
    //signal
    // we patch our audio input
    engine.audio_in(0) >> compressor >> channelGain         >> engine.audio_out(0);
                                        channelGain         >> engine.audio_out(1);
                          compressor >> delaySend >> delayL >> engine.audio_out(0);
                                        delaySend >> delayR >> engine.audio_out(1);
   
    d1_time_ctrl >> delayL.in_time();
    d2_time_ctrl >> delayL.in_time();
   
    d_fb_ctrl >> fb_node >> delayL.in_feedback();
                 fb_node >> delayR.in_feedback();
    d_damp_ctrl >> damp_node >> delayL.in_damping();
                   damp_node >> delayR.in_damping();

    gain_ctrl >> channelFader >> channelGain.in_mod();
    send_ctrl >> sendFader >> delaySend.in_mod();

    comp_threshold_ctrl >> compressor.in_threshold();
    comp_attack_ctrl    >> compressor.in_attack();
    comp_release_ctrl   >> compressor.in_release();


    cout<<"finished patching\n";
    
    //----------------------AUDIO SETUP-------------
    
    // start your audio engines!
    engine.listDevices();
    
    engine.setDeviceID(0); // REMEMBER TO SET THIS AT THE RIGHT INDEX!!!!
    // if listDevices() lists different devices for in and out you need to make something like this 
    //engine.setOutputDeviceID(0);
	//engine.setInputDeviceID(1);    
    engine.setup( 44100, 512, 3); 
    
    // GUI -----------------------------------   

    // you can use set() to set ofxPDSPValue like ofParameter    

    gui.setup("channel strip");

    comp_gui.setName("compressor parameters");
    comp_gui.add( comp_threshold_ctrl.set( "threshold", -20.0f, -50.0f, 0.0f ) );
    comp_gui.add( comp_meter.set("GR meter", 0.0f, 0.0f, -25.0f) );
    comp_gui.add( comp_attack_ctrl.set( "attack (ms)", 10.0f, 1.0f, 60.0f ) );
    comp_gui.add( comp_release_ctrl.set( "release (ms)", 50.0f, 10.0f, 250.0f ) );
    gui.add(comp_gui);
 
    delay_gui.setName("delays parameters");
    delay_gui.add( send_ctrl.set( "send", -30.0f, -60.0f, 0.0f ) );
    delay_gui.add( d1_time_ctrl.set( "time L (ms)", 200.0f, 5.0f, 800.0f ) );
    delay_gui.add( d2_time_ctrl.set( "time R (ms)", 300.0f, 5.0f, 800.0f ) );
    delay_gui.add( d_fb_ctrl.set( "feedback", 0.5f, 0.0f, 1.0f ) );
    delay_gui.add( d_damp_ctrl.set( "damping", 0.5f, 0.0f, 1.0f ) );
    gui.add(delay_gui);
    
    gui.add(gain_ctrl.set( "channel volume", 0.0f, -30.0f, 12.0f ));  

    gui.setPosition(15, 20);      
}

//--------------------------------------------------------------
void ofApp::update(){
    // we use comp_meter to report the gain reduction in dB
    comp_meter.set(compressor.meter_GR());
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
