#include "ofApp.h"

// before looking at this check out the basics examples

// Graincloud example. At the moment file loading is managed using libsndfile on Linux / Win
// it will work on Linux out of the box 
// but on Win you will have to find a way to install it and dynamically link it
// and then you have to go into ofxPDSP flags.h file and decomment #define PDSP_USE_LIBSNDFILE
// on OSX/iOS libaudiodecoder is used

//--------------------------------------------------------------
void ofApp::setup(){

    //--------GRAINCLOUD-----------------------
    float scaleAudio = 2.0f;
    grainVoices = cloud.getVoicesNum();
    
    // sampleData is an instance of pdsp::SampleBuffer
    sampleData.setVerbose(true); 
    sampleData.load("data/dreadlock_ride2.wav"); // at the moment this will work only on linux and osx
                                                 // many debugger (included gdb and xcode) change the base path 
                                                 // when you run your app in debug mode
                                                 // so sometimes on you have to use an absolute path
    cloud.setWindowType(pdsp::Triangular); // select the shape of the grain envelope
            // available windows: Rectangular, Triangular, Hann, Hamming, Blackman, BlackmanHarris, SineWindow, Welch
    cloud.setSample(&sampleData); // give to the pdsp::GrainCloud the pointer to the sample

    0.00f >> posX >> cloud.in_position();
             jitY >> cloud.in_position_jitter();  
    500.0f >> cloud.in_length();
    0.9f   >> cloud.in_density();
    20.0f  >> cloud.in_distance_jitter();
      0.5f >> cloud.in_pitch_jitter();
    -12.0f >> cloud.in_pitch();  
  
    // ampSlew is a pdsp::CRSlew, it smooths the control signal
            ampSlew.setSlewTime(50.0f);    
    0.0f >> ampSlew; 
            ampSlew * scaleAudio >> voiceAmpL.in_mod(); 
            ampSlew * scaleAudio >> voiceAmpR.in_mod();

    cloud.out_L() >> voiceAmpL >> engine.audio_out(0); 
    cloud.out_R() >> voiceAmpR >> engine.audio_out(1); 
    
    cout<<"finished patching\n";
    
    
    //ui values-------------------------------
    uiWidth  = ofGetWidth()-40; 
    uiHeigth = ofGetHeight()-40;
    uiX      = 20;
    uiY      = 20;
    uiMaxX = uiX + uiWidth;
    uiMaxY = uiY + uiHeigth;   
    drawGrains = false;
 
    //graphic setup---------------------------
    ofSetVerticalSync(true);
    ofDisableAntiAliasing();
    ofBackground(0);
    ofSetColor(ofColor(0, 255, 255));
    ofNoFill();
    ofSetLineWidth(1.0f);
    ofSetFrameRate(24);

    // create an ofFbo with the plotted waveform
    waveformGraphics.setWaveform(sampleData, 0, ofColor(0, 100, 100, 255), uiWidth, uiHeigth);
    
    
    //-----------------init and start audio-------------
    engine.listDevices();
    engine.setDeviceID(0); // REMEMBER TO SET THIS AT THE RIGHT INDEX!!!!
    engine.setup( 44100, 512, 3 ); 
    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofDrawRectangle(uiX, uiY, uiWidth, uiHeigth);    
    
    waveformGraphics.draw(uiX, uiY);
    ofDrawBitmapString (sampleData.filePath, uiX+5, uiY+15);
    
    if(drawGrains){
        //draw position cross
        ofDrawLine(controlX, uiY, controlX, uiMaxY);
        ofDrawLine(uiX, controlY, uiMaxX, controlY);
        //draw grains
        ofSetRectMode(OF_RECTMODE_CENTER);
        int grainsY = uiY + uiHeigth/2;
        for (int i=0; i<grainVoices; ++i){
            float xpos = uiX + (uiWidth * cloud.meter_position(i));
            float dimensionX = cloud.meter_env(i)*10;
            float dimensionY = cloud.meter_env(i)*50;
            ofDrawRectangle(xpos, grainsY, dimensionX, dimensionY);
        }       
    }

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
void ofApp::controlOn(int x, int y){
    ofMap(x, uiX, uiMaxX, 0.0f, 1.0f, true) >> posX;
    ofMap(y, uiY, uiMaxY, 0.25f, 0.0f) >> jitY;
    
    if(x > uiX && x<uiMaxX && y>uiY && y<uiMaxY){
        1.0f >> ampSlew;
        drawGrains = true;
        controlX = x;
        controlY = y;
    }
}

void ofApp::mouseDragged(int x, int y, int button){
    controlOn(x, y);
}

void ofApp::mousePressed(int x, int y, int button){
    controlOn(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    0.0f >> ampSlew;
    drawGrains = false;
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
