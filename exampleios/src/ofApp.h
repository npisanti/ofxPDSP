#pragma once

#include "ofxiOS.h"
#include "ofxPDSP.h"

class ofApp : public ofxiOSApp {
	
    public:
        void setup();
        void update();
        void draw();
        void exit();
    
        void audioOut(float * output, int bufferSize, int nChannels);
	
        void touchDown(ofTouchEventArgs & touch);
        void touchMoved(ofTouchEventArgs & touch);
        void touchUp(ofTouchEventArgs & touch);
        void touchDoubleTap(ofTouchEventArgs & touch);
        void touchCancelled(ofTouchEventArgs & touch);

        void lostFocus();
        void gotFocus();
        void gotMemoryWarning();
        void deviceOrientationChanged(int newOrientation);
 
        int sampleRate;
        int expectedBufferSize;
    
        pdsp::Processor         pdspEngine;
    
        pdsp::VAOscillator      osc;
        pdsp::Amp               amp;
};


