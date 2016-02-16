
// flags.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_FLAGS_H_INCLUDED
#define PDSP_FLAGS_H_INCLUDED

// comment it if you don't want direct file loading feature
// or maybe define it if you are able to include libsndfile in android in some way 
// in linux libsndfile is included anyway, so don't worry about this
    // #define PDSP_USE_LIBSNDFILE


//if you are on osx/ios, you can add the accelerate framework and decomment this for faster FFT
    //#define AUDIOFFT_APPLE_ACCELERATE

//if you can use FFTW for your project, link it and decomment this for faster FFT
    //#define AUDIOFFT_FFTW3


// some internally used values
#define PDSP_NODE_POINTERS_RESERVE 16
#define PDSP_PATTERN_MESSAGE_RESERVE_DEFAULT 32
#define PDSP_SCORESECTIONMESSAGERESERVE 32

#define PDSP_BUFFERS_EXTRA_DIM  5

#define PDSP_MIN_ENVSTAGE_MS 0.000001f

#endif // PDSP_FLAGS_H_INCLUDED
