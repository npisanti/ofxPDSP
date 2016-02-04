
#include "UpSampler.h"


pdsp::UpSampler::UpSampler(int inputOversample, int outputOversample) : input(inputOversample), output(outputOversample){
        
        addInput("signal", input);
        addOutput("signal", output);
        updateOutputNodes();    

        setOversampleLevels(inputOversample, outputOversample);           
}



void pdsp::UpSampler::setOversampleLevels(int inputOversample, int outputOversample){
        oversampleFactor = outputOversample / inputOversample;
        setOversampleLevel(inputOversample);
        output.setOversampleLevel(outputOversample);
        
}



void pdsp::UpSampler::process (int bufferSize) noexcept {
        
        int inputState;
        const float* inputBuffer = processInput(input, inputState);

        if(inputState == AudioRate){
                float* outputBuffer = getOutputBufferToFill(output);
                upsample(outputBuffer, inputBuffer, oversampleFactor, bufferSize);
        }else{
                setControlRateOutput(output, inputBuffer[0]);
        }    
}

