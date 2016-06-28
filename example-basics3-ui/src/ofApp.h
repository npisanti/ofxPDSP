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
    ofxPDSPEngine           engine;
    
    pdsp::FMOperator        osc1;
    pdsp::FMOperator        osc2;
    pdsp::FMOperator        osc3;
    pdsp::Amp               amp1;
    pdsp::Amp               amp2;
    pdsp::Amp               amp3;


    ofxPDSPValue            osc1_pitch_ctrl;
    ofxPDSPValue            osc2_pitch_ctrl;
    ofxPDSPValue            osc3_pitch_ctrl;
    ofxPDSPValue            osc1_amp_ctrl;
    ofxPDSPValue            osc2_amp_ctrl;
    ofxPDSPValue            osc3_amp_ctrl;
    
    ofParameterGroup     osc1_group;
    ofParameterGroup     osc2_group;
    ofParameterGroup     osc3_group;
    
    ofxPanel             gui;

};
