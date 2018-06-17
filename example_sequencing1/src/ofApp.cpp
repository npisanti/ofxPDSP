#include "ofApp.h"

// before looking at this check out the basics examples

// generative sequences
//
// PS: the sequence code is executed in the DSP thread, so please don't hold lock or allocate / deallocate memory
// if you need some heavy computation make it in another thread and copy it to the sequence when it's time
// as the code is executed into the DSP thread is better not to use ofRandom(), it is not thread safe
// use pdsp::dice(), pdsp::chance(), pdsp::uran() and pdsp::bran() instead (only in the sequence functions!) 


//--------------------------------------------------------------
void ofApp::setup(){
 
    // ------------------------- PATCHING -------------------------------------
    seq_mode = 0;
    quantize = true;
    quantime = 1.0/8.0; // 1/4th
    
    engine.sequencer.setTempo(120.0f); // sets the tempo of the music

    engine.sequencer.sections.resize(2); 
    // this is very important, each section can play just one sequence at time
    // you have to resize it to a sufficient number before starting to assign sequences
 
    // each section can output the messages to one or more outputs, as values or triggers
    // thing about them as gate/cv outputs, gate=trig, value=cv
    engine.sequencer.sections[0].out_trig(0)  >> kick.in("trig"); // assign the first sequence output to trig
    engine.sequencer.sections[0].out_value(1) >> kick.in("pitch"); // assign the second sequence output to values
    engine.sequencer.sections[1].out_trig(0)  >> lead.in("trig"); // assign the first sequence output to trig
    engine.sequencer.sections[1].out_value(1) >> lead.in("pitch"); // assign the second sequence output to values
    
    // decomment this to slew the lead value output signal
    // engine.sequencer.sections[1].out_value(1).enableSmoothing(100.0f);
    
    // patching (with panning)
    kick * (dB(-6.0f) * pdsp::panL(-0.25f)) >> engine.audio_out(0);
    kick * (dB(-6.0f) * pdsp::panR(-0.25f)) >> engine.audio_out(1);
    
    lead * (dB(-6.0f) * pdsp::panL(0.25f)) >> engine.audio_out(0);
    lead * (dB(-6.0f) * pdsp::panR(0.25f)) >> engine.audio_out(1);

    // ------------------------ SEQUENCING ------------------------------------

    // kick sequences
    kick_seqs.resize(4); 
    
    // we can use "set" to use an inlined array with the step values
    // "steplen" and "bars" are used for the timing and lenght of the sequence
    // init step length is 1/16 and init seq lenght is 1 bar
    float o = -1.0f; // when using "set" negative outputs are ignored, 'o' is graphically useful
    
    // kick synth has an AHR envelope, only positive values will trigger it
    
    kick_seqs[0].steplen = 1.0/4.0;
    kick_seqs[0].bars = 1.0; // this can also be omitted, 1.0 is the default value
    kick_seqs[0].set( { 1.0f, o, o, o } );
    
    kick_seqs[1].steplen = 1.0/4.0;
    kick_seqs[1].set( { 1.0f, 0.5f, 0.8f, 0.5f } );
    
    kick_seqs[2].steplen = 1.0/8.0;
    kick_seqs[2].set( { 1.0f, 0.5f, o, o,  o, o, 0.3f, o } );
    
    kick_seqs[3].steplen = 1.0/16.0;
    kick_seqs[3].bars    = 2.0; 
    kick_seqs[3].set( { 1.0f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,   
                        0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f }); 
    
    lead_seqs.resize(4);
    
    // lead sequences ( and sequences examples ) 
    lead_seqs[0].steplen = 1.0/16.0;    
    lead_seqs[0].set( { { 1.0f,  0.0f,  0.75f,  0.0f,  1.0f,  0.0f, 0.75f, 0.0f }, // triggers outputs
                        { 84.0f, o,     82.0f,  o,     84.0f, o,    80.0f, o    } }); // pitch outputs
                        
    lead_seqs[1].steplen = 1.0/16.0;  
    lead_seqs[1].set( { { 1.0f,  o,   o,   o,   1.0f,   o,   o,    0.0f },
                        { 72.0f, o,   o,   o,   75.f,   o,   77.f, o    } } );
     
    
    float minor_penta[] = { 72.0f, 75.0f, 77.0f, 79.0f, 82.0f, 84.0f, 87.0f, 89.0f }; // minor pentatonic scale
 
    // you can also use begin(), message() and end() to set sequences
    lead_seqs[2].steplen = 1.0/8.0;
    lead_seqs[2].begin(); 
        for (int i=0; i<4; ++i){
            float trig = (i%2==0) ? 1.0f : 0.75f;
            float pitch = minor_penta[pdsp::dice(8)]; // random pitch from the array, pdsp::dice give you an int
            
            lead_seqs[2].message( double(i),      trig,  0 ); // step, value, output (we set 0 as trig out)
            lead_seqs[2].message( double(i)+0.1f, 0.0f,  0 ); // trigger off, short step gate
            lead_seqs[2].message( double(i),      pitch, 1 ); // step, value, output (we set 1 as value out)
            // step value is double so you can use fractional timing if you want
        }
    lead_seqs[2].end(); // always call end() when you've done

    
    // this is the same as before, but we put the code into a lambda function,
    // the lambda function code is executed each time the sequence is retriggered
    // so each time the sequence is started it generates new patterns
    // read the ofBook about lambdas: https://openframeworks.cc/ofBook/chapters/c++11.html#lambdafunctions
    
    lead_seqs[3].code = [&] () noexcept { // better to tag noexcept for code used by the DSP engine 
            pdsp::Sequence & seq = lead_seqs[3]; // reference
            
            // declaring a variable inside the lambda is fine
            static float akebono[] = { 72.0f, 74.0f, 75.0f, 79.0f, 80.0f, 84.0f, 86.0f, 87.0f }; // akebono scale
            
            seq.steplen = 1.0/8.0;
            seq.begin();
            for (int i=0; i<4; ++i){
                float trig = (i%2==0) ? 1.0f : 0.75f;
                float pitch = akebono[pdsp::dice(8)]; 
                
                seq.message( double(i),       trig,  0 );
                seq.message( double(i)+0.6f,  0.0f,  0 ); // trigger off, half step gate
                seq.message( double(i),       pitch, 1 );
            }
            seq.end();
    };

    // assigning the sequences to the sequencer sections
    for(int i = 0; i<4; ++i){
        // arguments: cell index, pointer to pdsp:Sequence, behavior (pdsp::Behavior::Loop if not given)
        engine.sequencer.sections[0].setCell(i, kick_seqs[i] ); 
        engine.sequencer.sections[1].setCell(i, lead_seqs[i], pdsp::Behavior::OneShot); 
    } 
    
    
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
    for(int i=0; i<(int)engine.sequencer.sections.size(); ++i){
        int active_seq = engine.sequencer.sections[i].meter_current(); 
        if( active_seq < 4){
            float playheadPercent = engine.sequencer.sections[i].meter_playhead() / engine.sequencer.sections[i].meter_length();
            ofDrawRectangle(50*active_seq, 50*i, 50.0f*playheadPercent, 50);
        }
    }
    
    // printing out the instructions
    string info = "master playhead (bars): ";
    info += to_string( engine.sequencer.meter_playhead() );
    info += " \n";
    info += "1-4 : launch kick sequences\n";
    info += "q-w-e-r : launch lead sequences\n";
    info += "a : change lead cell change behavior. Mode = ";
    switch(seq_mode){
        case 0: info += "one shot. \n";
            break;
        case 1: info += "loop. \n";
            break;
    }
    info += "5 : stop kick sequence.\n";
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
    
    // we can launch our sequences with the launch method, with optional quantization
    switch (key){
    case '1':
        engine.sequencer.sections[0].launch( 0, quantize, quantime);
        break;
    case '2':
        engine.sequencer.sections[0].launch( 1, quantize, quantime);
        break;
    case '3':
        engine.sequencer.sections[0].launch( 2, quantize, quantime);
        break;
    case '4':
        engine.sequencer.sections[0].launch( 3, quantize, quantime);
        break;
    case '5':
        engine.sequencer.sections[0].launch( -1, quantize, quantime);
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
        engine.sequencer.sections[1].launch( 0, quantize, quantime);
        break;
    case 'w':
        engine.sequencer.sections[1].launch( 1, quantize, quantime);
        break;
    case 'e':
        engine.sequencer.sections[1].launch( 2, quantize, quantime);
        break;
    case 'r':
        engine.sequencer.sections[1].launch( 3, quantize, quantime);
        break;
    case 't':
        engine.sequencer.sections[1].launch( -1, quantize, quantime);
        break;
   
    case 'a':
        seq_mode = seq_mode ? 0 : 1;
        switch(seq_mode){
            case 0:
                for(int i = 0; i<4; ++i){
                    engine.sequencer.sections[1].oneshot(i);
                } 
                break;
            case 1:
                for(int i = 0; i<4; ++i){
                    engine.sequencer.sections[1].loop(i);
                } 
                break;
        }
        break;
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
