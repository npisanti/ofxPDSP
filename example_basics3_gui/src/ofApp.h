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
    pdsp::Engine    engine;
    
    pdsp::FMOperator        osc1;
    pdsp::FMOperator        osc2;
    pdsp::FMOperator        osc3;

    pdsp::Parameter      osc1_pitch_ctrl;
    pdsp::Parameter      osc2_pitch_ctrl;
    pdsp::Parameter      osc3_pitch_ctrl;
    
    pdsp::ParameterAmp   osc1_amp;
    
    pdsp::ParameterGain  osc2_gain;
    pdsp::ParameterGain  osc3_gain;
    
    ofParameterGroup     osc1_group;
    ofParameterGroup     osc2_group;
    ofParameterGroup     osc3_group;
    
    ofxPanel             gui;

};
