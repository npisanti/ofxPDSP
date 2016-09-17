#include "ofApp.h"


#define HOST "localhost"
#define PORT 12345

//--------------------------------------------------------------
void ofApp::setup(){

	ofBackground(40, 100, 40);

	// open an outgoing connection to HOST:PORT
	sender.setup(HOST, PORT);
    
    ofSetWindowTitle("example-osc-input-sender");
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
  

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

    
void ofApp::moveRoutine( int x, int y ) {
	ofxOscMessage m;
	m.setAddress("/pitch");
    float pitch = ofMap(x, 0.0f, ofGetWidth(), 36.0f, 84.0f, true );
	m.addFloatArg(pitch);
	sender.sendMessage(m, false);

	ofxOscMessage m2;    
	m2.setAddress("/amp");
	float amp = ofMap(y, 0.0f, ofGetHeight(), 1.0f, 0.0f, true );
    m2.addFloatArg(amp);
	sender.sendMessage(m2, false);    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    moveRoutine( x, y );
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    moveRoutine( x, y );
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    moveRoutine( x, y );
    
	ofxOscMessage m;
	m.setAddress("/gate");
	m.addFloatArg(1.0f);
	sender.sendMessage(m, false);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	ofxOscMessage m;
	m.setAddress("/gate");
	m.addFloatArg(0.0f);
	sender.sendMessage(m, false);

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

