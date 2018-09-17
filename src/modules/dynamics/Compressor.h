
// Compressor.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2018

#ifndef PDSP_MODULE_COMPRESSOR1_H_INCLUDED
#define PDSP_MODULE_COMPRESSOR1_H_INCLUDED

#include "../../DSP/pdspCore.h"
#include "../../DSP/dynamics/AbsoluteValue.h"
#include "../../DSP/dynamics/RMSDetector.h"
#include "../../DSP/dynamics/EnvelopeFollower.h"
#include "../../DSP/dynamics/GainComputer.h"
#include "../../DSP/filters/OnePole.h"
#include "../../DSP/utility/DBtoLin.h"
#include "../../DSP/utility/LinToDB.h"
#include "../../DSP/utility/MaxValue.h"
#include "../../DSP/dynamics/LHDelay.h"


namespace pdsp{
/*!
@brief Stereo Feed-forward compressor with peak-based or RMS detection and optional lookahead and stereo linking.

Feed-forward stereo compressor with peak or rms detection (defaults to peak), settable attack, release, threshold, ratio and knee. Optional lookahead. There is also a method for link the signal detection of the stereo channels, by default they are linked. If you set the ratio to a value greater than 40 the compressor will act as a limiter.
*/       

class Compressor : public Patchable {
    
public:
    Compressor( bool linkChannels ){ patch(linkChannels); };
    
    Compressor(){ patch(true); };
    Compressor(const Compressor& other){ patch(true); };
    Compressor& operator=(const Compressor& other){ return *this; };


    /*!
    @brief sets the ms window for the RMS detector and activates RMS detection mode
    @param[in] window_ms window milliseconds for detection. 50ms if not given.
    */          
    void RMS(float window_ms = 50.0f);

    /*!
    @brief activates peak detection mode. Peak detection is default.
    */          
    void peak();


    /*!
    @brief sets the envelope follower behavior to analog (slightly slower).
    */     
    void analog();

    /*!
    @brief sets the envelope follower behavior digital (more responsive, default behavior). Optional boolean value for activating the lookahead function (false by default).
    @param[in] lookahead activates the lookahead function
    */     
    void digital( bool lookahead = false );


    /*!
    @brief links the signal detection for stereo channels. Activated by default.
    @param[in] active activate the stereo link
    */          
    void stereoLink(bool active);
    

    /*!
    @brief Uses the selected channel as input/output for the patching operation. 0 is for the left channel (default input/output) and 1 is for the right channel. Index values outside of range are remapped to 0 or 1.
    @param[in] index channel index
    */  
    Patchable& ch( size_t index );
    
    /*!
    @brief Sets "attack" as selected input and returns this module ready to be patched. This input is the attack time of the compressor in milliseconds. Init default value is 10 ms.
    */   
    Patchable& in_attack();
    
    /*!
    @brief Sets "release" as selected input and returns this module ready to be patched. This input is the release time of the compressor in milliseconds. Init default value is 50 ms.
    */   
    Patchable& in_release();
    
    /*!
    @brief Sets "threshold" as selected input and returns this module ready to be patched. This is the threshold of the gain reduction. Default init value is -20dB.
    */      
    Patchable& in_threshold();
    
    /*!
    @brief Sets "ratio" as selected input and returns this module ready to be patched. This is the threshold of the gain reduction. If ratio is greater than 40 this module acts as a limiter. Default init value is 4.0f, that means 4:1 compression.
    */      
    Patchable& in_ratio();
    
    /*!
    @brief Sets "knee" as selected input and returns this module ready to be patched. This is the knee of the compressor. Init default value is 0dB.
    */  
    Patchable& in_knee();
  
    /*!
    @brief returns the gain reduction value (updated once each audio buffer). This method is thread-safe.
    */  
    float meter_GR() const ;
    
/*!
    @cond HIDDEN_SYMBOLS
*/
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
    void patch(bool stereoLink);
    
    struct Channel : public Patchable {
        Channel();
        PatchNode input;
        Amp dca;
    };
    
    Channel                 channel0;
    Channel                 channel1;
    
    FullWavePeakDetector    peak1;
    FullWavePeakDetector    peak2;
    RMSDetector             rms1;
    RMSDetector             rms2;
    
    LHDelay                 delay1;
    LHDelay                 delay2;
    
    PatchNode               attack;
    PatchNode               release;
    PatchNode               threshold;
    PatchNode               ratio;
    PatchNode               knee;
    
    EnvelopeFollower        envelope1;
    GainComputer            gr1;
    DBtoLin                 dbToLin1;
    LinToDB                 linToDB1;
    EnvelopeFollower        envelope2;
    GainComputer            gr2;
    DBtoLin                 dbToLin2;
    LinToDB                 linToDB2;

    OnePole                 leakDC1;
    OnePole                 leakDC2;

    MaxValue2               stereoLinker;

    Amp                     dca1;
    Amp                     dca2;
    
    bool linked;
    bool rms;
    bool lh;
    
    void repatch();
    
};

// legacy typedefs
typedef Compressor Compressor1;
typedef Compressor Compressor2;

} // end namespace





#endif // PDSP_MODULE_COMPRESSOR1_H_INCLUDED
