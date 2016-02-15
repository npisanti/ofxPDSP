
// MaxValue.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_UTILITY_ONEBARTIMEMS_H_INCLUDED
#define PDSP_UTILITY_ONEBARTIMEMS_H_INCLUDED

#include "../pdspCore.h"

namespace pdsp{

/** 
@brief Constantly outputs a control rate value that rapresent the time in milliseconds for one bar.
*/    

class OneBarTimeMs : public Unit{
    
public: 
    OneBarTimeMs();
    
    Patchable& out_signal();
    
private: 
    void prepareUnit( int expectedBufferSize, double sampleRate ) override;
    void releaseResources () override ;
    void process (int bufferSize) noexcept override;

    OutputNode  output;
};
    
}// pdsp namespace end


#endif // PDSP_UTILITY_ONEBARTIMEMS_H_INCLUDED
