#pragma once

#include "ofMain.h"
#include "ofxSIMDFloats.h"
#include "ofxPDSP.h"
#include "score.h"
#include "synths.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        
		void audioOut(ofSoundBuffer &outBuffer);
        
        void exit();

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
        
        void visualizeSection(const pdsp::ScoreSection & sect, int numCells, int x, int y, int w, int h); 
        
        int             expectedBufferSize;
        double          sampleRate;		
        ofSoundStream   audioStream;
       
        pdsp::Processor pdspEngine;
        
        MusicTest       music;
        
        SineBleep       bleep;
        BassSynth       bass;
        
        ofFbo           bassScoreCanvas;
        int             bassCanvasW;
        int             bassCanvasH;
};
