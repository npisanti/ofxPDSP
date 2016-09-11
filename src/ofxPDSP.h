
// ofxPDSP.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef OFXPDSP_H_INCLUDED
#define OFXPDSP_H_INCLUDED

#include "math/header.h"
#include "messages/header.h"
#include "DSP/header.h"
#include "sequencer/header.h"
#include "modules/header.h"

#include "ofx/ofxPDSPMidi.h"

#include "ofx/ofxPDSPComputerKeyboard.h"

#include "ofx/ofxPDSPSerialOut.h"

#include "ofx/ofxPDSPParameters.h"
#include "ofx/ofxPDSPValue.h"

#include "ofx/ofxSampleBufferPlotter.h"
#include "ofx/ofxScoreSectionPlotter.h"
#include "ofx/ofxPDSPEngineGraphics.h"

#include "ofx/ofxPDSPFunctions.h"

#include "ofx/ofxPDSPEngine.h"


// remember to delete those in a future major release
[[deprecated("deprecated for a less ambigous nomenclature, use ofxPDSPMidiIn instead, (you are currently using just a typedef to the same class) ")]]
typedef ofxPDSPMidiIn ofxMidiInProcessor;
[[deprecated("deprecated for a less ambigous nomenclature, use ofxPDSPMidiKeys instead, (you are currently using just a typedef to the same class) ")]]
typedef ofxPDSPMidiKeys ofxMidiKeysProcessor;
[[deprecated("deprecated for a less ambigous nomenclature, use ofxPDSPMidiPads instead, (you are currently using just a typedef to the same class) ")]]
typedef ofxPDSPMidiPads ofxMidiDrumKitProcessor;
[[deprecated("deprecated for a less ambigous nomenclature, use ofxPDSPMidiControls instead, (you are currently using just a typedef to the same class) ")]]
typedef ofxPDSPMidiControls  ofxMidiCCProcessor;
[[deprecated("deprecated for a less ambigous nomenclature, use ofxPDSPMidiOut instead, (you are currently using just a typedef to the same class) ")]]
typedef ofxPDSPMidiOut ofxMidiOutProcessor;


#endif
