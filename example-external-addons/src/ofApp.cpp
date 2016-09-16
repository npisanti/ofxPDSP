
#include "ofApp.h"

// sometimes you'd like to use some external addons that use the audio callback
// like in the oF example
// for this you can extend ofxPDSPAddonWrapper
// in this example we port to ofxPDSP the oF audioInputExample

// we use our AddonWrapper variables as public, as in the oF example
// but remember that using variable this way is NOT THREAD SAFE
// to limiting the damage we will use the atomic version of the variables
// for info about atomic variables: http://en.cppreference.com/w/cpp/atomic/atomic 

//--------------------------------------------------------------
void ofApp::setup(){
    
    int bufferSize = 256;
    
    //-------------------GRAPHIC SETUP--------------
	
	ofSetVerticalSync(true);
	ofSetCircleResolution(80);
	ofBackground(54, 54, 54);	
    
    //--------PATCHING-------
    engine.setChannels( 2, 2 ); // two inputs, two outputs

    wrapper.setChannels(2, 0); // we just process the inputs
    
    wrapper.setup(bufferSize); // we wrote this routine in this example
    
    engine.audio_in(0) >> wrapper.in(0); // you can use the wrapper in(int num) function only at the end of a chain
    engine.audio_in(1) >> wrapper.in(1); // (likewise you can use the out(int num) just at the beginning
    wrapper.out_silent() >> engine.blackhole(); // units and modules patched to nothing are not processed
                                                // so we have to patch a dummy output to engine.blackhole()
                                                // in this way we are just processing, outputting to nothing
    // variables from oF example
    drawCounter		= 0;
    scaledVol		= 0.0;
    volHistory.assign(400, 0.0);

    //------------SETUPS AND START AUDIO-------------
    engine.listDevices();
    engine.setDeviceID(0); // REMEMBER TO SET THIS AT THE RIGHT INDEX!!!!
    engine.setup( 44100, bufferSize, 3); 
    
}

//---------------AddonWrapper class setup!----------------------
void AddonWrapper::setup( int bufferSize ){
    	
	left.assign(bufferSize, 0.0);
	right.assign(bufferSize, 0.0);

	bufferCounter	= 0;
	smoothedVol     = 0.0;

}

//--------------------------------------------------------------
void ofApp::update(){ // copied from oF example
    
	//lets scale the vol up to a 0-1 range 
	scaledVol = ofMap(wrapper.smoothedVol, 0.0, 0.17, 0.0, 1.0, true);

	//lets record the volume into an array
	volHistory.push_back( scaledVol );
	
	//if we are bigger the the size we want to record - lets drop the oldest value
	if( volHistory.size() >= 400 ){
		volHistory.erase(volHistory.begin(), volHistory.begin()+1);
	}
}


//-----------------copied with some modifications---------------
void ofApp::draw(){
	ofSetColor(225);
	ofDrawBitmapString("AUDIO INPUT EXAMPLE", 32, 32);
	
	ofNoFill();
	
	// draw the left channel:
	ofPushStyle();
		ofPushMatrix();
		ofTranslate(32, 170, 0);
			
		ofSetColor(225);
		ofDrawBitmapString("Left Channel", 4, 18);
		
		ofSetLineWidth(1);	
		ofDrawRectangle(0, 0, 512, 200);

		ofSetColor(245, 58, 135);
		ofSetLineWidth(3);
					
			ofBeginShape();
			for (unsigned int i = 0; i < wrapper.left.size(); i++){
				ofVertex(i*2, 100 -wrapper.left[i]*180.0f);
			}
			ofEndShape(false);
			
		ofPopMatrix();
	ofPopStyle();

	// draw the right channel:
	ofPushStyle();
		ofPushMatrix();
		ofTranslate(32, 370, 0);
			
		ofSetColor(225);
		ofDrawBitmapString("Right Channel", 4, 18);
		
		ofSetLineWidth(1);	
		ofDrawRectangle(0, 0, 512, 200);

		ofSetColor(245, 58, 135);
		ofSetLineWidth(3);
					
			ofBeginShape();
			for (unsigned int i = 0; i < wrapper.right.size(); i++){
				ofVertex(i*2, 100 -wrapper.right[i]*180.0f);
			}
			ofEndShape(false);
			
		ofPopMatrix();
	ofPopStyle();
	
	// draw the average volume:
	ofPushStyle();
		ofPushMatrix();
		ofTranslate(565, 170, 0);
			
		ofSetColor(225);
		ofDrawBitmapString("Scaled average vol (0-100): " + ofToString(scaledVol * 100.0, 0), 4, 18);
		ofDrawRectangle(0, 0, 400, 400);
		
		ofSetColor(245, 58, 135);
		ofFill();		
		ofDrawCircle(200, 200, scaledVol * 190.0f);
		
		//lets draw the volume history as a graph
		ofBeginShape();
		for (unsigned int i = 0; i < volHistory.size(); i++){
			if( i == 0 ) ofVertex(i, 400);

			ofVertex(i, 400 - volHistory[i] * 70);
			
			if( i == volHistory.size() -1 ) ofVertex(i, 400);
		}
		ofEndShape(false);		
			
		ofPopMatrix();
	ofPopStyle();
	
	drawCounter++;
	
	ofSetColor(225);
	string reportString = "buffers received: "+ofToString(wrapper.bufferCounter)+"\ndraw routines called: "+ofToString(drawCounter)+"\n";
	ofDrawBitmapString(reportString, 32, 589);
}

//---------------AddonWrapper class DSP-------------------------
void AddonWrapper::audioProcess(float *input, int bufferSize, int nChannels){
    // this is copied from oF audioInputExample
    
    float curVol = 0.0;
	
	// samples are "interleaved"
	int numCounted = 0;	

	//lets go through each sample and calculate the root mean square which is a rough way to calculate volume	
	for (int i = 0; i < bufferSize; i++){
		left[i]		= input[i*2]*0.5;
		right[i]	= input[i*2+1]*0.5;

		curVol += left[i] * left[i];
		curVol += right[i] * right[i];
		numCounted+=2;
	}
	
	//this is how we get the mean of rms :) 
	curVol /= (float)numCounted;
	
	// this is how we get the root of rms :) 
	curVol = sqrt( curVol );
	
	smoothedVol = (smoothedVol * 0.93);
	smoothedVol = (smoothedVol + 0.07 * curVol);
	
	bufferCounter++;
    
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
