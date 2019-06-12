
#include "ofApp.h"

// before looking at this check out the basics examples

// you can use filters, detectors and envelope followers to divide the signal in frequencies and meter them
// most of the work is done inside AudioAnalyzerBand.cpp

//--------------------------------------------------------------
void ofApp::setup(){
    
    engine.setChannels(2, 2); // activate 2 input channels
    
    // patching the scope
    engine.audio_in(0) >> scope >> engine.blackhole();
    
    // decomment for a longer scope time window
    //scope.set(512*16); 
    
    // setting up analyzer
    gui.setup("", "analyzer.xml", 20, 20);
    gui.setName("MULTIBAND ANALYZER");

    bands.resize( 3 );        
    for (size_t i=0; i<bands.size(); ++i){ 
        
        string name = "band ";
        name+= ofToString(i+1);
      
        bands[i].setup ( engine, name );               
      
        gui.add( bands[i].ui );
    }
    
    gui.loadFromFile("analyzer.xml");

    ofSetWindowTitle( "psdp example-analysis");

    // start your audio engines!
    engine.listDevices();
    engine.setDeviceID(0); 

    // if listDevices() lists different devices for in and out you need to make something like this 
	//engine.setInputDeviceID(0);    
    //engine.setOutputDeviceID(2); // a valid output device is required
    
    engine.setup( 44100, 512, 3); 
  
}


//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);

    gui.draw();

    // draws some bars
    ofSetColor(255);
    ofFill();

    ofTranslate(    gui.getPosition().x + 20 + gui.getWidth(), 
                    gui.getPosition().y );
    for (size_t i=0; i<bands.size(); ++i){
        ofSetColor(255);
        string name = "band ";
        name+= ofToString(i+1);
        ofDrawBitmapString( name, 0, 20 );
        ofDrawBitmapString( "rms", 0, 45 );
        ofDrawBitmapString( "peak", 0, 75 );
        ofDrawBitmapString( "onset", 0, 105 );
        
        ofDrawRectangle( 40, 30, 180*bands[i].meter_rms(), 20);
        ofDrawRectangle( 40, 60, 180*bands[i].meter_peak(), 20);
        
        int white = 255 * bands[i].meter_onset();
        white = (white > 255) ? 255 : white;
         
        ofSetColor(white);
        ofDrawRectangle( 40, 90, 180, 20);        
        
        ofTranslate(0, 120);
    }

    // draws the scope
    ofSetColor(255);
    ofDrawBitmapString( "input scope", 0, 35 );
    scope.draw( 0, 50, 220, 100);

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
