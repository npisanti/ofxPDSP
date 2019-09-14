#include "ofApp.h"

// before looking at this check out the basics examples

// pdsp sequencing 
//
// PS: the sequence code is executed in the DSP thread, so please don't hold lock or allocate / deallocate memory
// if you need some heavy computation make it in another thread and copy it to the sequence when it's time
// as the code is executed into the DSP thread is better not to use ofRandom(), it is not thread safe
// use pdsp::dice(), pdsp::chance(), pdsp::uran() and pdsp::bran() instead (only in the sequence functions!) 


//--------------------------------------------------------------
void ofApp::setup(){
 
    seq_mode = 0;
    quantize = true;
    quantime = 1.0/8.0; // 1/4th


    // ------------------------ SEQUENCING ---------------------------
    
    // set up the sequencer 
    // sections, sequences, tempo
    engine.sequencer.init( 2, 4, 120.0f);
    int kicksect = 0;
    int leadsect = 1;
    

    // KICK SEQUENCES 
    
    auto & kick0 = engine.sequencer.sections[kicksect].sequence(0);
    
    kick0.bars = 1.0; // this is the sequence loop lenght
    kick0.begin(); // use begin() and end() to set the messages
        kick0.bang( 1.0f ); // bang sends a message
        //kick_seqs[0].message( 0.0, 1.0f, 0 );
    kick0.end();

    auto & kick1 = engine.sequencer.sections[kicksect].sequence(1);
    kick1.bars = 1.0;     
    kick1.begin(); 
        // 4 steps, delay sets the offset from start (in bars)
        kick1.delay( 0.0 ).bang(1.0f);
        kick1.delay( 1.0/4.0 );
        kick1.bang( 0.5f );
        kick1.delay( 2.0f / 4.0 ).bang(0.8f);
        kick1.delay( 3.0f / 4.0 ).bang(0.6f);
    kick1.end();


    auto & kick2 = engine.sequencer.sections[kicksect].sequence(2);
    // bars defaults to 1.0, can be omitted for 1 bars loops
    kick2.begin(); // another seq
        kick2.bang(1.0f);
        kick2.delay( 1.0f / 8.0f ).bang(0.8f);
        kick2.delay( 4.0f / 8.0f ).bang(0.6f);
    kick2.end();
    
    
    auto & kick3 = engine.sequencer.sections[kicksect].sequence(3);
    kick3.begin(); // many kicks
        kick3.bang(1.0f);
        for( int i=1; i<15; ++i ){
            kick3.delay( i/16.0 ).bang(0.5f);
        }
    kick3.end();    
        
    
    // LEAD SEQUENCES
    
    // we need to control gate and pitch of leads
    // we can use out() to change the output for the messages 
    
    auto & lead0 = engine.sequencer.sections[leadsect].sequence(0);
    lead0.begin();
        lead0.out(0).bang(1.0);
        lead0.out(1).bang(84.f);
        
        lead0.delay( 2.0 / 16.0 );
        lead0.out(0).bang( 0.75f );
        lead0.out(1).bang( 82.f) ;
        
        lead0.delay( 3.0 / 16.0 ).out(0).bang(0.0f); //gate off
        
        lead0.delay( 4.0 / 16.0 );
        lead0.out(0).bang( 1.0f );
        lead0.out(1).bang( 84.f );
        
        lead0.delay( 5.0 / 16.0 ).out(0).bang(0.0f); 
        
        lead0.delay( 6.0 / 16.0 );
        lead0.out(0).bang( 0.75f );
        lead0.out(1).bang( 80.f ); 
        
        lead0.delay( 7.0 / 16.0 ).out(0).bang(0.0f); 
    lead0.end();
    
    
    auto & lead1 = engine.sequencer.sections[leadsect].sequence(1);
    lead1.begin();
        lead1.out(0).bang(1.0);
        lead1.out(1).bang(72.f);
        
        // just to pitch
        lead1.delay( 4.0 / 16.0 ).out(1).bang( 75.f );
        lead1.delay( 6.0 / 16.0 ).out(1).bang( 77.f );
        
        // gate off
        lead1.delay( 7.0 / 16.0 ).out(0).bang( 0.f ); 
    lead1.end();        


    auto & lead2 = engine.sequencer.sections[leadsect].sequence(2);
    
    float minor_penta[] = { 72.0f, 75.0f, 77.0f, 79.0f, 82.0f, 84.0f, 87.0f, 89.0f }; // minor pentatonic scale
    
    lead2.begin(); 
        for (int i=0; i<4; ++i){
            float trig = (i%2==0) ? 1.0f : 0.75f;
            float pitch = minor_penta[pdsp::dice(8)]; // random pitch from the array, pdsp::dice give you an int
            
            lead2.out(0).delay( i/8.0 ).bang( trig );
            lead2.out(0).delay( (i+0.1)/8.0 ).bang( 0.0f );
            
            lead2.out(1).delay( i/8.0 ).bang( pitch );
        }
    lead2.end();

    
    // this is the same as before, but we put the code into a lambda function,
    // the lambda function code is executed each time the sequence is retriggered
    // so each time the sequence is started it generates new patterns
    // read the ofBook about lambdas: https://openframeworks.cc/ofBook/chapters/c++11.html#lambdafunctions
    auto & lead3 = engine.sequencer.sections[leadsect].sequence(3);
    lead3.bars = 0.5; // half bar loop
    lead3.code = [&] () noexcept { // better to tag noexcept for code used by the DSP engine 
            // declaring a variable inside the lambda is fine
            static float akebono[] = { 72.0f, 74.0f, 75.0f, 79.0f, 80.0f, 84.0f, 86.0f, 87.0f }; // akebono scale
            
            lead3.begin();
            for (int i=0; i<4; ++i){
                float trig = (i%2==0) ? 1.0f : 0.75f;
                float pitch = akebono[pdsp::dice(8)]; 
                
                lead3.message( double(i),       trig,  0 );
                lead3.message( double(i)+0.6f,  0.0f,  0 ); // trigger off, half step gate
                lead3.message( double(i),       pitch, 1 );
                

                lead3.out(0).delay( i/8.0 ).bang( trig );
                lead3.out(0).delay( (i+0.6)/8.0 ).bang( 0.0f );
                
                lead3.out(1).delay( i/8.0 ).bang( pitch );
            }
            lead3.end();
    };
    
    // MEMO: you can concatenay how many out(), delay() and bang()
    // you want, just remember that bang() will send a message
    // with the last given value of out() and delay()
    // out() and delay() are reset to 0 by seq.begin()
    

    for(int i = 0; i<4; ++i){
        // the section can play the sequenced looped or not
        // you can change this behavior with loop() or oneshot() 
        // loop is default 
        engine.sequencer.sections[1].oneshot( i ); 
    } 
    
    //----------------- PATCH THE SEQUENCES --------------------
    
    // each section can output the messages to one or more outputs, as values or triggers
    // thing about them as gate/cv outputs, gate=trig, value=cv
    engine.sequencer.sections[kicksect].out_trig(0) >> kick.in("trig");
    
    // lead seq out 0 to trig
    engine.sequencer.sections[leadsect].out_trig(0) >> lead.in("trig");
    // lead seq out 1 to pitch 
    engine.sequencer.sections[leadsect].out_value(1) >> lead.in("pitch"); 
    
    // patching (with panning)
    kick * (dB(-6.0f) * pdsp::panL(-0.25f)) >> engine.audio_out(0);
    kick * (dB(-6.0f) * pdsp::panR(-0.25f)) >> engine.audio_out(1);
    
    lead * (dB(-6.0f) * pdsp::panL(0.25f)) >> engine.audio_out(0);
    lead * (dB(-6.0f) * pdsp::panR(0.25f)) >> engine.audio_out(1);
    
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
