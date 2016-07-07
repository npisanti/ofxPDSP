#include "ofApp.h"

// before looking at this check out the basics examples and the scoring1 example

// this is a more advanced example of some of ofxPDSP sequencing, check out score.h for the bass pattern code

//--------------------------------------------------------------
void ofApp::setup(){
    
    //-------------------GRAPHIC SETUP--------------
    ofBackground(0);
    ofDisableAntiAliasing();
    ofSetFrameRate(60);
    
    oneShot = false;
    
    //-------------------------SETTING UP SCORE--------------
 
    float o = -1.0f; // we need at least two values for each dimension to avoid ambigous calls
    bleep1.set( { {1.0f, o}, {77.0f, o} }, 16.0, 0.25); // 1.0f goes to out 0, pitch goes to out 1
    bleep2.set( { {1.0f, o}, {80.0f, o} }, 16.0, 0.25); // 1/16th division, 0.25 bars length
    bleep3.set( { {1.0f, o}, {84.0f, o} }, 16.0, 0.25); 
    bleep4.set( { {1.0f, o}, {89.0f, o} }, 16.0, 0.25); 

    // ScoreProcessor setup
    engine.score.setTempo(120.0);
    engine.score.setMaxBars(16.0);    
    engine.score.sections.resize(2);

    // adding the bleep patterns and settings their timings
    engine.score.sections[0].setCell(0, &bleep1, pdsp::Behavior::Next );
    engine.score.sections[0].setCell(1, &bleep2, pdsp::Behavior::Next );
    engine.score.sections[0].setCell(2, &bleep3, pdsp::Behavior::Next );
    engine.score.sections[0].setCell(3, &bleep4, pdsp::Behavior::Next );
    // SeqChange triggering quantized to 1/4th
    engine.score.sections[0].enableQuantization(0, 0.25 );
    engine.score.sections[0].enableQuantization(1, 0.25 );
    engine.score.sections[0].enableQuantization(2, 0.25 );
    engine.score.sections[0].enableQuantization(3, 0.25 );

    // set bass sequence
    engine.score.sections[1].setCell(0, &bassPattern, pdsp::Behavior::Self); //pdsp::Behavior contains some ready-made SeqChange
    engine.score.sections[1].enableQuantization( 0, 1.0 ); // nextCell quantized to the next bar
    engine.score.sections[1].setCell(1, nullptr, nullptr); // a nullptr sequence will stop the bass everything
    
    // we launch the sequences
    engine.score.sections[0].launchCell(0);
    engine.score.sections[1].launchCell(0); 

    
    //-------------------------PATCHING--------------
    engine.score.sections[0].out_trig(0)  >> bleep.in("trig"); 
    engine.score.sections[0].out_value(1) >> bleep.in("pitch");
    
    // connect section outputs to bass
    engine.score.sections[1].out_trig(0)  >> bass.in("trig");
    engine.score.sections[1].out_value(1) >> bass.in("pitch");
    engine.score.sections[1].out_value(1).setSlewTime(80.0f); // 50ms slew
    engine.score.sections[1].linkSlewControl(1, 2); // now the out 2 will control the slew time of the value output 1
                                                    // slew time is multiplied by the message value
                                                    // so for example 0.0f deativates slew and 2.0f doubles it
                                                    // (the effect is subtle in this example but it's there

    // connect synths to stereo output
    bass * (panL(-0.5f) * dB(-6.0f)) >> engine.audio_out(0);
    bass * (panR(-0.5f) * dB(-6.0f)) >> engine.audio_out(1);
    
    bleep * (panL(0.5f) * dB(-6.0f)) >> engine.audio_out(0);
    bleep * (panR(0.5f) * dB(-6.0f)) >> engine.audio_out(1);
    
    // graphics
    // ofxScoreSectionPlotter is an utility class for displaying active cells and sequences values
    bleepPatternGraphics.setup(400, 20, 0, engine.score.sections[0] ); // with 0 outputs we just draw the sequence visualizer
    
    bassPatternGraphics.setup(400, 100, 2, engine.score.sections[1] ); // width, eight, outputs to monitor, section to monitor
    bassPatternGraphics.setRange(1, 24.0f, 60.0f); // setting the range for the pitch output, otherwise the range is 0.0f<->1.0f

    //------------SETUPS AND START AUDIO-------------
    engine.listDevices();
    engine.setDeviceID(0); // REMEMBER TO SET THIS AT THE RIGHT INDEX!!!!
    engine.setup( 44100, 512, 3); 
}

//--------------------------------------------------------------
void ofApp::update(){
    // do not forget to update your plotters!
    bleepPatternGraphics.update();
    bassPatternGraphics.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    float masterX = (ofGetWidth() - 400) * 0.5f;
    
    ofSetColor(255); // this will change the color of the plotted graphics
    
    ofDrawBitmapString("bleep patterns:", masterX, 25 );    
    bleepPatternGraphics.draw(masterX, 30);
    
    ofDrawBitmapString("bass pattern:", masterX, 75 );
    bassPatternGraphics.draw(masterX, 80);


    string info = "master playhead (bars): ";
    info += to_string( engine.score.meter_playhead() );
    info += " / 16.0. \n";
    info += "1-4 : launch high bleeps.\n";
    info += "5 : change bleep launch mode. Mode = ";
    if(oneShot){
        info += "one shot. \n";
    }else{
        info += "loop. \n";
    }
    info += "q : launch bass pattern.\n";
    info += "w : launch empty bass pattern.\n";
    info += "e : stop bass pattern.\n";
    info += "space : pause/play.\n";
    info += "s : stop.\n";
    
    ofDrawBitmapString(info, masterX, 230 );


}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key){
       
        case '1': // launch patterns 
            engine.score.sections[0].launchCell(0, true, 0.0625);
            break;
        case '2':
            engine.score.sections[0].launchCell(1, true, 0.0625);        
            break;
        case '3':
            engine.score.sections[0].launchCell(2, true, 0.0625);
            break;
        case '4':
            engine.score.sections[0].launchCell(4, true, 0.0625);
            break;
        case '5': // select one shot / loop pattern behavior
            if(oneShot){
                engine.score.sections[0].setChange(0, pdsp::Behavior::Next );
                engine.score.sections[0].setChange(1, pdsp::Behavior::Next );
                engine.score.sections[0].setChange(2, pdsp::Behavior::Next );
                engine.score.sections[0].setChange(3, pdsp::Behavior::Next );
                oneShot = false;
            }else{
                engine.score.sections[0].setChange(0, pdsp::Behavior::Nothing );
                engine.score.sections[0].setChange(1, pdsp::Behavior::Nothing );
                engine.score.sections[0].setChange(2, pdsp::Behavior::Nothing );
                engine.score.sections[0].setChange(3, pdsp::Behavior::Nothing );
                oneShot = true;
            }
            break;
        case 'q': // launch bass pattern
            engine.score.sections[1].launchCell(0, true, 1.0 );
            break;
        case 'w': // launch empty bass pattern
            engine.score.sections[1].launchCell(1, true, 1.0 );
            break;            
        case 'e': // stop bass pattern (by passing -1 as index)
            engine.score.sections[1].launchCell(-1, true, 1.0 );
            break;
        case ' ': // pause / play
            if(engine.score.isPlaying()){
                engine.score.pause();
            }else{
                engine.score.play();
            }
            break;
        case 's': // stop
            engine.score.stop();
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
