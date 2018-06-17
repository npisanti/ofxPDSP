
#pragma once

#include "ofxPDSP.h"

// to create your own modules, you have to extend pdsp::Patchable

class SlideSynth : public pdsp::Patchable{
    
public:
    
    SlideSynth() { patch(); } // default constructor
    SlideSynth( const SlideSynth & other ) { patch(); } // you need this to use std::vector with your class, otherwise will not compile
    // remember that is a bad thing to copy construct in pdsp, 
    //      always just resize the vector and let the default constructor do the work
    //          resizing the vector will also disconnect everything, so do it just once before patching


    void patch (){
       
        //create inputs/outputs to be used with the in("tag") and out("tag") methods
        addModuleInput("pitch", osc.in_pitch()); // the first input added is the default input
        addModuleInput("amp", y_ctrl);
        addModuleOutput("signal", amp ); // the first output added is the default output
        
        
        // pdsp::PatchNode is a class that simply route its inputs to its output
        y_ctrl.enableBoundaries(0.0f, 1.0f); // you can clip the input of pdsp::PatchNode
        y_ctrl.set(0.0f); // and you can set its input default value
        
        //patching
        osc.out_saw() * 2.0f >> drive >> filter >> amp;
                                         y_ctrl >> amp.in_mod();        
                                         y_ctrl * 60.0f >> filter.in_cutoff();
                                                  48.0f >> filter.in_cutoff();
                                                  0.3f  >> filter.in_reso();
    }
    
    // those are optional
    pdsp::Patchable & in_pitch() {
        return in("pitch");
    }
    
    pdsp::Patchable & in_amp() {
        return in("amp");
    }
    
    pdsp::Patchable & out_signal() {
        return out("signal");
    }
    
private:

    pdsp::PatchNode     y_ctrl;
    pdsp::PatchNode     pitch_ctrl;
    pdsp::Amp           amp;
    pdsp::VAOscillator  osc;
    pdsp::Saturator1    drive; // distort the signal
    pdsp::VAFilter      filter; // 24dB multimode filter

};
