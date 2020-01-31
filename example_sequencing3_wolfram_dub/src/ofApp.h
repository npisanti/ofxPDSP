#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"
#include "WolframSeq.h"
#include "ofxGui.h"
#include "Zaps.h"
#include "StereoDelay.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

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

        void switchRule();

        // pdsp modules
        pdsp::Engine   engine; 
             
        WolframSeq wolframSeq;
        
        Zaps zaps;

        pdsp::IRVerb reverb;
        
        pdsp::ParameterGain dubSwitch;

        StereoDelay dub;

        pdsp::Sequence     masterplan;
        pdsp::LowCut       revCut;

        ofxPanel gui;
        
        int caHeight;

        ofColor brightColor;
        ofColor darkColor;
        
        std::vector<pdsp::Scope> scopes;
        
};



