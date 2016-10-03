
// GrainBased.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_GRAIN_GRAINBASED_H_INCLUDED
#define PDSP_GRAIN_GRAINBASED_H_INCLUDED

#include "GrainTable.h"
#include <vector>

namespace pdsp
{
/*!
    @cond HIDDEN_SYMBOLS
*/
    class GrainBased{
   
    public:
        GrainBased();
        ~GrainBased();
   
        static std::vector<GrainTable*> tables;
        static int unitCount;
        
        GrainTable* getTable(Window_t type, int length);
        GrainTable* getTable(int index);
        const int getAvailableTablesNumber() const;
        
        static void addTable(Window_t type, int length);
        
    };
/*!
    @endcond
*/    

}



#endif  // PDSP_GRAIN_GRAINBASED_H_INCLUDED
