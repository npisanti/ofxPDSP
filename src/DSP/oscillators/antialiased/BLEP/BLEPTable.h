
// BLEPTable.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_OSC_BLEPTABLE_H_INCLUDED
#define PDSP_OSC_BLEPTABLE_H_INCLUDED

#include "../../../pdspCore.h"

namespace pdsp {
    
/*!
    @cond HIDDEN_SYMBOLS
*/
    
    
    class BLEPTable{
    public:
        BLEPTable();
        BLEPTable(const BLEPTable& other);
        BLEPTable& operator= (const BLEPTable& other);
        ~BLEPTable();
        
        float* buffer;
        int length;
        float length_f;
        bool eight;
        float points;
        Window_t type;
        
        void setTable(Window_t type, bool eight, int length);
        bool empty();
    };
    
/*!
    @endcond
*/
        
    
}



#endif  // PDSP_OSC_BLEPTABLE_H_INCLUDED
