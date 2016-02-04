#include "ofApp.h"

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
    //signal
    // input1 is an instance of pdsp::ExternalInput, a bridge between the audio input callback and pdsp
    input1 >> compressor >> channelGain >> pdspEngine.channels[0];
                            channelGain >> pdspEngine.channels[1];
              compressor >> delaySend >> delayL >> pdspEngine.channels[0];
                            delaySend >> delayR >> pdspEngine.channels[1];
   
    // you can use set() to set the parameters like ofParameter
    // an ofxPDSPParameter set() returns a Patchable reference so you can patch it with >>
    d1_time_ctrl.set( "time L (ms)", 200.0f, 5.0f, 800.0f ) >> delayL.in_time();
    d2_time_ctrl.set( "time R (ms)", 300.0f, 5.0f, 800.0f ) >> delayL.in_time();
   
    d_fb_ctrl.set( "feedback", 0.5f, 0.0f, 1.0f ) >> fb_node >> delayL.in_feedback();
                                                           fb_node >> delayR.in_feedback();
    d_damp_ctrl.set( "damping", 0.5f, 0.0f, 1.0f ) >> damp_node >> delayL.in_damping();
                                                            damp_node >> delayR.in_damping();

    gain_ctrl.set( "channel volume", 0.0f, -30.0f, 12.0f ) >> channelFader >> channelGain.in_mod();
    send_ctrl.set( "send", -30.0f, -60.0f, 0.0f ) >> sendFader >> delaySend.in_mod();

    comp_threshold_ctrl.set( "threshold", -20.0f, -50.0f, 0.0f ) >> compressor.in_threshold();
    comp_attack_ctrl.set( "attack (ms)", 10.0f, 1.0f, 60.0f ) >> compressor.in_attack();
    comp_release_ctrl.set( "release (ms)", 50.0f, 10.0f, 250.0f ) >> compressor.in_release();
    comp_meter.set("GR meter", 0.0f, 0.0f, -25.0f);
   

    cout<<"finished patching\n";
    
    //----------------------AUDIO SETUP-------------
    int expectedBufferSize = 512;
    double sampleRate = 44100.0;   

    ofxPDSPSetup(expectedBufferSize, sampleRate);
  
    ofSoundStreamListDevices();
    
    audioStream.setDeviceID(0);
    audioStream.setup(this, 2, 2, static_cast<int>(sampleRate), expectedBufferSize, 2);
    
    // if ofSoundStreamListDevices() lists different devices for in and out you need to make something like this 
    //audioStream.setDeviceID(0);
    //audioStream.setup(this, 2, 0, static_cast<int>(sampleRate), expectedBufferSize, 2);
	//inputStream.setDeviceID(1);
	//inputStream.setup(this, 0, 2, static_cast<int>(sampleRate), expectedBufferSize, 2);
    
    // GUI -----------------------------------   
    gui.setup("channel strip");
    
    comp_gui.setName("compressor parameters");
    comp_gui.add(comp_threshold_ctrl.parameter);
    comp_gui.add(comp_meter.parameter);
    comp_gui.add(comp_attack_ctrl.parameter);
    comp_gui.add(comp_release_ctrl.parameter);
    gui.add(comp_gui);
 
    delay_gui.setName("delays parameters");
    delay_gui.add(send_ctrl.parameter);
    delay_gui.add(d1_time_ctrl.parameter);
    delay_gui.add(d2_time_ctrl.parameter);
    delay_gui.add(d_fb_ctrl.parameter);
    delay_gui.add(d_damp_ctrl.parameter);
    gui.add(delay_gui);
    
    gui.add(gain_ctrl.parameter);  

    gui.setPosition(15, 20);      
}

//--------------------------------------------------------------
void ofApp::update(){
    // we use comp_meter ofPDSPParameter to report the gain reduction in dB
    comp_meter.set(compressor.meter_GR());
}

//--------------------------------------------------------------
void ofApp::draw(){
    // draw GUI
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    // this is mandatory for interfacing the audio input with pdsp
    input1.copyInterleavedInput(input, 0, nChannels, bufferSize);
}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer &outBuffer) {
    pdspEngine.processAndCopyInterleaved(outBuffer.getBuffer().data(), outBuffer.getNumChannels(), outBuffer.getNumFrames());
}

//--------------------------------------------------------------
void ofApp::exit() {
	audioStream.stop();
	audioStream.close();
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
