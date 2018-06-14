#include "ofApp.h"

// before looking at this check out the basics examples

// often you want to give the sequences some data from the main thread
// you should avoid absolutely using locks and instead use circular buffers
// and std::atomic values instead

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(0);

    chance = 1.0f;
    
    //--------PATCHING-------
    engine.sequencer.init( 1, 1, 200.0f); 
    engine.sequencer.sections[0].out_trig(0)  >> lead.in("trig"); 
    engine.sequencer.sections[0].out_value(1) >> lead.in("pitch"); 
    lead * dB(-6.0f) >> engine.audio_out(0);
    lead * dB(-6.0f) >> engine.audio_out(1);
    engine.sequencer.sections[0].launchCell(0); // launch seq

    //-------- buffer init ----------
    buffer.resize( 12 ); // a number big enough for the number of changes before running the sequence code
    index = 0;


    circularWrite(); // no problem running this while the engine runs

    // -------------- this code runs in the audio thread ----------------------
    engine.sequencer.sections[0].sequence(0).code = [&] () noexcept { 
        pdsp::Sequence & seq = engine.sequencer.sections[0].sequence(0);
        seq.begin( 8.0, 1.0 );
        
        // we save the index for reading, so there is no problem if it changes while this code is running
        int read = index; 
        
        for (size_t i=0; i<buffer[read].size(); ++i){
            if(pdsp::chance( chance) ){ // chance could also be changed while this code runs, no problem
                seq.message( double(i), 1.0f,  0 );
                seq.message( double(i), buffer[read][i], 1 );
            }
        }
        seq.end();
    };
    // ------------------------------------------------------------------------


    //------------SETUPS AND START AUDIO-------------
    engine.listDevices();
    engine.setDeviceID(0); // REMEMBER TO SET THIS AT THE RIGHT INDEX!!!!
    engine.setup( 44100, 512, 3); 
    
}

//--------------------------------------------------------------
void ofApp::circularWrite(){
    // this code runs in the ofApp thread
    
    // we save the index for writing
    int write = index + 1; // the engine is not reading there
    if( write >= (int) buffer.size() ){ write = 0; } // rebounds index
    
    // now we generate or copy some data
    buffer[write].resize( rand()%5 + 3 );
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
