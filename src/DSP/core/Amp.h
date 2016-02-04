
// Amp.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_CORE_AMP_H_INCLUDED
#define PDSP_CORE_AMP_H_INCLUDED


#include "BasicNodes.h"


namespace pdsp{


    /*!
    @brief Multiply in("signal") for in("mod").
    
    Multiply the in_signal() input for the in_mod() input. If the in_mod() input is running ad control rate and it is equal to 0, the in_signal() branch is not even calculated, saving cpu cycles.

    */
class Amp : public Unit{
    
public:
    
    Amp();
    
    ~Amp();
    Amp(const Amp& other);
    Amp& operator=(const Amp& other);
                
    /*!
    @brief set the default  in("mod") value and returns the unit ready to be patched.
    @param[in] value Value to set for scaling the input signal, Default is 0.0f . 
    */
    Patchable& set(float value);
    
    /*!
    @brief Sets "signal" as selected input and returns this Unit ready to be patched. This is the default input. This input is the signal/value to multiply.
    */   
    Patchable& in_signal();
    
    /*!
    @brief Sets "mod" as selected input and returns this Unit ready to be patched. Value from in("signal") is multiplied by this value/signal. If the input is running at control rate and it is equal to 0.0f the signal branch is not processed. Defaulot value is 0.0f .
    */   
    Patchable& in_mod();

    /*!
    @brief Sets "signal" as selected output and returns this Unit ready to be patched. This is the default output. This is the product of in("signal") multiplied by in("mod").
    */ 
    Patchable& out_signal();
    
    /*!
    @brief returns the value of the input buffer behind in("mod"), updated at control rate. This method is thread safe.
    */    
    float meter_mod() const;
    
    /*!
    @brief returns the value of the output buffer, updated at control rate. This method is thread safe.
    */ 
    float meter_out() const;
    
private:

    void prepareUnit ( int expectedBufferSize, double sampleRate ) override;
    
    void releaseResources () override;

    void process (int bufferSize) noexcept override;
    
    OutputNode output;
    InputNode input_mod;
    InputNode input_signal;
    
    std::atomic<float> meter;
    std::atomic<float> meterOut;
    
};    
        
}//END NAMESPACE




#endif  // AMP_H_INCLUDED
