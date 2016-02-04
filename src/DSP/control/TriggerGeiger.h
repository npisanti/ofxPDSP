
// TriggerGeiger.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_CONTROL_TRIGGERGEIGER_H_INCLUDED
#define PDSP_CONTROL_TRIGGERGEIGER_H_INCLUDED

#include "../pdspCore.h"

namespace pdsp{
    
/*!
@brief Generate randomly separated trigger values.

TriggerGeiger generate trigger values (only on values, no off) at a distance given by the in_distance() input value plus a value that goes randomly from zero to in_jitter() input value.
*/

class TriggerGeiger : public Unit
{

public:
        TriggerGeiger();


        /*!
        @brief Sets the default distance and jitter and returns the Unit ready to be patched
        @param[in] distance Default distance value to set, it will be overrided by patching.
        @param[in] jitter Default jitter value to set, it will be overrided by patching.
        */
        Patchable& set(float distance, float jitter);
      
        /*!
        @brief Sets "distance" as selected input and returns this Unit ready to be patched. This is the default input. This input is the base time distance between triggers. A random value given by in_jitter() is added to this base time. Default value is 200ms.
        */ 
        Patchable& in_distance();
     
        /*!
        @brief Sets "signal" as selected input and returns this Unit ready to be patched. This input set the max in millisecond of the randomly choosen time between triggers. Default value is 0ms.
        */ 
        Patchable& in_jitter();
        
        /*!
        @brief Sets "trig" as selected output and returns this Unit ready to be patched. This is the default output. This is the trigger output to connect to envelopes or "trig" inputs. 
        */   
        Patchable& out_trig();


private:
        void prepareUnit( int expectedBufferSize, double sampleRate ) override;
        void releaseResources () override {};
        void process (int bufferSize) noexcept override;
        
        InputNode in_distance_ms;
        InputNode in_jitter_ms;
        OutputNode output_trig;

        int counter;
        float sampleRateMult;

};

     
} // pdsp namespace end



#endif // PDSP_CONTROL_TRIGGERGEIGER_H_INCLUDED
