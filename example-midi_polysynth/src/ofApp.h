#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxPDSP.h"
#include "voice.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        
        void exit();
		void audioOut(ofSoundBuffer &outBuffer);
        
        void drawMeter(float value, float min, float max, int x, int y, int w, int h);
        
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        
        int     bufferSize;
        double  sampleRate;

        ofxMidiIn           midiIn;
        ofSoundStream       audioStream;

        int                     voicesNum;
        vector<SynthVoice>      voices;
     
        pdsp::Processor         pdspEngine;
        pdsp::DimensionChorus   chorus;       
            
        ofxMidiInProcessor      midiInProcessor;    
        ofxMidiKeysProcessor    midiKeys;

        ofxPanel             gui;
        
        ofParameterGroup     chorusUI;
        ofxPDSPParameter     chorusSpeed; 
        ofxPDSPParameter     chorusDepth; 

};
