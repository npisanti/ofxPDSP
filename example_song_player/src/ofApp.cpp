
#include "ofApp.h"

// before looking at this check out the basics examples

// audio player example
// useful to load song to analyze with the included dsp 
// or maybe extending the ofxPDSPWrapper class

// load a wav file using the load button and then press the play button

//--------------------------------------------------------------
void ofApp::setup(){

    player.out("0") >> engine.audio_out(0);
    player.out("1") >> engine.audio_out(1);

    // with this audio player you have to work with stereo samples
    // otherwise change the last line to this
    //player.out("0") >> engine.audio_out(1);

    // ------------SETUPS AND START AUDIO-------------
    engine.listDevices();
    engine.setDeviceID(0); // REMEMBER TO SET THIS AT THE RIGHT INDEX!!!!
    engine.setup( 44100, 512, 3); 
    
    gui.setup("panel");
    gui.add( player.ui );
    gui.setPosition( 20, 20);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    gui.draw();
    
    ofDrawRectangle( 0, ofGetHeight()-20, ofGetWidth()*player.playhead(), 20 );
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){}
void ofApp::mouseMoved(int x, int y){}
void ofApp::mouseDragged(int x, int y, int button){}
void ofApp::mousePressed(int x, int y, int button){}
void ofApp::mouseReleased(int x, int y, int button){}
void ofApp::mouseEntered(int x, int y){}
void ofApp::mouseExited(int x, int y){}
void ofApp::windowResized(int w, int h){}
void ofApp::gotMessage(ofMessage msg){}
void ofApp::dragEvent(ofDragInfo dragInfo){ }
//--------------------------------------------------------------
