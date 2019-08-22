#pragma once

#include "ofxPDSP.h"
#include "ofMain.h"

namespace pdsp {

  /*
  Oscillator with:
  - associated ADSR via adsr
  - detune coarse and fine via detuneCoarse and detuneFine
  - waveform selection via waveForm,  0 sine, 1 triangle 2 saw, 3 pulse
  - pulse width via in("pw")


  */

  class ADSR_OSC : public pdsp::Patchable
  {
  public:
    ADSR_OSC();


    pdsp::VAOscillator    osc;
    pdsp::ADSR            adsr;
    pdsp::Switch          waveForm;
    pdsp::Amp             amp;
    pdsp::PatchNode       detuneCoarse;
    pdsp::PatchNode       detuneFine;

  private:
    pdsp::PatchNode       trigger;
    pdsp::PatchNode       pitch;

  };

}
