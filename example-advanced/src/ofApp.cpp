#include "ofApp.h"

// before looking at this, be shure to check out the basics with the ofxPDSP wiki tutorials:
// https://github.com/npisanti/ofxPDSP/wiki

// this is a more advanced example of some of ofxPDSP inner workings, using the oF audio callback 

// sequencing is a lot easier using Sequence class and some of the more advanced feature of ScoreSection
// but I still have to write some lessons and examples on that

// example of classes for generative music and sequencing --> score.h
// also example of basic synth patching  -------------------> music.h

//--------------------------------------------------------------
void ofApp::setup(){
    
    //-------------------GRAPHIC SETUP--------------
    ofBackground(0);
    ofDisableAntiAliasing();
    ofSetFrameRate(60);
    bassCanvasW = 36;
    bassCanvasH = 340;
    bassScoreCanvas.allocate(bassCanvasW, bassCanvasH, GL_RGBA);
    
    //-------------------------PATCHING--------------
 
    // connect scores gates and pitch outpus to synths
    music.bassGate  >> bass.in("trig");
    music.bassPitch >> bass.in("pitch");
    
    music.bleepGate  >> bleep.in("trig");
    music.bleepPitch >> bleep.in("pitch");

    // connect synths to stereo output
    bass * 0.75f >> pdspEngine.channels[0];
    bass * 0.25f >> pdspEngine.channels[1];
    
    bleep * 0.25f >> pdspEngine.channels[0]; 
    bleep * 0.75f >> pdspEngine.channels[1]; 
    
    cout<<"finished patching\n";
    
    //----------------------AUDIO SETUP-------------
    expectedBufferSize = 512;
    sampleRate = 44100.0;

    // ofxPDSPEngine automatically call this, if you are not using it you have to call it yourself
    ofxPDSPSetup(expectedBufferSize, sampleRate);
  
    // default oF audio callbacks
    ofSoundStreamListDevices();
    audioStream.setDeviceID(0);
    audioStream.setup(this, 2, 0, static_cast<int>(sampleRate), expectedBufferSize, 3);
    
    // start playing
    music.scoregen.play();
    
}

//--------------------------------------------------------------
void ofApp::update(){

    // update bass pattern graphics when bass pattern changes
    // music.bassPattern.updateScoreDraw is a bool used to make this operation thread-safe
    if(music.bassPattern.updateScoreDraw){
        bassScoreCanvas.begin();
            ofClear(255,255,255, 255);
            ofBackground(0);
            ofNoFill();
            ofSetColor(255);
            ofDrawRectangle(1, 1, bassCanvasW-2, bassCanvasH-2);
            
            double gateStart;
            double gateStop;
            double gateVal;
            bool gate = false;
            
            for(pdsp::ScoreMessage &msg : music.bassPattern.score){
                if(msg.lane == 0){
                    if(msg.value>0.0){
                        gateVal = msg.value;
                        if(!gate){
                            gateStart = msg.time;
                            gate = true;
                        }
                    }else{
                        if(gate){
                            gateStop = msg.time;
                            gate = false;
                            int rectH  = (gateStop - gateStart) * bassCanvasH;
                            ofSetColor( static_cast<int> (100.0 * gateVal));
                            ofFill();
                            ofDrawRectangle(2, gateStart*bassCanvasH +2, bassCanvasW-4, rectH);
                        }
                    }
                }
            }
            
            for(pdsp::ScoreMessage &msg : music.bassPattern.score){
                if(msg.lane == 1){
                    ofSetColor(255);
                    ofDrawBitmapString( msg.value, 9, (msg.time*bassCanvasH + 12) );
                }
            }
            
        bassScoreCanvas.end();
        music.bassPattern.updateScoreDraw = false;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    bassScoreCanvas.draw(16, 10);
    
    double playY = music.scoregen.sections[1].meter_playhead() * bassCanvasH + 11;
    ofSetColor(255);
    ofDrawLine(56, playY, 80, playY);

    visualizeSection(music.scoregen.sections[0], 4, 100, 11, 25, 25);
    visualizeSection(music.scoregen.sections[1], 2, 100, 36, 25, 25);
    
    string info = "master playhead (bars): ";
    info += to_string( music.scoregen.meter_playhead() );
    info += " / 16.0. \n";
    info += "1-4 : launch high bleeps.\n";
    info += "5 : change bleep launch mode. Mode = ";
    if(music.bleepSeq.oneShot.load()){
        info += "one shot. \n";
    }else{
        info += "loop. \n";
    }
    info += "q : launch bass pattern.\n";
    info += "w : launch empty bass pattern.\n";
    info += "e : stop bass pattern.\n";
    info += "space : pause/play.\n";
    info += "s : stop.\n";
    
    ofDrawBitmapString(info, 100, 90 );
}

//--------------------------------------------------------------
void ofApp::visualizeSection(const pdsp::ScoreSection & sect, int numCells, int x, int y, int w, int h){
        
    for(int i=0; i<numCells; ++i){
        if( sect.meter_next() == i ){
            ofSetColor(50);
            ofFill();
            ofDrawRectangle( x + i*w, y, w, h);
        }
        if( sect.meter_current() == i){
            ofSetColor(150);
            ofFill();
            ofDrawRectangle( x + i*w, y, w, h);
        }
        ofSetColor(255);
        ofNoFill();
        ofDrawRectangle( x + i*w, y, w, h); 
    }
}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer &outBuffer) {
    // pdsp::ScoreProcessor has always to be processed before the DSP system
    music.scoregen.process(outBuffer.getNumFrames());
    // we process the DSPs
    pdspEngine.processAndCopyInterleaved(outBuffer.getBuffer().data(), outBuffer.getNumChannels(), outBuffer.getNumFrames());

}

//--------------------------------------------------------------
void ofApp::exit() {
    // cleaning everything
	audioStream.stop();
	audioStream.close();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key){
       
        case '1': // launch patterns 
            music.scoregen.sections[0].launchCell(0, false, true, 0.0625);
            break;
        case '2':
            music.scoregen.sections[0].launchCell(1, false, true, 0.0625);        
            break;
        case '3':
            music.scoregen.sections[0].launchCell(2, false, true, 0.0625);
            break;
        case '4':
            music.scoregen.sections[0].launchCell(4, false, true, 0.0625);
            break;
        case '5': // select one shot / loop pattern behavior
            if(music.bleepSeq.oneShot.load()){
                music.bleepSeq.oneShot.store(false);
            }else{
                music.bleepSeq.oneShot.store(true);
            }
            break;
        case 'q': // launch bass pattern
            music.scoregen.sections[1].launchCell(0, false, true, 1.0 );
            break;
        case 'w': // launch empty bass pattern
            music.scoregen.sections[1].launchCell(1, false, true, 1.0 );
            break;            
        case 'e': // stop bass pattern (by passing -1 as index)
            music.scoregen.sections[1].launchCell(-1, false, true, 1.0 );
            break;
        case ' ': // pause / play
            if(music.scoregen.isPlaying()){
                music.scoregen.pause();
            }else{
                music.scoregen.play();
            }
            break;
        case 's': // stop
            music.scoregen.stop();
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
