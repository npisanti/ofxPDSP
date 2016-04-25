
#include "SaturatorThreshold.h"

pdsp::SaturatorThreshold::SaturatorThreshold(){
    addInput("signal", input);
    addInput("threshold", input_threshold);
    addInput("return", input_return);
    addOutput("signal", output);
    addOutput("send", send);
    updateOutputNodes();
    

    input_threshold.setDefaultValue(0.5f);
    
    if(dynamicConstruction){
        prepareToPlay(globalBufferSize, globalSampleRate);
    }
}


pdsp::Patchable& pdsp::SaturatorThreshold::set(float threshold){
    input_threshold.setDefaultValue(threshold);
    return *this;
}


pdsp::Patchable& pdsp::SaturatorThreshold::in_signal(){
    return in("signal");
}

pdsp::Patchable& pdsp::SaturatorThreshold::in_threshold(){
    return in("threshold");
}

pdsp::Patchable& pdsp::SaturatorThreshold::in_return(){
    return in("return");
}

pdsp::Patchable& pdsp::SaturatorThreshold::out_signal(){
    return out("signal");
}

pdsp::Patchable& pdsp::SaturatorThreshold::out_send(){
    return out("send");
}

void pdsp::SaturatorThreshold::prepareUnit( int expectedBufferSize, double sampleRate ){
}

void pdsp::SaturatorThreshold::releaseResources () {    
}

void pdsp::SaturatorThreshold::process (int bufferSize) noexcept {
    
    int inputState;
    const float* inputBuffer = processInput(input, inputState);

    switch(inputState) {
        case Unchanged: case Changed:
            setOutputToZero(output);
            setOutputToZero(send);
            break;
        
        case AudioRate:
            int changed;
            const float* thresholdBuffer = processInput(input_threshold, changed);
            if(changed){ 
                threshold = thresholdBuffer[0]; 
                if(threshold > 0.99999f) threshold = 0.99999f;
                if(threshold < 0.0f) threshold = 0.0f;
            }
            
            float * sendBuffer = getOutputBufferToFill(send);
            float * outputBuffer = getOutputBufferToFill(output);
            
            
            // SPLIT PROCESSING
            
            int n=0;
            int maxSimd = ROUND_DOWN(bufferSize, 8);
                    
            float thresholdLo = - threshold;
            float thresholdScale = 1.0f / (1.0f - threshold);
            
            for(; n<maxSimd; n+=8){
                
                ofx::f128 x0 = ofx::m_load(inputBuffer + n);
                ofx::f128 x4 = ofx::m_load(inputBuffer + n + 4);
                ofx::f128 s0 = ofx::m_set1(0.0f);
                ofx::f128 s4 = ofx::m_set1(0.0f);
                
                ofx::f128 bitmask0 = ofx::m_cmp1_gt(x0, threshold);
                ofx::f128 bitmask4 = ofx::m_cmp1_gt(x4, threshold);
                ofx::f128 result0  = ofx::m_sub1(x0, threshold);
                ofx::f128 result4  = ofx::m_sub1(x4, threshold);
                result0            = ofx::m_and(result0, bitmask0);
                result4            = ofx::m_and(result4, bitmask4);
                
                s0                 = ofx::m_add(result0, s0); // now y0 has the positive thresholded values
                s0                 = ofx::m_add(result4, s4); // now y4 has the positive thresholded values
                
                bitmask0           = ofx::m_cmp1_lt(x0, thresholdLo);
                bitmask4           = ofx::m_cmp1_lt(x4, thresholdLo);
                result0            = ofx::m_add1(x0, threshold);
                result4            = ofx::m_add1(x4, threshold);
                result0            = ofx::m_and(result0, bitmask0);
                result4            = ofx::m_and(result4, bitmask4);
                                
                s0                 = ofx::m_add(result0, s0); // now y0 has the positive and negative thresholded values
                s4                 = ofx::m_add(result4, s4); // now y4 has the positive and negative thresholded values
                
                x0                 = ofx::m_sub(x0, s0); // now x0 has the input minus the signal to saturate
                x4                 = ofx::m_sub(x4, s4); // now x4 has the input minus the signal to saturate
                
                s0                 = ofx::m_mul1(s0, thresholdScale); // rescale the signal
                s4                 = ofx::m_mul1(s4, thresholdScale); // rescale the signal
                
                ofx::m_store(outputBuffer + n,     x0);
                ofx::m_store(outputBuffer + n + 4, x4);
                ofx::m_store(  sendBuffer + n,     s0);
                ofx::m_store(  sendBuffer + n + 4, s4);
                
            }

            for(; n<bufferSize; ++n){
                
                float s = 0.0f;
                if(inputBuffer[n] > threshold){
                    s = inputBuffer[n] - threshold;
                }else if (inputBuffer[n] < thresholdLo){
                    s = inputBuffer[n] + threshold;
                }
                outputBuffer[n] = inputBuffer[n] - s;
                
                sendBuffer[n] = s*thresholdScale;
            }

            
            // PROCESS SATURATOR
            int state;
            const float* returnBuffer = processInput(input_return, state); // now return buffer has the processed signal
            float rescale = 1.0f - threshold;
            
               
            // SUM RETURN TO OUTPUT
            // multiply return for (1.0-threshold)
            // add return to output
            ofx_Aeq_Badd_CmulS(outputBuffer, outputBuffer, returnBuffer, rescale, bufferSize);
                        
            //END
            break;
    }


}


