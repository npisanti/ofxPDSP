
// DSP/header.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_DSP_HEADER_H_INCLUDED
#define PDSP_DSP_HEADER_H_INCLUDED


#include "pdspConstants.h"

#include "core/BasicNodes.h"
#include "core/PatchNode.h"
#include "core/Processor.h"
#include "core/leftSum.h"
#include "core/operators.h"
#include "core/Formula.h"
#include "core/Amp.h"
#include "core/ExternalInput.h"

#include "signal/Saturator1.h"
#include "signal/Saturator2.h"
#include "signal/SoftClip.h"
#include "signal/SampleAndHold.h"
#include "signal/Bitcruncher.h"
#include "signal/Decimator.h"

#include "control/ToGateTrigger.h"
#include "control/SequencerGateOutput.h"
#include "control/SequencerValueOutput.h"
#include "control/TriggerGeiger.h"
#include "control/TriggerControl.h"
#include "control/ValueControl.h"

#include "dynamics/EnvelopeFollower.h"
#include "dynamics/PositiveValue.h"
#include "dynamics/AbsoluteValue.h"
#include "dynamics/GainComputer.h"
#include "dynamics/RMSDetector.h"
#include "dynamics/SquarePeakDetector.h"

#include "oscillators/phasors/PhasorShifter.h"
#include "oscillators/phasors/ClockedPhasor.h"
#include "oscillators/phasors/PMPhasor.h"
#include "oscillators/phasors/LFOPhasor.h"

#include "oscillators/cheap/CheapTri.h"
#include "oscillators/cheap/CheapSaw.h"
#include "oscillators/cheap/CheapPulse.h"
#include "oscillators/cheap/CheapSine.h"

#include "oscillators/antialiased/DPWTri.h"
#include "oscillators/antialiased/BLEPSaw.h"

#include "oscillators/wavetable/WaveTableOsc.h"
#include "oscillators/wavetable/DataOsc.h"

#include "random/WhiteNoise.h"
#include "random/TriggeredRandom.h"

#include "filters/OnePole.h"
#include "filters/MultiLadder4.h"
#include "filters/SVF2.h"
#include "filters/APF4.h"
#include "filters/APF1.h"

#include "filters/biquads/BiquadLPF2.h"
#include "filters/biquads/BiquadHPF2.h"
#include "filters/biquads/BiquadBPF2.h"
#include "filters/biquads/BiquadNotch2.h"
#include "filters/biquads/BiquadAPF2.h"
#include "filters/biquads/BiquadLowShelf.h"
#include "filters/biquads/BiquadHighShelf.h"
#include "filters/biquads/BiquadPeakEQ.h"

#include "samplers/SampleBuffer.h"
#include "samplers/Sampler.h"
#include "samplers/GrainWindow.h"

#include "oscillators/fm/SineFB.h"

#include "delays/SRDelay.h"
#include "delays/Delay.h"
#include "delays/AllPassDelay.h"

#include "envelopes/ADSR.h"
#include "envelopes/AHR.h"

#include "utility/OneMinusInput.h"
#include "utility/LinToDB.h"
#include "utility/DBtoLin.h"
#include "utility/PitchToFreq.h"
#include "utility/BipolarToUnipolar.h"
#include "utility/MaxValue.h"
#include "utility/OneBarTimeMs.h"
#include "utility/FreqToMs.h"
#include "utility/SamplesDelay.h"

#include "convolution/FDLConvolver.h"

#include "resamplers/resamplers.h"

namespace pdsp{

        typedef DPWTri AATri;

        typedef BLEPSaw AASaw;

}



#endif  // PDSP_DSP_HEADER_H_INCLUDED
