#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"

// Zap class
class Zap : public pdsp::Patchable{ 
public:
        Zap(){ patch(); }
        Zap(const Zap & other){ patch(); }

        void patch() {
                addModuleInput("trig", triggers); 
                addModuleInput("pitch", osc.in_pitch()); 
                addModuleOutput("signal", amp ); 
                //patching
                                                            osc >> amp;
                triggers >> aEnv.set(0.0f, 60.0f, 10.0f) * 0.5f  >> amp.in_mod();
                triggers >> pEnv.set(0.0f, 0.0f,  60.0f) * 48.0f >> osc.in_pitch();
                                                           36.0f >> osc.in_pitch();
        }

        float meter_env() const{
            return aEnv.meter_output();
        }

private:
        pdsp::PatchNode     triggers;
        pdsp::FMOperator    osc;
        pdsp::AHR           aEnv;
        pdsp::AHR           pEnv;
        pdsp::Amp           amp;
};


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

        void newGeneration() noexcept;
        void wolframToPatterns() noexcept;
        void initFirst() noexcept;
        void initRandom() noexcept;
        void switchRule();
        void initRule() noexcept;
        void processSequence() noexcept;

        // pdsp modules
        ofxPDSPEngine   engine; 
             
        pdsp::Sequence wolframSequence;
                
        vector<Zap> zaps;

        int rule;
        vector<int> wrules;
        
        vector<vector<int>> wolframCA;
        int caGenerationIndex;
        vector<float> bars;
        
        float triggerScaling;
        
        int offsetX;
        
        atomic<bool> initFirstFlag;
        atomic<bool> initRandomFlag;
        atomic<bool> ruleChanged;
        
        int blink;
        
};



