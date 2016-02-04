
// BLEPBased.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_OSC_BLEPBASED_H_INCLUDED
#define PDSP_OSC_BLEPBASED_H_INCLUDED

#include "BLEPTable.h"
#include "BLEPFunc.h"

namespace pdsp{
/*!
    @cond HIDDEN_SYMBOLS
*/
    
    class BLEPBased{
   
    public:
        void setTable(Window_t type, bool eight, int length, bool interpolate);
        
    protected:
        static std::vector<BLEPTable> tables;
        
        BLEPTable* getTable(Window_t type, bool eight, int length);
        static void addTable(Window_t type, bool eight, int length);
        BLEPTable* getTable(int index);
        const int getAvailableTablesNumber() const;
        
        BLEPTable* blepTable;
        bool interpolateBLEP;
    };

/*!
    @endcond
*/
    
}


#endif  // PDSP_OSC_BLEPBASED_H_INCLUDED
