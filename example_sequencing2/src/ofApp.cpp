
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){    
    ofBackground(0);

    // ----------------------
    engine.sequencer.setTempo( 200 );
    
    // ---- drums sequencer ----
    states.resize(16);

    dseq.timing = 16;
    dseq.code = [&] ( int frame ) noexcept { 
        // generates the seq every 4 bars + fills
        if( frame % 64 == 0 || frame % 128 == 112 ){ 
            states[0] = 0; 
            for(size_t i=1; i<states.size();++i){
                if( i%2==0 ){
                    states[i] = pdsp::dice(4);
                }else{
                    if( dseq.chance(0.5) ){
                        states[i] = 4; // ghost
                    }else{
                        states[i] = 6; // nothing
                    }
                }
                
            }
        }
        
        switch( states[frame%16] ){
            case 0: // kick 
                dseq.send( "bang", 0.7f );
                dseq.send( "sample", 0.0f );
            break;
            
            case 1: // snare 
                dseq.send( "bang", 0.7f );
                dseq.send( "sample", 1.0f );
            break;
            
            case 2: // hh
                dseq.send( "bang", 1.0f );
                dseq.send( "sample", 2.0f );
            break;
            
            case 3: // hh soft
                dseq.send( "bang", 0.5f );
                dseq.send( "sample", 2.0f );
            break;
            
            case 4: // shuffle
                dseq.send( "bang", 0.7f );
                dseq.send( "sample", 3.0f );
            break;
            
            default: break;
        }
    };

    dseq.out_trig( "bang" ) >> drums.in("trig");
    dseq.out_value( "sample" ) >> drums.in("select");
    
    
    // ---- sub reese sequencer ----
    rmode = 0;
    rseq.timing = 4;
    rseq.code = [&] ( int frame ) noexcept { 
        if( frame%32==0 ){
            rmode = 0;
        }
        
        if( frame%32==24 ){ // random fill in the last two of 8 bars
            rmode = 1 + rseq.dice(3);
        }

        static float tracks[4][2][8]= {
            {   // main
                { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
                { 29.f, 29.f, 29.f, 29.f, 29.f, 29.f, 31.f, 31.f }
            },
            {   // fill 1
                { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
                { 29.f, 29.f, 29.f, 29.f, 29.f, 29.f, 31.f, 31.f }
            },
            {   // fill 2
                { 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
                { 29.f, 29.f, 32.f, 32.f, 31.f, 31.f, 30.f, 30.f }
            },
            {   // fill 3
                { 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
                { 29.f, 29.f, 29.f, 29.f, 34.f, 34.f, 33.f, 33.f }
            }
        };
        
        rseq.send("gate", tracks[rmode][0][frame%8] );
        rseq.send("pitch", tracks[rmode][1][frame%8] );
    };
   
    rseq.out_trig("gate")  >> reese.in("trig");
    rseq.out_value("pitch") >> reese.in("pitch");
       
    // ---------------------   
       
    drums.add( ofToDataPath("break_kick.wav") );
    drums.add( ofToDataPath("break_snare.wav") );
    drums.add( ofToDataPath("break_hh.wav") );
    drums.add( ofToDataPath("break_ghost.wav") );
    3.0f >> drums.in("pitch");

    drums * dB(9.0f) >> engine.audio_out(0);
    drums * dB(9.0f)  >> engine.audio_out(1);
    
    reese * dB(-8.0f) >> engine.audio_out(0);
    reese * dB(-8.0f) >> engine.audio_out(1);

    reeseScope.set(4096); // this will make the scope buffersize longer    
    drums * dB(9.0f)  >> drumScope  >> engine.blackhole(); // scopes need to be patched to engine.blackhole()
    reese * dB(-6.0f) >> reeseScope >> engine.blackhole(); // blackhole() process module without outputting sound
    
    // ------------ setups and starts audio -------------
    engine.listDevices();
    engine.setDeviceID(0); // <----- remember to set your index
    engine.setup( 44100, 512, 3); 
    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    drumScope.draw(  20,  20, 290, 80);
    reeseScope.draw( 320, 20, 290, 80);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if( key == ' '){
        if(engine.sequencer.isPlaying()){
            engine.sequencer.stop();
        }else{
            engine.sequencer.play();
        }
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
