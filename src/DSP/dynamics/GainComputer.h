
// GainComputer.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_DYN_GAINCOMPUTER_H_INCLUDED
#define PDSP_DYN_GAINCOMPUTER_H_INCLUDED


#include "../pdspCore.h"

namespace pdsp{
    /*!
    @brief Calculate gain reduction value. Needs an input converted from linear to dB values.
    
    This unit takes an input of values converted in db values (you can use LinToDB for this) and calculate the gain reduction to apply (in dB) with a given threshold, ratio and knee. For an example of a full compressor chain see the Compressor1 code. Threshold, ratio and knee are updated at control rate speed, so there is no reason to patch audio rate units to those inputs.
    */   
    
class GainComputer : public Unit{

public:
    GainComputer();
    
    /*!
    @brief sets the threshold, ratio and optionally knee default values and returns this unit ready to be patched.
    @param[in] threshold threshold befor compress the audio signal
    @param[in] ratio ratio of compression. If the value is greater than 40 then the units behave as a limiter.
    @param[in] knee knee of gain reduction in dB. If not given this argument is 0.0f
    */    
    Patchable& set(float threshold, float ratio, float knee=0.0f);

    /*!
    @brief returns the first value of the last processed buffer, it rappresent the gain reduction in db. This method is thread-safe.
    */  
    float meter_reduction() const ;

    /*!
    @brief Sets "signal" as selected input and returns this Unit ready to be patched. This is the default input. This input is the signal to process.
    */   
    Patchable& in_signal();
    
    /*!
    @brief Sets "threshold" as selected input and returns this Unit ready to be patched. This is the threshold of the gain reduction.
    */      
    Patchable& in_threshold();
    
    /*!
    @brief Sets "ratio" as selected input and returns this Unit ready to be patched. This is the ratio of gain reduction. If the input value is greater than 40 the gain reduction operate as a limiter.
    */  
    Patchable& in_ratio();
    
    /*!
    @brief Sets "knee" as selected input and returns this Unit ready to be patched. This is the knee of the gain reduction.
    */  
    Patchable& in_knee();
        
    /*!
    @brief Sets "signal" as selected output and returns this Unit ready to be patched. This is the default output. This is the gain reduction in dB to apply to the signal.
    */ 
    Patchable& out_signal();
    
private:
    void prepareUnit( int expectedBufferSize, double sampleRate ) override;
    void releaseResources () override ;
    void process (int bufferSize) noexcept override;

    InputNode       input;
    
    InputNode       in_thresh;
    InputNode       input_ratio;
    InputNode       input_knee;
     
    OutputNode      output;
    float CS;
    
    std::atomic<float> meter;
    
};

}//END NAMESPACE


#endif  // PDSP_DYN_GAINCOMPUTER_H_INCLUDED
