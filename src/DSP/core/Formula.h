
// Formula.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_CORE_FORMULA_H_INCLUDED
#define PDSP_CORE_FORMULA_H_INCLUDED

#include "BasicNodes.h"

namespace pdsp{
    
    /*!
    @brief An abstract unit with one input and one output and no other controls, for simple processing.
    
    This is a class perform on a input the operation defined by the function formula() that the user can implement. If formulaAudioRate() is not implemented, formula() is performed for each sample, otherwise formula() is performent with control rate inputs and formulaAudioRate() for audio rate inputs.
    */   
    
class Formula :   public Unit{

public:
    Formula();
    
    /*!
    @brief returns the value of the input buffer, updated at control rate. This method is thread safe.
    */ 
    float meter_input() const;
    
    /*!
    @brief returns the value of the output buffer, updated at control rate. This method is thread safe.
    */ 
    float meter_output() const;
    
    /*!
    @brief sets the default input value and returns the Unit ready to be patched.
    */  
    Patchable& set(float value);
    
    /*!
    @brief sets some boundaries to which the values of the input will be clamped
    @param[in] lo low boundary (inclusive)
    @param[in] hi high boundary (inclusive)
    */  
    void enableBoundaries( float low, float high);
    
    /*!
    @brief disables the clamping of input values
    */  
    void disableBoundaries();
    
protected:

    /*!
    @brief function to be overloaded with your own math function or dsp code.
    @param[in] x input to process
    */  
    virtual float formula(const float & x) noexcept = 0;
        
    /*!
    @brief function to be overloaded with your own math function or dsp code.
    @param[in] output output array to fill with the rendered values
    @param[in] input input array to process
    @param[in] bufferSize number of samples to process
    */  
    virtual void formulaAudioRate(float* & output, const float* & input, const int & bufferSize) noexcept;

private:

    float lastProcessedValue;
    void process (int bufferSize) noexcept override;

    void prepareUnit( int expectedBufferSize, double sampleRate ) override;
    void releaseResources () override;
   
    OutputNode output;
    InputNode  input;        

    std::atomic<float> inputMeter;
    std::atomic<float> outputMeter;

};

}//END NAMESPACE






#endif  // FORMULA_H_INCLUDED
