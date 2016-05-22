
// KneeClipper.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_MODULE_KNEECLIPPER_H_INCLUDED
#define PDSP_MODULE_KNEECLIPPER_H_INCLUDED

#include "../../DSP/pdspCore.h"
#include "../../DSP/utility/DBtoLin.h"
#include "../../DSP/utility/SaturatorThreshold.h"
#include "../../DSP/signal/Saturator1.h"


namespace pdsp{
    
    /*!
    @brief Clipper with adjustable knee, for saturating limitation of signals.
    
    It works better with signals below the 0dB range, high gain signals will make it sound almost like a ringmodulator.
    */  
       
class KneeClipper : public Patchable {
    

public:
    KneeClipper(){ patch(); };
    KneeClipper(const KneeClipper& other){ patch(); };
    KneeClipper& operator=(const KneeClipper& other){ return *this; };

    
    /*!
    @brief Sets "0" as selected input and returns this module ready to be patched. This is the default input. This is the left input channel.
    */      
    Patchable& in_0();

    /*!
    @brief Sets "1" as selected input and returns this module ready to be patched. This is the right input channel.
    */     
    Patchable& in_1();

    /*!
    @brief Sets "knee" as selected input and returns this module ready to be patched. This is the knee of the clipping curve, the saturation will start at a value equal to threshold + knee, a good value for the knee is between -1.0f (-1dB) and -6.0f (-6dB). Default value is -3dB.
    */      
    Patchable& in_knee();

    /*!
    @brief Sets "threshold" as selected input and returns this module ready to be patched. This is the threshold the signal will not overshoot.
    */      
    Patchable& in_threshold();
    
    /*!
    @brief Sets "0" as selected output and returns this module ready to be patched. This is the default output. This is the left output channel.
    */  
    Patchable& out_0();
    
    /*!
    @brief Sets "1" as selected output and returns this module ready to be patched. This is the right output channel. If you don't patch this output the effect will behave as a mono EQ.
    */  
    Patchable& out_1();
            
            
private:
    void patch ();
    
    PatchNode   freq;

    PatchNode   input0;
    PatchNode   input1;
    
    PatchNode   threshold_ctrl;
    PatchNode   knee_ctrl;
    
    
    Amp         input_gain0;
    Amp         input_gain1;
    Amp         output_gain0;
    Amp         output_gain1;
    
    DBtoLin     input_gain_db;
    DBtoLin     output_gain_db;
    
    DBtoLin     knee_gain_db;
    
    SaturatorThreshold threshold0;
    SaturatorThreshold threshold1;
    
    Saturator1 sat0;
    Saturator1 sat1;
    
};

} // pdsp namespace end


#endif // PDSP_MODULE_KNEECLIPPER_H_INCLUDED
