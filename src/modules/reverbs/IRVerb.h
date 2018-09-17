
// IRVerb.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_MODULE_IRVERB_H_INCLUDED
#define PDSP_MODULE_IRVERB_H_INCLUDED

#include "../../DSP/pdspCore.h"
#include "../../DSP/convolution/FDLConvolver.h"
#include "../../DSP/samplers/SampleBuffer.h"


namespace pdsp{
    
/*!
@brief Impulse Response based reverb, with stereo or mono input and stereo output.
*/       

class IRVerb : public Patchable {


public:

    IRVerb();
    IRVerb(const IRVerb& other);
    IRVerb& operator=(const IRVerb& other);


    /*!
    @brief Uses the selected channel as input/output for the patching operation. 0 is for the left channel (default input/output) and 1 is for the right channel. Index values outside of range are remapped to 0 or 1.
    @param[in] index channel index
    */  
    Patchable& ch( size_t index );

    /*!
    @brief sets the impulse response for the reverb
    @param[in] path path of the impulse response file
    */  
    void loadIR ( std::string path );

/*!
    @cond HIDDEN_SYMBOLS
*/
    [[deprecated("in_mono() deprecated for this module, just use one channel for that")]]
    Patchable& in_mono();
  
    [[deprecated("in_0() deprecated for this module, use the ch( 0 ) method instead")]]
    Patchable& in_0();
    
    [[deprecated("in_1() deprecated for this module, use the ch( 1 ) method instead")]]
    Patchable& in_1();
    
    [[deprecated("out_0() deprecated for this module, use the ch( 0 ) method instead")]]
    Patchable& out_0();
    
    [[deprecated("out_1() deprecated for this module, use the ch( 1 ) method instead")]]
    Patchable& out_1();
    
    [[deprecated("in_L() deprecated for this module, use the ch( 0 ) method instead")]]
    Patchable& in_L();
    
    [[deprecated("in_R() deprecated for this module, use the ch( 1 ) method instead")]]
    Patchable& in_R();
    
    [[deprecated("out_L() deprecated for this module, use the ch( 0 ) method instead")]]
    Patchable& out_L();
    
    [[deprecated("out_R() deprecated for this module, use the ch( 1 ) method instead")]]
    Patchable& out_R();
/*!
    @endcond
*/

private:
    void patch();

    FDLConvolver    reverbL;
    FDLConvolver    reverbR;
    
    SampleBuffer    impulse;

};
    
    
} // end namespace



#endif // PDSP_MODULE_IRVERB_H_INCLUDED
