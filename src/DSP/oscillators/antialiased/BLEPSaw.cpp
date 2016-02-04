
#include "BLEPSaw.h"


pdsp::BLEPSaw::BLEPSaw(Window_t window, bool eight, int length, bool interpolate){
    setTable(window, eight, length, interpolate);
    addInput("inc", input_inc);
    updateOutputNodes();
}

pdsp::BLEPSaw::BLEPSaw() : BLEPSaw(Rectangular, false, 4096, true){};

pdsp::Patchable& pdsp::BLEPSaw::in_inc(){
    return in("inc");
}

void pdsp::BLEPSaw::prepareOscillator( double sampleRate){   }

void pdsp::BLEPSaw::releaseOscillator() {   }

void pdsp::BLEPSaw::oscillate( float* outputBuffer, const float* phaseBuffer, int bufferSize) noexcept {
    //TRIVIAL SAW

    genSaw(outputBuffer, phaseBuffer, bufferSize);
    //trivialSawDSP(bufferSize, outputBuffer, phaseBuffer);


    //BLEP CORRECTION
    int incBufferState;
    const float* incBuffer = processInput(input_inc, incBufferState);

    //float tableCenter = blepTable->length_f * 0.5 - 1.0;
    //Logger::writeToLog(String(tableCenter));


    if (incBufferState == AudioRate){ //phase increment is always changing

            BLEPFMWaveformDSP(bufferSize, outputBuffer, phaseBuffer, incBuffer,
                              blepTable->buffer, false, blepTable->points, blepTable->length_f, interpolateBLEP);

    }else { //phase increment is always the same

            BLEPWaveformDSP(bufferSize, outputBuffer, phaseBuffer, incBuffer[0],
                            blepTable->buffer, false, blepTable->points, blepTable->length_f, interpolateBLEP);

    }

}

