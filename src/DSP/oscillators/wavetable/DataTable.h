// DataTable.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2017

#ifndef PDSP_OSC_DATATABLE_H_INCLUDED
#define PDSP_OSC_DATATABLE_H_INCLUDED

#include "../pdspCore.h"

namespace pdsp {
    
    /*!
    @brief Utility class for creating waveform from realtime data, thread safely.
    
    */
    
class DataTable {
    friend class DataOsc;
    
public:    

    DataTable();
    ~DataTable();

    /*!
    @brief activate the smoothing of the transition to the new setted waves
    @param[in] value a value in the 0.0f-0.9999f (boundary checked). 0.0f deactivate smoothing
    */     
    void smoothing( float value );

    /*!
    @brief when setting the data with additive synthesis, scales the harmonics to the saw wave ratio
    @param[in] active activate the harmonic scaling, it is active by default
    */      
    void harmonicAdditiveMode( bool active );
    
    /*!
    @brief returns the lenght of each wave buffer
    */  
    int getTableLength() const;
    
    /*!
    @brief returns the number of partials you can set
    */  
	int getMaxPartials() const;

    /*!
    @brief sets the table length, and the maximum number of partial.
    @param[in]  len length of each wave sample buffer
    @param[in]  maxPartials max number of partials available for the waves. 64 if not given.
    @param[in]  harmonicAdditive mode for additive synthesis. true if not given.
    */  
    void setup( int len, int maxPartials=64, bool harmonicAdditive=true);
   

    /*!
    @brief checks if you can set the wave, always use it for thread-safe operations
    */     
	bool ready();
	
    /*!
    @brief begins the setting of the data vector. Check if the DataTable is ready() before calling it
    */  
	void begin();
	
    /*!
    @brief sets a value in the data vector. Not setted values are set to 0.0f.
    @param[in]  i index of the value
    @param[in]  value data value. Use values in the -1.0f<-->1.0f range
    */  
    void data( int i, float value );
    
    /*!
    @brief end the data vector setting. Transform the values in the vector in a waveform. If the parameter is true the wave is set using additive synthesis, each data value is the amplitude of a partial, by default the partial are also scaled to the harmonic ratio. If the parameter is false the values is interpreted as raw waveform. Beware that if you are setting directly a waveform you could cause a DC offset.
    @param[in]  additive if true the wave is set using additive synthesis, otherwive the data is interpreted as raw waveform
    */  
    void end( bool additive ); 
    
private:    
	void update(); // accessible from DataOscillator
								// use OutputNode::getGlobalProcessingTurnId() to check if it needs updating
	
	void additive();


    float * buffer;  // read by DataOscillator
    int length;

    int maxPartials;
    float ** partialsTable;    

    float * bufferNew;            
	std::vector<float> vData;
	std::atomic<bool>  bSetting;
	std::atomic<bool>  bLoaded;
	std::atomic<bool>  bAdditive;
	std::atomic<bool>  bHarmonic;
	int lastIndex;
	int lastTurn;
	
	std::atomic<float> smooth;
	
};    

}


#endif // PDSP_OSC_WAVETABLE_H_INCLUDED
