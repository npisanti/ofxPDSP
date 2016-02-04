
// math/header.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_MATH_HEADER_H_INCLUDED
#define PDSP_MATH_HEADER_H_INCLUDED

#include "../types.h"

#include "functions.h"


#include "trig/fastsin.h"
#include "trig/fastcos.h"
#include "trig/fasttan.h"
#include "trig/fasttanh.h"

#include "interpolation/linear.h"
#include "interpolation/cosine.h"
#include "interpolation/smooth.h"
#include "interpolation/hermite.h"

#include "dsphelpers/nonlinear1.h"
#include "dsphelpers/phazorShifter.h"
#include "dsphelpers/warpCutoff.h"
#include "dsphelpers/incrementCalculator.h"
#include "dsphelpers/genPulse.h"
#include "dsphelpers/genSaw.h"
#include "dsphelpers/genTriangle.h"
#include "dsphelpers/calculateGainReduction.h"

#include "tables/dsp_windows.h"
#include "tables/blep.h"

#include "random/random.h"

#include "fft/include_audiofft.h"
#include "fft/complex_mult_add.h"


#endif  // PDSP_MATH_HEADER_H_INCLUDED
