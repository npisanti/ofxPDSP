
#include "DownSampler.h"


pdsp::DownSampler::DownSampler(int inputOversample, int outputOversample) : input(inputOversample), output(outputOversample){
        
        addInput("signal", input);
        addOutput("signal", output);
        updateOutputNodes();    

        setOversampleLevels(inputOversample, outputOversample);           
}



void pdsp::DownSampler::setOversampleLevels(int inputOversample, int outputOversample){
        oversampleFactor = inputOversample / outputOversample;
        setOversampleLevel(outputOversample);
        input.setRequiredOversampleLevel(inputOversample);
        
}



void pdsp::DownSampler::process (int bufferSize) noexcept {
        
        int inputState;
        const float* inputBuffer = processInput(input, inputState);

        if(inputState == AudioRate){
                float* outputBuffer = getOutputBufferToFill(output);
                downsample(outputBuffer, inputBuffer, oversampleFactor, bufferSize);
        }else{
                setControlRateOutput(output, inputBuffer[0]);
        }
    
}

