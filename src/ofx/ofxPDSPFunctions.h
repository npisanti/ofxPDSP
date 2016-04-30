
#ifndef OFXPDSP_FUNCTIONS_INCLUDED
#define OFXPDSP_FUNCTIONS_INCLUDED

#include "../DSP/core/Preparable.h"
#include "../DSP/utility/PitchToFreq.h"
#include "../DSP/utility/DBtoLin.h"
#include "../math/random/random.h"


/*!
@brief documentation of some global functions ( ofxPDSPFunctions is just a dummy class for generating this doc ).
*/
class ofxPDSPFunctions{
    friend void ofxPDSPSetup(int bufferSize, double sampleRate);
    friend void ofxPDSPRelease();
    friend float p2f( float pitch);
    friend void setReferenceFrequency(float frequency);
    friend float panL ( float pan );
    friend float panR ( float pan );
    friend float dB ( float dBvalue );
    friend bool pdspCoin( float chance );
    friend float pdspURan( );
    friend float pdspRan( );
};

/*!
@brief prepares all the module and units, initializing their internal buffers. You don't have to worry about this if you're using an ofxPDSPEngine.
@param[in] bufferSize audio callbacks expected buffer size
@param[in] sampleRate audio callbacks sample rate
*/
void ofxPDSPSetup(int bufferSize, double sampleRate);


/*!
@brief releases all the module and units initialized resources. You don't have to worry about this if you're using an ofxPDSPEngine.
*/
void ofxPDSPRelease();


/*!
@brief take a pitch value as input an returns a frequency value. Standard reference tuning is 440hz, you can set it with setReferencyFrequency().
@param[in] pitch pitch value to convert
*/
float p2f( float pitch);


/*!
@brief set the reference tuning frequency. Standard reference tuning is 440hz.
@param[in] frequency new reference tuning
*/
void setReferenceFrequency(float frequency);


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

/*!
@brief returns a random bool value, with the give change of having "true" as result. Use this instead of ofRandom() in the code that should run in the audio thread.
@param[in] value value that rapresent the possibility of a true outcome. Use values in the 0.0f - 1.0f range.
*/
inline_f bool pdspChance( float value ){
    return pdsp::chance(value);
};

/*!
@brief returns a random int value that ranges from 0 to sides-1
@param[in] sides number of possible outcomes
*/
inline_f int pdspDice( float sides ){
    return pdsp::dice(sides);
};

/*!
@brief returns a random float value ranging from 0.0f, to the 1.0f. Use this instead of ofRandom() in the code that should run in the audio thread.
*/
inline_f float pdspURan( ){
    return pdsp::randomUnipolarInclusive();
};

/*!
@brief returns a random float value ranging from -1.0f to 1.0f. Use this instead of ofRandom() in the code that should run in the audio thread.
*/
inline_f float pdspBRan(){
    return pdsp::randomBipolar();
};


/*!
@brief returns a value for panning voices in an uniform distribution into the stereo panorama
@param[in] index index of the voice to pan
@param[in] max number of max voices
@param[in] spread spread of the total panorama, value should range from 0.0f to 1.0f
*/
float pdspSpread(int index, int max, float spread);


#endif // OFXPDSP_FUNCTIONS_INCLUDED
