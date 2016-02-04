
// MaxValue.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_UTILITY_MAXVALUE_H_INCLUDED
#define PDSP_UTILITY_MAXVALUE_H_INCLUDED

#include "../pdspCore.h"

namespace pdsp{

/** 
@brief Output is the max between the values of the two inputs
*/    

class MaxValue2 : public Unit{
    
public: 
    MaxValue2();
    
    Patchable& in_0();
    Patchable& in_1();
    
    Patchable& out_signal();
    
private: 
    void prepareUnit( int expectedBufferSize, double sampleRate ) override;
    void releaseResources () override ;
    void process (int bufferSize) noexcept override;


    InputNode   input1;
    InputNode   input2;
    OutputNode  output;
};
    
}// pdsp namespace end


#endif // PDSP_UTILITY_MAXVALUE_H_INCLUDED
