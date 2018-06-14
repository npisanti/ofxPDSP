#include "ofApp.h"

// This is the most basic pdsp example
// we set up everything and check that everything is working
// before running this also check that the basic oF audio output example is working
// ofxx_folder/examples/sound/audioOutputExample/


//--------------------------------------------------------------
void ofApp::setup(){
    
    //-------------------GRAPHIC SETUP--------------
    ofBackground(0);
    ofSetFrameRate(30);
    
    //-------------------------PATCHING--------------
    // the basic operation of pdsp is the patching 
    // it is made with the patch operator >>
    // each module has some inputs and outputs you access with in("tag"), in_tag(), out("tag") and out_tag()
    // ofxPDSPEngine is always the last unit in the chain, it also manages the ofSoundStreams
    // and let you patch your modules to the system audio output
    
    // an oscillator with nothing connected to in_pitch() will play A4 = 440hz
    // (every unit or module has some default values that are used when nothing is patched)
    // the * operator scales the output signal
    // the dB() function returns a float for the linear equivalent of the dB value, use it set volume
    osc.out_sine() * dB(-12.0f) >> engine.audio_out(0); // connect to left output channel
    osc.out_sine() * dB(-12.0f) >> engine.audio_out(1); // connect to right right channel
    // ( a pdsp::VAOscillator produces a cyclic waveform in the audio range, with a control in semitones )

    // you can patch a float value to an input, only the last one patched is used
    // 72.0f >> osc.in_pitch();
    // 84.0f >> osc.in_pitch(); // higher


/*
    // decomment also this to add a cyclic modulation to the oscillator pitch
    // do not forgate to decomment the float values above, or the range of the oscillator will just go from 48.0f to 48.0f (semitones) 
    0.5f >> lfo;
            lfo * 48.0f >> osc;
    // ( a pdsp::LFO is a Low Frequency Oscillator module )
*/

    // when an output or an input ar not selected before patching the default one are used
    // the lines above are the same of 
    // 0.5f >> lfo.in_freq();
    //         lfo.out_triangle() * 48.0f >> osc.in_pitch();


/*
    // you can also scale the audio by dB values using the dB() function, multiplying again will override the last value
    osc.out_sine() * dB(-12.0f) >> engine.audio_out(0); // connect to left output channel
    osc.out_sine() * dB(-12.0f) >> engine.audio_out(1); // connect to right right channel  
*/

/*
    // and you can pan with the panL() and panR() functions, you have to pass the same value to both, from -1.0f to 1.0f
    // you can combine pdsp::pan with dB those function together by multiplying their results before patching
    float pan = 0.5f;
    osc.out_sine() * (dB(-12.0f) * pdsp::panL(pan)) >> engine.audio_out(0); // connect to left output channel
    osc.out_sine() * (dB(-12.0f) * pdsp::panR(pan)) >> engine.audio_out(1); // connect to right right channel  
*/

    cout<<"[example] finished patching\n";
    //----------------------AUDIO SETUP-------------
    
    // set up the audio output device
    engine.listDevices();
    engine.setDeviceID(0); // REMEMBER TO SET THIS AT THE RIGHT INDEX!!!!

    // start your audio engine !
    engine.setup( 44100, 512, 3); 
    // arguments are : sample rate, buffer size, and how many buffer there are in the audio callback queue
    // 512 is the minimum buffer size for the raspberry Pi to work
    // buffer size is better to be a power of 2, typical buffer sizes are: 128, 256, 512, 1024
    // you need at least 2 buffer queue
    // 3 buffers queue is the minimum for the rPi to work
    // if you are using JACK you have to set this number to the bufferSize you set in JACK
    // on Windows you have to set the sample rate to the system sample rate, usually 44100hz

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
