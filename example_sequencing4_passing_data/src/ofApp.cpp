#include "ofApp.h"

// before looking at this check out the basics examples

// often you want to give the sequences some data from the main thread
// you should avoid absolutely using locks and use
// std::atomic values and circular buffers instead 
// (or even better, SPSC queues, but for hacky things a circular buffer is fine )

// remember: real audio waits for nothing!
// http://www.rossbencina.com/code/real-time-audio-programming-101-time-waits-for-nothing

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(0);

    engine.sequencer.setTempo(  200.0f); 

    chance = 1.0f;
    
    //-------- buffer init ----------
    buffer.resize( 12 ); // a number big enough for the number of changes before running the sequence code
    index = 0;

    circularWrite(); // no problem running this while the engine runs

    // ---- this code runs in the audio thread ----
    seq.code = [&] ( int frame ) noexcept {
        int read = index; // is important to memorize thi index 
        
        int steps = buffer[read].size();
        if( seq.chance( chance) ){
            seq.send( "gate", 1.0f );
            seq.send( "pitch", buffer[read][frame%steps] );
        }
    };
    
    // ---------- patching -----------------------
    seq.out_trig("gate")  >> lead.in("trig"); 
    seq.out_value("pitch") >> lead.in("pitch"); 
    lead * dB(-6.0f) >> engine.audio_out(0);
    lead * dB(-6.0f) >> engine.audio_out(1);

    //------------ setups and starts audio -------------
    engine.listDevices();
    engine.setDeviceID(0); // <--- remember to set your index 
    engine.setup( 44100, 512, 3); 
    
}

//--------------------------------------------------------------
void ofApp::circularWrite(){
    // this code runs in the ofApp thread
    
    // we save the index for writing
    int write = index + 1; // the engine is not reading there
    if( write >= (int) buffer.size() ){ write = 0; } // rebounds index
    
    // now we generate or copy some data
    buffer[write].resize( (rand()%8)*2 + 8 );
    for( size_t i=0; i<buffer[write].size(); ++i ){
        // random notes from akebono scale
        static float akebono[] = { 72.0f, 74.0f, 75.0f, 79.0f, 80.0f,  84.0f, 86.0f, 87.0f }; // akebono scale
        buffer[write][i] = akebono[rand()%8];
    }
    
    // and now we update the index, this is thread-safe as index is std::atomic
    index = write;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    string info = "press r to change the sequence data\n";
    info += "note chance = ";
    info += ofToString( chance );
    info += "\n";
    info += "click with the mouse to change, mapped to x axis";
    
    ofDrawBitmapString(info, 20, 20 );
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    switch (key){
        case 'r':
            circularWrite();
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
    // chance is thread safe to set
    chance = ofMap(x, 50, ofGetWidth()-50, 0.0f, 1.0f, true );
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    // chance is thread safe to set
    chance = ofMap(x, 50, ofGetWidth()-50, 0.0f, 1.0f, true );
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
