#include "ofApp.h"

// before looking at this check out the basics examples

// pdsp sequencing 
//
// the sequence code is executed in the DSP thread, so please don't hold lock or allocate / deallocate memory
// ofRandom is not thread-safe for this use, so it's better to use internal random function

//--------------------------------------------------------------
void ofApp::setup(){
 
    engine.sequencer.setTempo( 108 );
    
    // pdsp::Function can be used to make generative sequencers
    
    // this assignable function is executed each 16th
    // frame is the step number since the start of engine timeline
    // ( you can change the variable name, but it has to be int )
    seq.code = [&]( int frame ) noexcept { 
        // synth -------
        if( seq.chance( 0.5f ) ){
            seq.send("gate", 1.0f ); // sends note on to "gate" out
        }else{
            seq.send("gate", 0.0f ); // note off
        }
       
        static float akebono[] = { 72.0f, 74.0f, 75.0f, 79.0f, 80.0f, 84.0f, 86.0f, 87.0f }; // akebono scale
        float p = akebono[ seq.dice( 8 ) ]; // random note from scale
     
        seq.send( "pitch", p );
        
        // kick -------
        const float x = 1.0f;
        const float o = 0.0f;
        static float ks[] = { x,x,x,o, o,o,o,o, x,o,o,o, o,o,o,o };
        
        // you can use modulo operations 
        // to get the step index for a sequence
        seq.send( "kick", ks[frame%16] );
    };
    
    seq.out_trig("gate") >> lead.in("trig");
    seq.out_value("pitch") >> lead.in("pitch");
    seq.out_trig("kick") >> kick.in("trig");
    
    // patching to audio outputs
    kick * dB(-6.0f) >> engine.audio_out(0);
    kick * dB(-6.0f) >> engine.audio_out(1);
    
    lead * dB(-6.0f) >> engine.audio_out(0);
    lead * dB(-6.0f) >> engine.audio_out(1);
    
    //------------SETUPS AND START AUDIO-------------
    engine.listDevices();
    engine.setDeviceID(0); // <--- set this at the right index
    engine.setup( 44100, 512, 3); 
    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    ofSetColor(255);
    // printing out the instructions
    string info = "master playhead (bars): ";
    info += to_string( engine.sequencer.meter_playhead() );
    info += " \n";
    info += "space : pause/play.\n";
    info += "s : stop.\n";

    ofDrawBitmapString(info, 50, 50 );
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    switch (key){
    case ' ': // pause / play
        if(engine.sequencer.isPlaying()){
            engine.sequencer.pause();
        }else{
            engine.sequencer.play();
        }
        break;
    case 's': // stop
        engine.sequencer.stop();
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
