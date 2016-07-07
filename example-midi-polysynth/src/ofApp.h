#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxPDSP.h"
#include "ofxGui.h"
#include "voice.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        
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
        
        ofxPDSPEngine   engine;

        int                     voicesNum;
        vector<SynthVoice>      voices;
        SynthUI                 synthUI;
     
        pdsp::DimensionChorus   chorus;       
        
        ofxPDSPMidiIn        midiIn;
        ofxPDSPMidiKeys      midiKeys;

        ofxPanel             gui;
        
        ofParameterGroup     chorusUI;

        ofxPDSPValue     chorusSpeed;
        ofxPDSPValue     chorusDepth;
    
};
