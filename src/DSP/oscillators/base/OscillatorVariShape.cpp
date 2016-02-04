

#include "OscillatorVariShape.h"


pdsp::OscillatorVariShape::OscillatorVariShape(){

    addInput("phase", input_phase);
    addInput("shape", input_shape);
    addOutput("signal", output);
    updateOutputNodes();
    
    input_shape.setDefaultValue( 0.0f );
    
    if(dynamicConstruction){
            prepareToPlay(globalBufferSize, globalSampleRate);
    }
}

float pdsp::OscillatorVariShape::meter_output() const{
    return meter.load();
}


pdsp::Patchable& pdsp::OscillatorVariShape::in_phase(){
    return in("phase");
}

pdsp::Patchable& pdsp::OscillatorVariShape::in_shape(){
    return in("shape");
}

pdsp::Patchable& pdsp::OscillatorVariShape::out_signal(){
        return out("signal");
}

void pdsp::OscillatorVariShape::prepareOscillator(double sampleRate){}
void pdsp::OscillatorVariShape::releaseOscillator(){}


void pdsp::OscillatorVariShape::prepareUnit( int expectedBufferSize, double sampleRate) {
        prepareOscillator(sampleRate);
}

void pdsp::OscillatorVariShape::releaseResources() {
        releaseOscillator();
}


void pdsp::OscillatorVariShape::process(int bufferSize) noexcept {
    
        int phaseBufferState;
        const float* phaseBuffer = processInput(input_phase, phaseBufferState);

        if (phaseBufferState == AudioRate){

                int shapeBufferState;
                const float* shapeBuffer = processInput(input_shape, shapeBufferState);
                
                float* outputBuffer = getOutputBufferToFill(output);
                
                if(shapeBufferState==AudioRate){
                        //----------------oscillator DSP-----------------------
                        oscillateShapeAR(outputBuffer, phaseBuffer, shapeBuffer, bufferSize);
                }else{
                        //----------------oscillator DSP-----------------------
                        oscillateShapeCR(outputBuffer, phaseBuffer, shapeBuffer[0], bufferSize);
                }
                meter.store(outputBuffer[0]);
        }else{
                setOutputToZero(output);
                meter.store(0.0f);
        }
}
