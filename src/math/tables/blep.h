
// blep.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_MATH_BLEP_H_INCLUDED
#define PDSP_MATH_BLEP_H_INCLUDED

#include "dsp_windows.h"
//#include "../../../../JuceLibraryCode/JuceHeader.h"

namespace pdsp{
    
    
    
    float* blepTable2( const Window_t windowType, const int len);
    float* blepTable8( const Window_t windowType, const int len);
    
}




#endif  // PDSP_MATH_BLEP_H_INCLUDED
