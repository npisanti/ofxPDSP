
// SampleBuffer.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_SAMPLERS_SAMPLEBUFFER_H_INCLUDED
#define PDSP_SAMPLERS_SAMPLEBUFFER_H_INCLUDED

#include "../pdspCore.h"
#include <assert.h>
#include "../../flags.h"

#if (defined(PDSP_USE_LIBSNDFILE) || defined(__linux))
    #include <sndfile.hh>
#endif

#include <cstring>


namespace pdsp {

    /*!
    @brief Contains the data loaded from an audio file.

    This is a class that contains data loaded from an audio file (or created in any other way). It is used by units that require samples like Sampler, FDLConvolver or TableOsc. It uses libsndfile to load audio file from path so if you want to use it you have to link libsndfile to your project, go in flags.h and uncomment #define PDSP_USE_LIBSNDFILE. You also use any method you have on you platform for getting an interleaved or a mono array of floats and load it with  load( float* interleavedBuffer, double sampleRate, int length, int channels=1 )
    
    */

class SampleBuffer {

public:
    SampleBuffer();
    SampleBuffer(const SampleBuffer& other) = delete;
    SampleBuffer& operator= (const SampleBuffer& other) = delete;
    ~SampleBuffer();
    
    /*!
    @brief loads the audio data from a file using libsndfile
    @param[in] filePath absolute or relative path to audio file
    */
    void    load( std::string filePath );
    
    /*!
    @brief loads the audio data from a given interleaved float array (or from a single non interleaved channel)
    @param[in] interleavedBuffer interleaved buffer to load. Obviously if channels==1 the buffer is not interleaved
    @param[in] sampleRate sample rate of the loaded file
    @param[in] length length of a single channel, NOT the lenght of the full interleaved buffer
    @param[in] channels number of channels interleaved in the buffer, if not specified 1
    */
    void    load( float* interleavedBuffer, double sampleRate, int length, int channels=1 );

    /*!
    @brief unload the data from the buffer and free the allocated memory
    */    
    void    unLoad();
    
    /*!
    @brief returns true if data has been loaded into the SampleBuffer
    */ 
    bool    loaded();
    
    /*!
    @brief some Units automatically select one channel if more than one are loaded. This set this default channel.
    @param[in] monoChannelIndex
    */ 
    void    setMonoChannel(int monoChannelIndex);
    
    /*!
    @brief activate logging of file loading operations for this SampleBuffer
    @param[in] verbose
    */     
    void    setVerbose(bool verbose);

    /*!
    @brief path of loaded file, or contains diagnostic strings if a file is not loaded
    */  
    std::string     filePath;
    
    /*!
    @brief multidimensional buffer, use it like this: buffer[channel][sample]
    */      
    float**         buffer;
    
    /*!
    @brief number of channels
    */  
    int             channels;
    
    /*!
    @brief index of channel to select for mono operation
    */    
    int             mono;
    
    /*!
    @brief length of each buffer
    */    
    int             length;
    
    /*!
    @brief sample rate of the loaded channels
    */    
    double          fileSampleRate;

private:
    bool            verbose;

};

} // end pdsp namespace



#endif  // PDSP_SAMPLERS_SAMPLEBUFFER_H_INCLUDED
