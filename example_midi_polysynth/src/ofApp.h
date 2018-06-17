#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxPDSP.h"
#include "ofxGui.h"
#include "synth.h"

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
        
        pdsp::Engine   engine;

        int                     voicesNum;

        PolySynth               synth;
     
        pdsp::DimensionChorus   chorus;       
        
        pdsp::midi::Input       midiIn;
        pdsp::midi::Keys        midiKeys;
        pdsp::midi::Controls    midiCCs;

        ofxPanel             gui;
        
        ofParameterGroup     chorusUI;

        pdsp::Parameter chorusSpeed;
        pdsp::Parameter chorusDepth;
        
        pdsp::ParameterGain  gain;
    
};
