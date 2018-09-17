
// GrainCloud.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_MODULE_GRAINCLOUD_H_INCLUDED
#define PDSP_MODULE_GRAINCLOUD_H_INCLUDED

#include "../../DSP/pdspCore.h"
#include "../../DSP/utility/OneMinusInput.h"
#include <string>
#include "TriggeredGrain.h"

namespace pdsp{

    /*!
    @brief Stereo "cloud" of really short sampled segments. 
    
    By default it has 8 voices, but you can construct it with more voices if you use the non-default constructor. 
    */     
    
class GrainCloud : public Patchable {


    class MultiGrainTrigger : public Unit {
    public:

        MultiGrainTrigger(int outputs);
        
        std::vector<OutputNode> outs;
    
    private:
        void prepareUnit( int expectedBufferSize, double sampleRate ) override;
        void releaseResources () override {};
        void process (int bufferSize) noexcept override;
        
        InputNode in_distance_ms;
        InputNode in_jitter_ms;
        
        int counter;
        float sampleRateMult;
        int currentOut; 
        int outputs;
    };
    

public:

    GrainCloud(int voices);
    GrainCloud();
    GrainCloud(const GrainCloud &other);
    GrainCloud& operator=(const GrainCloud &other);

    /*!
    @brief returns the last processed envelope amplitude value of the selected grain. This method is thread-safe.
    */
    float meter_env(int voice) const;
    
    /*!
    @brief returns a value from 0.0f to 1.0f that broadly rapresent the "playhead" of the selected grain. This method is thread-safe.
    */ 
    float meter_position(int voice) const;

    /*!
    @brief returns a value from -1.0f to 1.0f that rapresent the jitter of the selected grain. This method is thread-safe.
    */ 
    float meter_jitter(int voice) const;

    /*!
    @brief Sets "position" as selected input and returns this module ready to be patched. This is the default input. Send a value between 0.0f and 1.0f to control the position of the grains inside the sample.
    */ 
    Patchable& in_position();
    
    /*!
    @brief Sets "length" as selected input and returns this module ready to be patched. Controls the grain length in milliseconds.
    */ 
    Patchable& in_length();
    
    /*!
    @brief Sets "pitch" as selected input and returns this module ready to be patched. Controls the grain pitch in semitones.
    */
    Patchable& in_pitch();
    
    /*!
    @brief Sets "select" as selected input and returns this module ready to be patched. Select the sample to stream from the ones previously added.
    */
    Patchable& in_select();    
    
    /*!
    @brief Sets "density" as selected input and returns this module ready to be patched. Controls the density of the graincloud by reducing the distance between grain retriggerings. 
    */    
    Patchable& in_density();
    
    /*!
    @brief Sets "distance_jitter" as selected input and returns this module ready to be patched. Controls a random value in millisecond that is added to the time between grain retriggerings.
    */ 
    Patchable& in_distance_jitter();
    
    /*!
    @brief Sets "position_jitter" as selected input and returns this module ready to be patched. Grain position jitter control relative to the sample length. 0.0f = no jitter, 1.0f = position jitter as long as the entire sample lenght.
    */ 
    Patchable& in_position_jitter();
    
    /*!
    @brief Sets "pitch_jitter" as selected input and returns this module ready to be patched. Grain pitch jitter in semitones.
    */ 
    Patchable& in_pitch_jitter();
    
    /*!
    @brief Sets "direction" as selected input and returns this module ready to be patched. A positive value makes the grains play forward, a negative in reverse.
    */ 
    Patchable& in_direction();
     
    /*!
    @brief Uses the selected channel as output for the patching operation. 0 is for the left channel (default output) and 1 is for the right channel. Index values outside of range are remapped to 0 or 1.
    @param[in] index channel index
    */  
    Patchable& ch( size_t index );
    
    /*!
    @brief returns the number of voices used for the graincloud.
    */ 
    int getVoicesNum() const;

    /*!
    @brief adds a pointer to a SampleBuffer to an internal array of SampleBuffer pointers
    @param[in] newSample pointer to a SampleBuffer
    */ 
    void addSample(SampleBuffer* newSample);
    
    /*!
    @brief Sets the SampleBuffer pointer at the given index to a new pointer.
    @param[in] samplePointer pointer to a sample buffer with a loaded file inside
    @param[in] index index of the position of the sample to set inside the sample pointers table
    */ 
    void setSample(SampleBuffer* samplePointer, int index=0);
    
    /*!
    @brief sets the envelope window shape, optionally the resolution of the table.
    @param[in] type window type
    @param[in] window_length window length, if not specified 1024
    */    
    void setWindowType(Window_t type, int window_length=1024 );

/*!
    @cond HIDDEN_SYMBOLS
*/    
    [[deprecated("out_L() deprecated for this module, use the ch( 0 ) method instead")]]
    Patchable& out_L();
    
    [[deprecated("out_R() deprecated for this module, use the ch( 1 ) method instead")]]
    Patchable& out_R();
/*!
    @endcond
*/

private:

    void patch();
    
    PatchNode       ctrl_start;
    PatchNode       ctrl_grain_length;
    PatchNode       ctrl_pitch;
    PatchNode       ctrl_select;
    PatchNode       ctrl_direction;
    
    PatchNode       ctrl_pitch_jit;
    PatchNode       ctrl_pos_jit_scale;
    PatchNode       ctrl_pos_jit;    
    
    OneMinusInput   ctrl_density;
    
    MultiGrainTrigger   triggers;
    
    std::vector<TriggeredGrain> streams;
    
    Amp     density_amp;
    
    Amp     outL;
    Amp     outR;
    
    int voices;
     
};

} //END NAMESPACE




#endif // PDSP_MODULE_GRAINCLOUD_H_INCLUDED
