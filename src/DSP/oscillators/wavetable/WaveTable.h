
#ifndef PDSP_OSC_WAVETABLE_H_INCLUDED
#define PDSP_OSC_WAVETABLE_H_INCLUDED


#include "../../samplers/SampleBuffer.h"

namespace pdsp {
    
    /*!
    @brief Utility class for storing and loading buffers of waveforms
    
    */
    
class WaveTable {
    friend class WaveTableOsc;
    
public:    

    WaveTable();
    ~WaveTable();

    /*!
    @brief add an empty waveform to the buffer, 
    */    
    void addEmpty();
    
    /*!
    @brief add a waveform from a sample file located at path
    @param[in] path absolute or relative path to audio file
    
    */    
    void addSample( std::string path );

    /*!
    @brief generate a waveform from an inlined list of partials. This function is precise but not fast, use it before starting the DSP engine.
    @param[in] partials inlined array of partials 
    @param[in] harmonicScale if true each value is multiplied by a saw wave corresponding partial
    
    */    
    void addAdditiveWave( std::initializer_list<double> partials, bool harmonicScale = false );

    /*!
    @brief generate a waveform from a given vector of partials. This function is precise but not fast, use it before starting the DSP engine.
    @param[in] partials inlined array of partials 
    @param[in] harmonicScale if true each value is multiplied by a saw wave corresponding partial
    
    */    
    void addAdditiveWave( const std::vector<double> & partials, bool harmonicScale = false );

    /*!
    @brief generate a sawtooth waveform with the given number of of partials. This function is precise but not fast, use it before starting the DSP engine.
    @param[in] partials number of partials
    
    */    
    void addSawWave( int partials );


    /*!
    @brief generate a square waveform with the given number of of partials. This function is precise but not fast, use it before starting the DSP engine.
    @param[in] partials number of partials
    
    */    
    void addSquareWave ( int partials );
    

    /*!
    @brief generate a triangle waveform with the given number of of partials. This function is precise but not fast, use it before starting the DSP engine.
    @param[in] partials number of partials
    
    */        
    void addTriangleWave ( int partials );


    /*!
    @brief generate a sine waveform. This function is precise but not fast, use it before starting the DSP engine.
    
    */        
    void addSineWave ( );

    /*!
    @brief returns an handle to the buffer of the wave at the given index
    @param[in]  index index of the wavetable
    */    
    float* table( int index );

    /*!
    @brief returns the lenght of each wave buffer
    */  
    int tableLength() const;

    /*!
    @brief sets the table length, it is called automatically if you add samples and you should call it by yourself only if you want to generate your waves.
    */  
    void initLength( int len );

    /*!
    @brief returns the size of the wavetable
    */  
    int size() const;

    /*!
    @brief enables logs for file loading
    */      
    void setVerbose( bool verbose );
    
private:    
    float ** buffer;    
    int length;
    int tableSize;
    bool verbose;
        
    SampleBuffer loader;
};    

}


#endif // PDSP_OSC_WAVETABLE_H_INCLUDED
