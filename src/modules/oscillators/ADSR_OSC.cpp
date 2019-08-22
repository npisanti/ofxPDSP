#include "ADSR_OSC.h"

pdsp::ADSR_OSC::ADSR_OSC()
{
  addModuleInput("trigger", trigger); // first = default
  addModuleInput("pitch", pitch);
  addModuleInput("pw", osc.in_pw());
  addModuleInput("waveForm", waveForm.in_select());

  addModuleOutput("signal", amp); // first = default

  detuneCoarse.set(0);
  detuneFine.set(0);

  pitch + detuneCoarse + detuneFine >> osc.in_pitch();
  //pitch  >> osc.in_pitch();

  waveForm.resize(4);
  osc.out_sine() >> waveForm.input(0);
  osc.out_triangle() >> waveForm.input(1);
  osc.out_saw() >> waveForm.input(2);
  osc.out_pulse() >> waveForm.input(3);

  0.0f >> waveForm.in_select();

  trigger >> adsr >> amp.in_mod();
  waveForm >> amp;
}