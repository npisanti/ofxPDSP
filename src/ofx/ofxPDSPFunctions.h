
#ifndef OFXPDSP_FUNCTIONS_INCLUDED
#define OFXPDSP_FUNCTIONS_INCLUDED

#include "../DSP/core/Preparable.h"
#include "../DSP/utility/PitchToFreq.h"
#include "../DSP/utility/DBtoLin.h"
#include "../math/random/random.h"

/*!
@brief returns a float value you can use to multiply your signal for panning when patching to a left channel
@param[in] pan pan value
*/
float panL ( float pan );

/*!
@brief returns a float value you can use to multiply your signal for panning when patching to a right channel
@param[in] pan pan value
*/
float panR ( float pan );


/*!
@brief returns a float value you can use to multiply your signal, the value returned is the linar equivalent of the given dB value;
@param[in] dBvalue deciBel value for gain scaling
*/
float dB ( float dBvalue );


namespace pdsp{


/*!
@brief take a frequency value as input an returns a pitch value. Standard reference tuning is 440hz, you can set it with setReferencyFrequency() (remember to set it before using this function!).
@param[in] freq freq value to convert
*/
float f2p( float freq);


/*!
@brief take a pitch value and a float ratio. The returns a pitch that is the pitch value of the frequency corresponding to the first pitch, multiplied by the ratio.
@param[in] pitch base pitch value
@param[in] ratio ratio of the frequency multiplication
*/
float pratio ( float pitch, float ratio );


/*!
@brief take a pitch value as input an returns the highest partial before 20000hz of a waweform with that base pitch. Useful for initializing pdsp::WaveTable, to avoid aliasing use the pitch of the maximum note you want to use with that oscillator. This function is calibrated for 44.1 kHz sapling rate.
*/
int highestPartial( float pitch );


/*!
@brief returns a random float value ranging from 0.0f, to the 1.0f. Use this instead of ofRandom() in the code that should run in the audio thread.
*/
inline_f float uran( ){
    return randomUnipolarInclusive();
};

/*!
@brief returns a random float value ranging from -1.0f to 1.0f. Use this instead of ofRandom() in the code that should run in the audio thread.

*/
inline_f float bran(){
    return randomBipolar();
};

/*!
@brief returns a random int value that ranges from min to maxExclusive-1
@param[in] min number
@param[in] maxExclusive
*/
inline_f int dice(  int min, int maxExclusive  ){
    return (dice(maxExclusive-min) + min);
};


/*!
@brief returns a value for panning voices in an uniform distribution into the stereo panorama
@param[in] index index of the voice to pan
@param[in] max number of max voices
@param[in] spread spread of the total panorama, value should range from 0.0f to 1.0f
@param[in] fromCenter if true the spreading will start from the center and alternate notes at right and left

*/
float spread(int index, int max, float spread, bool fromCenter = false);


/*!
@brief take a pitch value as input an returns a frequency value. Standard reference tuning is 440hz, you can set it with setReferencyFrequency() (remember to set it before using this function!).
@param[in] pitch pitch value to convert
*/
float p2f( float pitch);

} // end namespace



/*!
 @brief deactivated documentation of some global functions ( ofxPDSPFunctions is just a dummy class for generating this doc ).
*/
class ofxPDSPFunctions{

    friend float pdsp::p2f( float pitch);
    friend float pdsp::pratio( float pitch, float ratio);
    friend int pdsp::highestPartial( float pitch );
    friend float pdsp::f2p( float freq );
    friend float panL ( float pan );
    friend float panR ( float pan );
    friend float dB ( float dBvalue );
    friend bool pdsp::chance( float chance );
    friend float pdsp::uran( );
    friend float pdsp::bran( );
    friend int pdsp::dice( int sides );    
    friend int pdsp::dice( int min, int maxExclusive );
    friend float pdsp::spread(int index, int max, float spread, bool fromCenter);
};


// ------------------------- OLD FUNCTIONS -----------------------------------

/*!
    @cond HIDDEN_SYMBOLS
*/

inline float pdspSpread(int index, int max, float spread, bool fromCenter = false){ 
    return pdsp::spread( index, max, spread, fromCenter );
}

inline_f bool pdspChance( float value ){
    return pdsp::chance(value);
};

inline_f int pdspDice( int sides ){
    return pdsp::dice(sides);
};

inline_f int pdspDice(  int min, int maxExclusive  ){
    return (pdsp::dice(maxExclusive-min) + min);
};

inline_f float pdspURan( ){
    return pdsp::randomUnipolarInclusive();
};

inline_f float pdspBRan(){
    return pdsp::randomBipolar();
};

/*!
    @endcond
*/


#endif // OFXPDSP_FUNCTIONS_INCLUDED
