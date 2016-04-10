#include "ofApp.h"

// before looking at this example, be shure to check out the basics with the ofxPDSP wiki tutorials:
// https://github.com/npisanti/ofxPDSP/wiki

// example for the pdsp::SeqChange class

//--------------------------------------------------------------
void ofApp::setup(){
    
    //-------------------GRAPHIC SETUP--------------
    ofSetWindowShape(640, 360);
    
    //--------PATCHING-------
    seq_mode = 0;
    quantize = true;
    quantime = 1.0/8.0; // 1/4th
    
    engine.score.setTempo(120.0f);
    
    engine.score.sections.resize(2);
 
    engine.score.sections[0].out_trig(0)  >> bass.in("trig");
    engine.score.sections[0].out_value(1) >> bass.in("pitch");
    engine.score.sections[1].out_trig(0)  >> lead.in("trig");
    engine.score.sections[1].out_value(1) >> lead.in("pitch");
    
    bass * (dB(-6.0f) * panL(-0.25f)) >> engine.audio_out(0);
    bass * (dB(-6.0f) * panR(-0.25f)) >> engine.audio_out(1);
    
    lead * (dB(-6.0f) * panL(0.25f)) >> engine.audio_out(0);
    lead * (dB(-6.0f) * panR(0.25f)) >> engine.audio_out(1);


    // writing our sequences
    bass_seqs.resize(4); 
    lead_seqs.resize(4); 
    float o = -1.0f;
    
    bass_seqs[0].set( { {1.0f, o}, {36.0f, o} }, 16.0, (1.0/8.0) );
    bass_seqs[1].set( { {1.0f, o}, {38.0f, o} }, 16.0, (1.0/8.0) );
    bass_seqs[2].set( { {1.0f, o}, {39.0f, o} }, 16.0, (1.0/8.0) );
    bass_seqs[3].set( { { 1.0f,  1.0f}, 
                        {41.0f, 42.0f} }, 16.0, (1.0/8.0) );
                        
    lead_seqs[0].set( { { 1.0f,  0.75f, 1.0f,  0.75f },
                        { 84.0f, 82.0f, 84.0f, 80.0f } }, 8.0, 1.0 );
    lead_seqs[1].set( { { 1.0f,  0.75f, 1.0f,  0.75f},
                        { 72.0f, 74.0f, 75.0f, 77.0f} }, 8.0, 1.0 );
    lead_seqs[2].set( { { 1.0f,  0.75f, 1.0f,  0.75f},
                        { 84.0f, 86.0f, 89.0f, 87.0f} }, 8.0, 1.0 );
    lead_seqs[3].set( { { 0.5f,  1.0f,  0.5f,  1.0f },
                        { 80.0f, 79.0f, 80.0f, 79.0f } }, 8.0, 1.0 );

    // assigning the sequences to the sections
    for(int i = 0; i<4; ++i){
        engine.score.sections[0].setCell(i, &bass_seqs[i], &alternate);
        engine.score.sections[1].setCell(i, &lead_seqs[i], pdsp::Behavior::Nothing);
    } 

    // this is a custom SeqChange for our bass patterns
    alternate_flag = true;
    alternate.code = [&]() noexcept {
        if(alternate_flag){
            alternate_flag = false;
            return 0;
        }else{
            alternate_flag = true;
            return (1+pdspDice(3));
        }
    };

    
    //------------SETUPS AND START AUDIO-------------
    engine.listDevices();
    engine.setDeviceID(0); // REMEMBER TO SET THIS AT THE RIGHT INDEX!!!!
    engine.setup( 44100, 512, 3); 
    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    ofSetColor(255);
    ofNoFill();
    ofTranslate(50,50);
    
    for(int i=0; i<4; ++i){
        ofDrawRectangle(50*i, 0, 50, 50);
        ofDrawRectangle(50*i, 50, 50, 50);
    }
    
    ofFill();
    
    // we can query the active index, the sequence playhead and length in bars with those thread-safe methods
    for(int i=0; i<(int)engine.score.sections.size(); ++i){
        int active_seq = engine.score.sections[i].meter_current(); 
        if( active_seq < 4){
            float playheadPercent = engine.score.sections[i].meter_playhead() / engine.score.sections[i].meter_length();
            ofDrawRectangle(50*active_seq, 50*i, 50.0f*playheadPercent, 50);
        }
    }
    
    // printing out the instructions
    string info = "master playhead (bars): ";
    info += to_string( engine.score.meter_playhead() );
    info += " \n";
    info += "1-4 : launch bass sequences\n";
    info += "q-w-e-r : launch lead sequences\n";
    info += "a : change lead cell change behavior. Mode = ";
    switch(seq_mode){
        case 0: info += "one shot. \n";
            break;
        case 1: info += "loop. \n";
            break;
        case 2: info += "next. \n";
            break;
        case 3: info += "random. \n";
            break;
    }
    info += "5 : stop bass sequence.\n";
    info += "t : stop lead sequence.\n";
    info += "6-0: set launch quantization. quantization = ";
    if(quantize){
        info += to_string(quantime);
        info += " bars\n";
    }else{
        info += "no quant\n";
    }
    info += "space : pause/play.\n";
    info += "s : stop.\n";

    ofDrawBitmapString(info, 0, 130 );
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    // we can launch our sequences with the launchCell method, with optional quantization
    switch (key){
    case '1':
        engine.score.sections[0].launchCell( 0, quantize, quantime);
        break;
    case '2':
        engine.score.sections[0].launchCell( 1, quantize, quantime);
        break;
    case '3':
        engine.score.sections[0].launchCell( 2, quantize, quantime);
        break;
    case '4':
        engine.score.sections[0].launchCell( 3, quantize, quantime);
        break;
    case '5':
        engine.score.sections[0].launchCell( -1, quantize, quantime);
        break;
    case '6':
        quantize = true;
        quantime = 1.0;
        break;
    case '7':
        quantize = true;
        quantime = 1.0/4.0;
        break;
    case '8':
        quantize = true;
        quantime = 1.0/8.0;
        break;
    case '9':
        quantize = true;
        quantime = 1.0/16.0;
        break;
    case '0':
        quantize = false;
        break;
    case 'q':
        engine.score.sections[1].launchCell( 0, quantize, quantime);
        break;
    case 'w':
        engine.score.sections[1].launchCell( 1, quantize, quantime);
        break;
    case 'e':
        engine.score.sections[1].launchCell( 2, quantize, quantime);
        break;
    case 'r':
        engine.score.sections[1].launchCell( 3, quantize, quantime);
        break;
    case 't':
        engine.score.sections[1].launchCell( -1, quantize, quantime);
        break;
    case 'a':
        seq_mode++;
        if(seq_mode > 3){ seq_mode = 0; }
        switch(seq_mode){
            case 0:
                for(int i = 0; i<4; ++i){
                    engine.score.sections[1].setChange(i, pdsp::Behavior::Nothing);
                } 
                break;
            case 1:
                for(int i = 0; i<4; ++i){
                    engine.score.sections[1].setChange(i, pdsp::Behavior::Loop);
                } 
                break;
            case 2:
                for(int i = 0; i<4; ++i){
                    engine.score.sections[1].setChange(i, pdsp::Behavior::Next);
                } 
                break;
            case 3:
                for(int i = 0; i<4; ++i){
                    engine.score.sections[1].setChange(i, pdsp::Behavior::RandomOther);
                } 
                break;
        }
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
