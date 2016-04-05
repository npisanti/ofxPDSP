#pragma once

#include "ofxiOS.h"
#include "ofxPDSP.h"

class ofApp : public ofxiOSApp {
	
    public:
        void setup();
        void update();
        void draw();
	
        void touchDown(ofTouchEventArgs & touch);
        void touchMoved(ofTouchEventArgs & touch);
        void touchUp(ofTouchEventArgs & touch);
        void touchDoubleTap(ofTouchEventArgs & touch);
        void touchCancelled(ofTouchEventArgs & touch);

        void lostFocus();
        void gotFocus();
        void gotMemoryWarning();
        void deviceOrientationChanged(int newOrientation);
    
        ofxPDSPEngine           engine;
    
        pdsp::VAOscillator      osc;
        pdsp::Amp               amp;
};


