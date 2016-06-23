#include "ofApp.h"

// before looking at this check the basics and scoring examples

//--------------------------------------------------------------
void ofApp::setup(){
    
    //-------------------GRAPHIC SETUP--------------
    ofBackground(0);
    ofSetFrameRate(30);
    
    // PATCHING
    midiOut.setVerbose(true);
    midiOut.listPorts();
    midiOut.openPort(0); // set the right midi port
    
    // some sequences, basically just on/off notes
    sequence1.setDivision(8.0);
    sequence1.setLength(2.0);
    sequence1.set( { 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f } );
    sequence2.setDivision(16.0);
    sequence2.setLength(2.0);
    sequence2.set( { 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f } );
    sequence3.setDivision(32.0);
    sequence3.setLength(2.0);
    sequence3.set( { 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f } );
    
    // setup as usual, check example-scoring for more info
    engine.score.setTempo(172.0);
    
    engine.score.sections.resize(1); // we need just one section
    engine.score.sections[0].setCell(0, &sequence1, pdsp::Behavior::Self);
    engine.score.sections[0].setCell(1, &sequence2, pdsp::Behavior::Self);
    engine.score.sections[0].setCell(2, &sequence3, pdsp::Behavior::Self);
    

    // you connect the ScoreSections to midiOut like this
    // outputs a midi on on a message with value > 0.0f and a midi off message for a value <= 0.0f
    // first argument of gate() is midi channel, second the default note
    engine.score.sections[0].out_message() >> midiOut.gate(1, 60); 
    
    // if you patch an out to a note() this out will control the note number of the last patched gate()
    // so beware! use gate() and note() always one after the other, like this
    // engine.score.sections[0].out_message() >> midiOut.gate(1); 
    // engine.score.sections[1].out_message() >> midiOut.note();
    
    // you can also connect the out to a midi cc like this
    // first argument of cc() is midi channel, second the  midi CC number to send
    //engine.score.sections[0].out_message() >> midiOut.cc(1, 2);
    
    // in this example we are sending messages just to gate, so they will always be on the same note
    engine.score.sections[0].launchCell(0);
    engine.score.play();
    
    //----------------------AUDIO SETUP-------------
    engine.addMidiOut( midiOut );
    engine.listDevices();
    engine.setDeviceID(0); // REMEMBER TO SET THIS AT THE RIGHT INDEX!!!!
    engine.setup( 44100, 512, 3); 

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key){
        case '1':
            engine.score.sections[0].launchCell(0, false);
            break;
        case '2':
            engine.score.sections[0].launchCell(1, false);        
            break;
        case '3':
            engine.score.sections[0].launchCell(2, false);
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
