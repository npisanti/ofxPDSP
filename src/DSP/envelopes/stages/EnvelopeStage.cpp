
#include "EnvelopeStage.h"


const float pdsp::EnvelopeStage::digitalAttackTCO = 0.99999f;
const float pdsp::EnvelopeStage::analogAttackTCO  = exp( -1.5 );

const float pdsp::EnvelopeStage::digitalDecayTCO  = exp(-11.05); 
const float pdsp::EnvelopeStage::analogDecayTCO   = exp( -4.95); 
