
// types.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_TYPES_H_INCLUDED
#define PDSP_TYPES_H_INCLUDED

#include <limits>

namespace pdsp{

enum Window_t { Rectangular, Triangular, Hann, Hamming, Blackman, BlackmanHarris, SineWindow, Welch };

enum Interpolator_t {Linear, Smooth};

enum SlewMode_t {Rate, Time};

static const float TriggerOff = - std::numeric_limits<float>::infinity();

} // end namespace

#endif // PDSP_TYPES_H_INCLUDED
