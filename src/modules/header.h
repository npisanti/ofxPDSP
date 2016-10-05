
// modules/header.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_MODULEHEADER_H_INCLUDED
#define PDSP_MODULEHEADER_H_INCLUDED

#include "dynamics/Compressor1.h"
#include "dynamics/Compressor2.h"

#include "filters/VAFilter.h"

#include "oscillators/VAOscillator.h"
#include "oscillators/LFO.h"
#include "oscillators/ClockedLFO.h"
#include "oscillators/FMOperator.h"
#include "oscillators/TableOscillator.h"

#include "samplers/GrainCloud.h"
#include "samplers/TriggeredGrain.h"

#include "sources/BitNoise.h"

#include "signal/LinearCrossfader.h"
#include "signal/Panner.h"
#include "signal/KneeClipper.h"

#include "effects/CombFilter.h"
#include "effects/DimensionChorus.h"
#include "effects/StereoConvolver.h"

#include "equalizers/AAHighShelfEQ.h"
#include "equalizers/AALowShelfEQ.h"
#include "equalizers/AAPeakEQ.h"
#include "equalizers/HighShelfEQ.h"
#include "equalizers/LowShelfEQ.h"
#include "equalizers/PeakEQ.h"
#include "equalizers/LowCut.h"
#include "equalizers/HighCut.h"

#include "reverbs/BasiVerb.h"
#include "reverbs/IRVerb.h"


#endif // PDSP_MODULEHEADER_H_INCLUDED
