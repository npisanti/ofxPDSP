
// FDLConvolver.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_SFDLCONVOLVER_H_INCLUDED
#define PDSP_SFDLCONVOLVER_H_INCLUDED


#include "../pdspCore.h"
#include "../helpers/FFTWorker.h"
#include "../samplers/SampleBuffer.h"

namespace pdsp{
/*!

@brief Process the input using FFT convolution with a given impulse response.

This Units implement partitioned convolution using a Frequency-Domain Delay Line. Expecially useful if you have some real space impulse response to be used to make a IR Reverb.
*/

class FDLConvolver : public Unit {

public:
        FDLConvolver();
        
        /*!
        @brief Sets "signal" as selected input and returns this Unit ready to be patched. This is the default input. This input is the audio input of the convolver.
        */ 
        Patchable& in_signal();
        
        /*!
        @brief Sets "signal" as selected output and returns this Unit ready to be patched. This is the default output. This is the processed output.
        */ 
        Patchable& out_signal();
        
        /*!
        @brief Sets the impulse response for the convolution.
        @param[in] impulseResponse SampleBuffer to load as Impulse Response for the convolution.
        @param[in] channel select the channel to be if the SampleBuffer has more than one. If omitted the first channel is selected.
        */
        void loadIR ( SampleBuffer & impulseResponse, int channel=0);
       
/*!
    @cond HIDDEN_SYMBOLS
*/
        [[deprecated("deprecated, now direclty pass your SampleBuffer")]]
        void loadIR ( SampleBuffer* impulseResponse, int channel=0);
/*!
    @endcond
*/

private:
        void prepareUnit( int expectedBufferSize, double sampleRate ) override;
        void releaseResources () override ;
        void process (int bufferSize) noexcept override;

        void prepareIR();
        
        void deallocateBlocksBuffers();
        bool allocateBlocksBuffers();

        bool loadImpulseResponseSegments(const float &inc, const int &convertedLen);
        
        OutputNode output;
        InputNode input;
        
        int numBlocks;
        int blockIndex;
        int complexSize;
        int signalBlock;
        int processingSize;
        int processingOffset;
        int silenceCount;
        
        float* addR;
        float* addI;
        float*  paddedInput;
        float*  overlapAdd;
        
        float** impulseR;
        float** impulseI;
        float** circularR;
        float** circularI;      
        
        int             IRChannel;
        SampleBuffer*   impulseResponse;
        bool            IRLoaded; 
        double          sampleRate;
        
        static FFTWorker    fftWorker;
           
};    


}//END NAMESPACE



#endif  // PDSP_SFDLCONVOLVER_H_INCLUDED
