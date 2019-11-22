
// OscParser.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2019

#ifndef OFXPDSPMIDI_PDSPOSCPARSER_H_INCLUDED
#define OFXPDSPMIDI_PDSPOSCPARSER_H_INCLUDED

#include "ofMain.h"
#include <chrono>
#include <algorithm>
#include <atomic>
#include "../DSP/pdspCore.h"
#include "../sequencer/SequencerSection.h"
#include "ofxOsc.h"
#include "helper/OscParser.h"

namespace pdsp{ namespace osc {

static const float Ignore = std::numeric_limits<float>::infinity(); 

class OscParser {

private:
    class OscChannel {
    public:
        OscChannel();
        
        void deallocate();
        
        pdsp::MessageBuffer* messageBuffer;
        pdsp::SequencerGateOutput* gate_out;
        pdsp::SequencerValueOutput* value_out;
        
        bool hasParser;
        std::function<float(float)> code;
        
        float initValue;
    };
    
public:
    OscParser();
    ~OscParser();
    
    std::string address;
        
        
    pdsp::SequencerGateOutput& out_trig( int out );  
    pdsp::SequencerValueOutput& out_value(  int out );
    void initTo( int argument, float value );

    void process( ofxOscMessage msg, int sample );
    void processDestinations( int bufferSize );

    std::function<float(float)> & parser( int argument );
    
    void clear( bool sendClearMessage );
    
private:
    std::vector<OscChannel*> channels;

    static pdsp::SequencerGateOutput invalidGate;
    static pdsp::SequencerValueOutput invalidValue;
    
};

}}

#endif //OFXPDSPMIDI_PDSPOSCPARSER_H_INCLUDED
