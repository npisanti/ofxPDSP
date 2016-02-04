
// GrainTable.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_GRAIN_GRAINTABLE_H_INCLUDED
#define PDSP_GRAIN_GRAINTABLE_H_INCLUDED
#include "../../pdspCore.h"


namespace pdsp {
/*!
    @cond HIDDEN_SYMBOLS
*/
    class GrainTable{
    public:
        GrainTable();
        GrainTable(const GrainTable& other);
        GrainTable& operator= (const GrainTable& other);
        ~GrainTable();
        
        float* buffer;
        int length;
        float length_f;
        Window_t type;
        
        void setTable(Window_t type, int length);
        bool empty();
    };
/*!
    @endcond
*/
}


#endif  // PDSP_GRAIN_GRAINTABLE_H_INCLUDED
