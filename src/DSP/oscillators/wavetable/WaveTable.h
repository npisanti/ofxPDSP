
#ifndef PDSP_OSC_WAVETABLE_H_INCLUDED
#define PDSP_OSC_WAVETABLE_H_INCLUDED


#include "../../samplers/SampleBuffer.h"
#include <map>

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
    @brief clears the table at the given index.
    @param[in] index index of the wavetable
    */    
    void setEmpty( int index );
    
    
    /*!
    @brief sets the table at the given index to a sine waveform a waveform from a sample file located at path
    @param[in] index index of the wavetable
    @param[in] path absolute or relative path to audio file
    
    */    
    void setSample( int index, std::string path );


    /*!
    @brief generate a waveform from an inlined list of partials. This function is not thread safe, avoid access to the given index while you set wave,
    @param[in] index index of the wavetable
    @param[in] partials inlined array of partials 
    @param[in] harmonicScale if true each value is multiplied by a saw wave corresponding partial
    
    */    
    void setAdditiveWave( int index,  std::initializer_list<float> partials, bool harmonicScale = false );


    /*!
    @brief sets the table at the given index to a waveform from a given vector of partials. This function is not thread safe, avoid access to the given index while you set wave,
    @param[in] index index of the wavetable
    @param[in] partials inlined array of partials 
    @param[in] harmonicScale if true each value is multiplied by a saw wave corresponding partial
    
    */    
    void setAdditiveWave( int index, const std::vector<float> & partials, bool harmonicScale = false );


    /*!
    @brief sets the table at the given index to a sine waveform a sawtooth waveform with the given number of of partials. This function is not thread safe, avoid access to the given index while you set wave,
    @param[in] index index of the wavetable
    @param[in] partials number of partials
    
    */    
    void setSawWave( int index, int partials );


    /*!
    @brief sets the table at the given index to a sine waveform a square waveform with the given number of of partials. This function is not thread safe, avoid access to the given index while you set wave,
    @param[in] index index of the wavetable
    @param[in] partials number of partials
    
    */    
    void setSquareWave ( int index, int partials );


    /*!
    @brief sets the table at the given index to a sine waveform a triangle waveform with the given number of of partials. This function is not thread safe, avoid access to the given index while you set wave,
    @param[in] index index of the wavetable
    @param[in] partials number of partials
    
    */        
    void setTriangleWave ( int index, int partials );


    /*!
    @brief sets the table at the given index to a sine waveform. This function is not thread safe, avoid access to the given index while you set wave,
    @param[in] index index of the wavetable
    
    */        
    void setSineWave ( int index );


    /*!
    @brief adds an empty waveform. This function reallocate all the internal pointers, never access the wavetable while you call it.
    
    @param[in] numberOfWavesToAdd number of empty waveforms to add. 1 if not given.
    */    
    void addEmpty(int numberOfWavesToAdd=1);

    
    /*!
    @brief add a waveform from a sample file located at a given path. This function reallocate all the internal pointers, never access the wavetable while you call it.
    @param[in] path absolute or relative path to audio file
    
    */    
    void addSample( std::string path );


    /*!
    @brief generate a waveform from an inlined list of partials. This function reallocate all the internal pointers, never access the wavetable while you call it.
    @param[in] partials inlined array of partials 
    @param[in] harmonicScale if true each value is multiplied by a saw wave corresponding partial
    
    */    
    void addAdditiveWave( std::initializer_list<float> partials, bool harmonicScale = false );


    /*!
    @brief generate a waveform from a given vector of partials. This function reallocate all the internal pointers, NEVER access the wavetable while you call it.
    @param[in] harmonicScale if true each value is multiplied by a saw wave corresponding partial
    
    */    
    void addAdditiveWave( const std::vector<float> & partials, bool harmonicScale = false );


    /*!
    @brief generate a sawtooth waveform with the given number of of partials. This function reallocate all the internal pointers, NEVER access the wavetable while you call it.
    @param[in] partials number of partials
    
    */    
    void addSawWave( int partials );


    /*!
    @brief generate a square waveform with the given number of of partials. This function reallocate all the internal pointers, NEVER access the wavetable while you call it.
    @param[in] partials number of partials
    
    */    
    void addSquareWave ( int partials );
    

    /*!
    @brief generate a triangle waveform with the given number of of partials. This function reallocate all the internal pointers, NEVER access the wavetable while you call it.
    @param[in] partials number of partials
    
    */        
    void addTriangleWave ( int partials );


    /*!
    @brief generate a sine waveform. This function reallocate all the internal pointers, NEVER access the wavetable while you call it.
    
    */        
    void addSineWave ( );


    /*!
    @brief returns an handle to the buffer of the wave at the given index, avoid accessing the given index while you modify the wave values.
    @param[in]  index index of the wavetable
    */    
    float* table( int index );


    /*!
    @brief returns the lenght of each wave buffer
    */  
    int tableLength() const;


    /*!
    @brief sets the table length, and the maximum number of partial.
    @param[in]  len length of each wave sample buffer
    @param[in]  maxPartials max number of partials available for the waves. 64 if not given.
    
    It is called automatically if you add samples as first waves, maxPartial default is 64. Keeping the partial number low will use less memory, 128-256 is enough, the old Waldorf synths sounded good with just 64 partial waves.

    */  
    void setup( int len, int maxPartials=64);
    
    
    [[deprecated("Replaced by setup(int len, int maxPartials)")]]
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

    int maxPartials;
    static std::map<int, std::vector<float*>> partialsTable;
        
    SampleBuffer loader;
};    

}


#endif // PDSP_OSC_WAVETABLE_H_INCLUDED
