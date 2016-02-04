
// pdspFunctions.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_FUNCTIONS_H_INCLUDED
#define PDSP_FUNCTIONS_H_INCLUDED

#include "../math/header.h"

namespace pdsp {

inline_f bool checkTrigger(float x){
        return (x > 0.0f);
}

inline_f bool envTrigger(float x){
        return (x != 0.0f);
}

inline_f bool checkSync(float x){
        return (x >= 0.0f);
}

}//END NAMESPACE




#endif  // PDSP_FUNCTIONS_H_INCLUDED
