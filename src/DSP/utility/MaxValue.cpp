
#include "MaxValue.h"


pdsp::MaxValue2::MaxValue2(){
    addInput("0", input1);
    addInput("1", input2);
    addOutput("signal", output);
    updateOutputNodes();
    
    input1.setDefaultValue(-32000.0f);
    input2.setDefaultValue(-32000.0f);

    if(dynamicConstruction){
        prepareToPlay(globalBufferSize, globalSampleRate);
    }    
}

pdsp::Patchable& pdsp::MaxValue2::in_0(){
    return in("0");
}

pdsp::Patchable& pdsp::MaxValue2::in_1(){
    return in("1");
}

pdsp::Patchable& pdsp::MaxValue2::out_signal(){
    return out("signal");
}

void pdsp::MaxValue2::prepareUnit( int expectedBufferSize, double sampleRate ) {}

void pdsp::MaxValue2::releaseResources () {}

void pdsp::MaxValue2::process (int bufferSize) noexcept {
    
    int input1State;
    const float * buffer1 = processInput(input1, input1State);
    int input2State;
    const float * buffer2 = processInput(input2, input2State);    

    int switcher = input1State + input2State*4;

    //process once
    switch (switcher & 42) {
    case 0 :
    {
        float max;
        if(buffer1[0] > buffer2[0]){
            max = buffer1[0];
        }else{
            max = buffer2[0];
        }
        setControlRateOutput(output, max);
    }
        break;
    case 2 : //input1 is audioRate 
    {
        float * outputBuffer = getOutputBufferToFill(output);
        ofx_Aeq_maxBS(outputBuffer, buffer1, buffer2[0], bufferSize);
    }
        break;
    case 8 :
    {
        float * outputBuffer = getOutputBufferToFill(output);
        ofx_Aeq_maxBS(outputBuffer, buffer2, buffer1[0], bufferSize);
    }
        break;
    case 10 : 
    {
        float * outputBuffer = getOutputBufferToFill(output);
        ofx_Aeq_maxBC(outputBuffer, buffer1, buffer2, bufferSize);
    }  
        break;
    default:
        break;
    }

}

