
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
@brief Feed-forward stereo compressor with peak-based or RMS detection and optional lookahead and stereo linking.

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
    @brief Sets "0" as selected input and returns this module ready to be patched. This is the default input. This is the left input channel.
    */      
    Patchable& in_0();

    /*!
    @brief Sets "0" as selected input and returns this module ready to be patched. This is the right input channel.
    */     
    Patchable& in_1();
    
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
    @brief Sets "0" as selected output and returns this module ready to be patched. This is the default output. This is the left output channel.
    */  
    Patchable& out_0();
    
    /*!
    @brief Sets "1" as selected output and returns this module ready to be patched. This is the right output channel. If you don't patch this output and you have called stereoLink(false) the compressor behave as a mono compressor with in_0() as input and out_0() as output.
    */  
    Patchable& out_1();
  
    /*!
    @brief returns the gain reduction value (updated once each audio buffer). This method is thread-safe.
    */  
    float meter_GR() const ;
    

    
private:
    void patch(bool stereoLink);
    
    PatchNode               input1;
    PatchNode               input2;
    
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
