
#include "PhasorShifter.h"


pdsp::PhasorShifter::PhasorShifter(){
        addOutput("phase", output);
        addInput("phase", input_phase);
        addInput("shift", input_shift);
        updateOutputNodes();
        
        if(dynamicConstruction){
                prepareToPlay(globalBufferSize, globalSampleRate);
        }

}

pdsp::Patchable& pdsp::PhasorShifter::set(float phaseOffset){
    input_shift.setDefaultValue(phaseOffset);
    return *this;
}

pdsp::Patchable& pdsp::PhasorShifter::in_phase(){
    return in("phase");
}

pdsp::Patchable& pdsp::PhasorShifter::out_phase(){
    return out("phase");
}

pdsp::Patchable& pdsp::PhasorShifter::in_shift(){
    return in("shift");
}

void pdsp::PhasorShifter::prepareUnit ( int expectedBufferSize, double sampleRate ) {}

void pdsp::PhasorShifter::releaseResources () { }


void pdsp::PhasorShifter::process(int bufferSize) noexcept {

        int phaseBufferState;
        const float* phaseBuffer = processInput(input_phase, phaseBufferState);

        switch(phaseBufferState){
            case Unchanged: case Changed:
                setOutputToZero(output);
            break;
            
            case AudioRate:
            {
                int shiftBufferState;
                const float* shiftBuffer = processInput(input_shift, shiftBufferState);

                float* outputBuffer = getOutputBufferToFill(output);

                if(shiftBufferState==AudioRate){
                        vect_phazorShiftB(outputBuffer, phaseBuffer, shiftBuffer, bufferSize);
                }else{
                        vect_phazorShiftS(outputBuffer, phaseBuffer, shiftBuffer[0], bufferSize);
                }
            }
            break;
            
            default: break;
        }

}



