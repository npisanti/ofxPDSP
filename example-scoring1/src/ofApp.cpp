#include "ofApp.h"

// before looking at this check out the basics examples

// example for the pdsp::SeqChange class

//--------------------------------------------------------------
void ofApp::setup(){
    
    //-------------------GRAPHIC SETUP--------------
    ofSetWindowShape(640, 360);
    
    //--------PATCHING-------
    seq_mode = 0;
    quantize = true;
    quantime = 1.0/8.0; // 1/4th
    
    engine.score.setTempo(120.0f); // sets the tempo of the music
    
    engine.score.sections.resize(2); 
    // this is very important, each section can contain and switch between one or more sequences
    // you have to resize it to a sufficient number before starting to assign sequences
 
    // each section can output the messages to one or more outputs, as values or triggers
    engine.score.sections[0].out_trig(0)  >> bass.in("trig"); // assign the first sequence output to trig
    engine.score.sections[0].out_value(1) >> bass.in("pitch"); // assign the second sequence output to values
    engine.score.sections[1].out_trig(0)  >> lead.in("trig"); // assign the first sequence output to trig
    engine.score.sections[1].out_value(1) >> lead.in("pitch"); // assign the second sequence output to values
    
    // decomment this to slew the lead value output signal
    // engine.score.sections[1].out_value(1).enableSmoothing(100.0f);
    
    // patching
    bass * (dB(-6.0f) * panL(-0.25f)) >> engine.audio_out(0);
    bass * (dB(-6.0f) * panR(-0.25f)) >> engine.audio_out(1);
    
    lead * (dB(-6.0f) * panL(0.25f)) >> engine.audio_out(0);
    lead * (dB(-6.0f) * panR(0.25f)) >> engine.audio_out(1);


    // writing our sequences
    bass_seqs.resize(4); 
    lead_seqs.resize(4);
    
    
    // lead sequences ( and sequences examples ) 
    lead_seqs[0].set( { { 1.0f,  0.75f, 1.0f,  0.75f }, // to the first output ( triggers )
                        { 84.0f, 82.0f, 84.0f, 80.0f } }, // to the second output ( pitch values )
                          8.0, 1.0 ); // time division (1/8th), sequence length in bars (1 bar)
  
    float o = -1.0f; // when using "set" negative outputs are ignored, 'o' is graphically useful
    lead_seqs[1].set( { { 1.0f,  o,  1.0f,  0.75f},
                        { 72.0f, o,  75.0f, 77.0f} }, 8.0, 1.0 );
     
     
    
    float minor_penta[] = { 72.0f, 75.0f, 77.0f, 79.0f, 82.0f, 84.0f, 87.0f, 89.0f }; // minor pentatonic scale
    // you can also use begin(), message() and end() to set sequences
    // we will create a sequence that is different for each program execution
    lead_seqs[2].begin( 8.0, 1.0 ); // division, sequence length
    for (int i=0; i<4; ++i){
        float trig = (i%2==0) ? 1.0f : 0.75f;
        float pitch = minor_penta[pdspDice(8)]; // random pitch from the array, pdspDice give you an int
        lead_seqs[2].message( double(i), trig,  0 ); // step, value, output (we set 0 as trig out)
        lead_seqs[2].message( double(i), pitch, 1 ); // step, value, output (we set 1 as value out)
    }
    lead_seqs[2].end(); // always call end() when you've done

    
    // this is the same as before, but we put the code into a lambda function
    // the lambda function code is executed each time the sequence is retriggered
    // DO NOT USE VARIABLES LOCAL TO THE setup() INSIDE THIS FUNCTION
    // the [&] means we take reference to the variables where the lambda function is defined 
    // and the life of the variable local to the setup() fuction ends at the end of the setup
    // use variables you have declared in the .h file or internally to the lambda
    lead_seqs[3].code = [&] () noexcept { // better to tag noexcept for code used by the DSP engine 
        // declaring a variable inside the lambda is fine
        static float akebono[] = { 72.0f, 74.0f, 75.0f, 79.0f, 80.0f,  84.0f, 86.0f, 87.0f }; // akebono scale
        lead_seqs[3].begin( 8.0, 1.0 );
        for (int i=0; i<4; ++i){
            float trig = (i%2==0) ? 1.0f : 0.75f;
            float pitch = akebono[pdspDice(8)]; 
            lead_seqs[3].message( double(i), trig,  0 );
            lead_seqs[3].message( double(i), pitch, 1 );
        }
        lead_seqs[3].end();
    };
    // the sequence code is executed in the DSP thread, so please don't hold lock or allocate / deallocate memory
    // if you need some heavy computation make it in another thread and copy it to the sequence when it's time
    // as the code is executed into the DSP thread is better not to use ofRandom(), that is not thread safe
    // use pdspDice(), pdspChance(), pdspURan() and pdspBRan() istead 
    // they also aren't thread safe if you use them in more than one thread, but as long you use
    // ofRandom() in oF and those functions in pdsp you're fine
    // more info on those functions:
    // http://npisanti.com/ofxPDSP/classofx_p_d_s_p_functions.html
 
    // bass sequences
    bass_seqs[0].set( { {1.0f, o}, {36.0f, o} }, 16.0, (1.0/8.0) ); // we need at least 2 values for each subarray, otherwise won't compile
    bass_seqs[1].set( { {1.0f, o}, {38.0f, o} }, 16.0, (1.0/8.0) );
    bass_seqs[2].set( { {1.0f, o}, {39.0f, o} }, 16.0, (1.0/8.0) );
    bass_seqs[3].set( { { 1.0f,  1.0f}, { 41.0f, 42.0f} }, 16.0, (1.0/8.0) );
    
    
    // assigning the sequences to the sections
    for(int i = 0; i<4; ++i){
        engine.score.sections[0].setCell(i, &bass_seqs[i], &alternate); // cell index, pointer to pdsp:Sequence, pointer to pdsp::SeqChange
        engine.score.sections[1].setCell(i, &lead_seqs[i], pdsp::Behavior::OneShot); 
    } 

    // pdsp::SeqChange is a class that choose another sequence when a sequence ends it's length
    // there are some standard SeqChange to use in pdsp::Behavior
    // you will mostly use pdsp::Behavior::Loop that will loop the sequence or
    // pdsp::Behavior::OneShot that won't sequence anything after the sequence's end


    // this is a custom pdsp::SeqChange for our bass patterns, it works the same as assigning code to pdsp::Sequence
    // it alternates between the first sequence and the other three
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
