
// fasttanh.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_MATH_FASTTANH_H_INCLUDED
#define PDSP_MATH_FASTTANH_H_INCLUDED

#include "../functions.h"

namespace pdsp{
    
    inline_f float fasttanh(float angle){
        
        return angle/(fabs(2*angle)+3/(2+2*angle*2*angle));
        
    }
    
}

#endif  // PDSP_MATH_FASTTANH_H_INCLUDED
