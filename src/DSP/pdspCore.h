
// pdspCore.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSPCOREUNITS_H_INCLUDED
#define PDSPCOREUNITS_H_INCLUDED


#ifdef _MSC_VER
	#pragma warning(disable : 4305) //double to float
	#pragma warning(disable : 4244) 
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdint.h>
#include <assert.h>
#include <cstring>
#include <atomic>
#include "pdspFunctions.h"
#include "pdspConstants.h"
#include "core/BasicNodes.h"
#include "core/PatchNode.h"
#include "core/Preparable.h"
#include "core/PatchNode.h"
#include "core/Switch.h"
#include "core/leftSum.h"
#include "core/operators.h"
#include "core/BufferShell.h"
#include "core/ExternalInput.h"
#include "core/Processor.h"
#include "core/Formula.h"
#include "core/Amp.h"

#include "../math/header.h"
#include "../messages/header.h"

#endif  // PDSPCOREUNITS_H_INCLUDED
