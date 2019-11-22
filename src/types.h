
// types.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_TYPES_H_INCLUDED
#define PDSP_TYPES_H_INCLUDED

#include <cfloat>

namespace pdsp{

enum Window_t { Rectangular, Triangular, Hann, Hamming, Blackman, BlackmanHarris, SineWindow, Welch, Gaussian, Tukey };

enum Interpolator_t {Linear, Smooth};

enum SlewMode_t {Rate, Time};

#define PDSP_TRIGGER_OFF -FLT_MAX

} // end namespace

#endif // PDSP_TYPES_H_INCLUDED
