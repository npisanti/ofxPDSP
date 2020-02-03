#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"
#include "synths.h"

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
        void circularWrite();

        // pdsp modules
        pdsp::Engine            engine;
        SineBleep               lead;
        pdsp::Function          seq;

        std::atomic<int> index;
        std::vector<std::vector<float>> buffer;
        
        std::atomic<float> probability;
        

};
