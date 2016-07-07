#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"
#include "ofxGui.h"

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
        
        // pdsp modules
        ofxPDSPEngine   engine;
        
        pdsp::Compressor1       compressor;
        
        pdsp::Amp               channelGain;
        pdsp::Amp               delaySend;
        pdsp::DampedDelay       delayL;
        pdsp::DampedDelay       delayR;
        pdsp::PatchNode         fb_node;
        pdsp::PatchNode         damp_node;
        pdsp::DBtoLin           channelFader;
        pdsp::DBtoLin           sendFader;        

        ofxPanel             gui;
        ofParameterGroup     comp_gui;
        ofParameterGroup     delay_gui;

        ofxPDSPValue     d1_time_ctrl; 
        ofxPDSPValue     d2_time_ctrl; 
        ofxPDSPValue     d_fb_ctrl;         
        ofxPDSPValue     d_damp_ctrl;     
        ofxPDSPValue     gain_ctrl;         
        ofxPDSPValue     send_ctrl;         

        ofxPDSPValue        comp_threshold_ctrl;
        ofxPDSPValue        comp_attack_ctrl;
        ofxPDSPValue        comp_release_ctrl;
        ofParameter<float>  comp_meter;
        

};
