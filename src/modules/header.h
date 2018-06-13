
// modules/header.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_MODULEHEADER_H_INCLUDED
#define PDSP_MODULEHEADER_H_INCLUDED

#include "dynamics/Compressor.h"
#include "dynamics/Ducker.h"

#include "filters/VAFilter.h"
#include "filters/CombFilter.h"

#include "oscillators/VAOscillator.h"
#include "oscillators/LFO.h"
#include "oscillators/ClockedLFO.h"
#include "oscillators/FMOperator.h"
#include "oscillators/TableOscillator.h"
#include "oscillators/DataOscillator.h"

#include "samplers/GrainCloud.h"
#include "samplers/TriggeredGrain.h"

#include "sources/BitNoise.h"

#include "signal/LinearCrossfader.h"
#include "signal/Panner.h"

#include "effects/DimensionChorus.h"

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
