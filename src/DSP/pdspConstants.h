
// pdspConstants.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSPCOSTANTS_H_INCLUDED
#define PDSPCOSTANTS_H_INCLUDED

#include "../types.h"


#ifndef M_TAU
    #define M_TAU 6.2831853071795864769252f
#endif

#ifndef M_TAU_DOUBLE
    #define M_TAU_DOUBLE 6.283185307179586476925286766559005768
#endif

#ifndef M_PI_DOUBLE
    #define M_PI_DOUBLE 3.141592653589793238462643383279502884
#endif


namespace pdsp{

static const float pdspSyncNull = -1.0f;
static const float pdspSyncPulse = 0.0f;


//static const float pdspTriggerOff = -32000.0f;
static const float pdspTriggerOff = TriggerOff;


static const int Unchanged = 0;
static const int Changed = 1;
static const int AudioRate = 2;


static const int onceFFFF = 0;
static const int onceTFFF = 1;
static const int onceFTFF = 4;
static const int onceTTFF = 5;
static const int onceFFTF = 16;
static const int onceTFTF = 17;
static const int onceFTTF = 20;
static const int onceTTTF = 21;

static const int onceFFFT = 64;
static const int onceTFFT = 65;
static const int onceFTFT = 68;
static const int onceTTFT = 69;
static const int onceFFTT = 80;
static const int onceTFTT = 81;
static const int onceFTTT = 84;
static const int onceTTTT = 85;

static const int processOnceBitMask = 85;

static const int audioFFFF = 0;
static const int audioTFFF = 2;
static const int audioFTFF = 8;
static const int audioTTFF = 10;
static const int audioFFTF = 32;
static const int audioTFTF = 34;
static const int audioFTTF = 40;
static const int audioTTTF = 42;

static const int audioFFFT = 128;
static const int audioTFFT = 130;
static const int audioFTFT = 136;
static const int audioTTFT = 138;
static const int audioFFTT = 160;
static const int audioTFTT = 162;
static const int audioFTTT = 168;
static const int audioTTTT = 170;

static const int processAudioBitMask = 170;

    
    
}//END NAMESPACE


#endif  // PDSPCOSTANTS_H_INCLUDED
