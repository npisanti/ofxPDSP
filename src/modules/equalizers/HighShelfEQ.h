
// HighShelfEQ.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016-2018

#ifndef PDSP_MODULE_HIGHSHELFEQ_H_INCLUDED
#define PDSP_MODULE_HIGHSHELFEQ_H_INCLUDED

#include "../../DSP/pdspCore.h"
#include "../../DSP/filters/biquads/BiquadHighShelf.h"

namespace pdsp{
    
    /*!
    @brief High Shelf Equalizer. Multichannel.
    */  
    
class HighShelfEQ : public Patchable {
    
public:
    HighShelfEQ(){ patch(); };
    HighShelfEQ(const HighShelfEQ& other){ patch(); };
    HighShelfEQ& operator=(const HighShelfEQ& other){ return *this; };
    ~HighShelfEQ(){ channels(0); }
    
    /*!
    @brief Sets "signal" as selected input and returns this module ready to be patched. This is the default input. This is the first input channel.
    */      
    Patchable& in_signal();

    /*!
    @brief Sets "signal" as selected output and returns this module ready to be patched. This is the default output. This is the first output channel.
    */  
    Patchable& out_signal();

    /*!
    @brief Sets "freq" as selected input and returns this module ready to be patched. This is the frequency at which the EQ operates.
    */      
    Patchable& in_freq();
    
    /*!
    @brief Sets "Q" as selected input and returns this module ready to be patched. This is the "quality" factor of the EQ and controls the shape of the EQ at the corner frequency.
    */     
    Patchable& in_Q();
    
    /*!
    @brief Sets "gain" as selected input and returns this module ready to be patched. This is the amount of gain to add or remove at the set frequency.
    */     
    Patchable& in_gain();


    /*!
    @brief Allocate a number of channels for processing different inputs. This is automatically called if you query for a channel outside the allocated range. You can access different channels with the ch() method.
    @param[in] size number of channels
    */        
    void channels( size_t size );    

    /*!
    @brief Uses the selected channel as input/output for the patching operation.
    @param[in] index channel index
    */  
    Patchable& ch( size_t index );
        
/*!
    @cond HIDDEN_SYMBOLS
*/
    [[deprecated("operator[] deprecated, use the ch( int index ) method instead")]]    
    Patchable& operator[]( size_t index );
    
    [[deprecated("in_0() deprecated, use the ch( int index ) method instead")]]
    Patchable& in_0();
    
    [[deprecated("in_1() deprecated, use the ch( int index ) method instead")]]
    Patchable& in_1();
    
    [[deprecated("out_0() deprecated, use the ch( int index ) method instead")]]
    Patchable& out_0();
    
    [[deprecated("out_1() deprecated, use the ch( int index ) method instead")]]
    Patchable& out_1();
/*!
    @endcond
*/

   
private:
    void patch ();
    
    PatchNode   freq;
    PatchNode   Q;
    PatchNode   gain;
    
    std::vector<BiquadHighShelf*>  eqs;
    
};

} // pdsp namespace end


#endif // PDSP_MODULE_HIGHSHELFEQ_H_INCLUDED
