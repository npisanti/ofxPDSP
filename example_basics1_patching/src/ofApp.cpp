#include "ofApp.h"

// This is the most basic pdsp example
// we set up everything and check that everything is working
// before running this also check that the basic oF audio output example is working
// ofxx_folder/examples/sound/audioOutputExample/

// for documentation of the modules and functions:
// http://npisanti.com/ofxPDSP/md__modules.html

//--------------------------------------------------------------
void ofApp::setup(){
    
    //-------------------GRAPHIC SETUP--------------
    ofBackground(0);
    ofSetFrameRate(30);
    
    //-------------------------PATCHING--------------

    // you connect modules ins/outs with the patch operator >>
    // the * operator lets you scales the signals by a float
    // the dB() function returns a float for setting the volume
    osc.out_sine() * dB(-12.0f) >> engine.audio_out(0); // connect to left output channel
    osc.out_sine() * dB(-12.0f) >> engine.audio_out(1); // connect to right right channel

    // pdsp::VAOscillator is an antialiased virtual analog oscillator
    // pdsp::LFO is a Low Frequency Oscillator, for cyclic modulations


    // you can patch a float value to an input, only the last one patched is used
    // (every inputs or module has some default values that are used when nothing is patched, oscs play A4=440hz)
    // 72.0f >> osc.in_pitch();
    // 84.0f >> osc.in_pitch(); // higher


    // decomment also these two lines to add a cyclic modulation to the oscillator pitch
    // pdsp modules mostly generate values in the -1.0f <----> 1.0f range 
    //0.5f >> lfo;
    //        lfo * 48.0f >> osc;
    
    
    // when an output or an input ar not selected default ones are used
    // the lines above are the same of 
    // 0.5f >> lfo.in_freq();
    //         lfo.out_triangle() * 48.0f >> osc.in_pitch();

    
    // only the last value is used in scaling, so
    // lfo * 48.0f >> osc;          // correct
    // lfo * 2.0f * 24.0f >> osc;   // WRONG, scales to 24.0f
    // lfo * (2.0f * 24.0f) >> osc; // correct


    cout<<"[example] finished patching\n";
    //----------------------AUDIO SETUP-------------
    
    // set up the audio output device
    engine.listDevices();
    engine.setDeviceID(0); // REMEMBER TO SET THIS AT THE RIGHT INDEX!!!!

    // start your audio engine !
    engine.setup( 44100, 512, 3); 
    // arguments are : sample rate, buffer size, and how many buffer there are in the audio callback queue
    // 512 is the minimum buffer size for the raspberry Pi to work
    // 3 buffers queue is the minimum for the rPi to work
    // if you are using JACK you have to set this number to the bufferSize you set in JACK
    // on Windows you have to set the sample rate to the system sample rate, usually 44100hz
    // on iOS sometimes the systems forces the sample rate to 48khz, so if you have problems set 48000

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
