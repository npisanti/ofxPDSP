
// Preparable.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_CORE_ENGINEINTERFACE_H_INCLUDED
#define PDSP_CORE_ENGINEINTERFACE_H_INCLUDED


#include <vector>
#include <math.h>
#include "../../math/header.h"
#include "../pdspConstants.h"
#include "../../messages/Clockable.h"

namespace pdsp{
    
    
/*!
@brief prepares all the Preparable to play, it is mandatory to be called at least once with the correct sample rate and buffer size
*/    
void prepareAllToPlay(int expectedBufferSize, double sampleRate);

/*!
@brief releases the resources of all the Preparable classes
*/    
void releaseAll();

/*!
@brief changes the default oversample level (default is 1 = no oversampling ) 
*/   
void setInitOversample(int initOversample);


class Preparable{
        friend void prepareAllToPlay(int expectedBufferSize, double sampleRate);
        friend void releaseAll();
        friend void setInitOversample(int initOversample);
        friend class Processor;

public:
    Preparable();
        
protected:
    /*!
    @cond HIDDEN_SYMBOLS
    */
    virtual void prepareToPlay(int expectedBufferSize, double sampleRate) = 0;
    
    virtual void releaseResources() = 0;
   

    virtual ~Preparable();

    static void setTurnBufferSize(int turnBufferSize);
    static std::vector<Preparable*>  constructionVector;
    static bool dynamicConstruction;
    static int globalBufferSize;
    static double globalSampleRate;
    static int initOversampleLevel;
    static int turnBufferSize;

    /*!
    @endcond
    */
};


    
}//END NAMESPACE


#endif  // ENGINEINTERFACE_H_INCLUDED
