
#include "MultiLadder4.h"

pdsp::MultiLadder4::MultiLadder4(){
    	    
        addInput("signal", input_signal);
        addInput("cutoff", input_cutoff);
        addInput("reso", input_reso);
        addOutput("lpf4", output_lpf4);
        addOutput("lpf2", output_lpf2);
        addOutput("bpf4", output_bpf4);
        addOutput("bpf2", output_bpf2);
        addOutput("hpf4", output_hpf4);
        addOutput("hpf2", output_hpf2);
        updateOutputNodes();

        input_cutoff.setDefaultValue(8000.0f);
        input_reso.setDefaultValue(0.0f);
        input_cutoff.enableBoundaries( 30.0f, 20000.0f);
        input_reso.enableBoundaries( 0.0f, 1.0f);
        if(dynamicConstruction){
                prepareToPlay(globalBufferSize, globalSampleRate);
        }
}

pdsp::Patchable& pdsp::MultiLadder4::in_signal(){
    return in("signal");
}

pdsp::Patchable& pdsp::MultiLadder4::in_cutoff(){
    return in("cutoff");
}

pdsp::Patchable& pdsp::MultiLadder4::in_reso(){
    return in("reso");
}

pdsp::Patchable& pdsp::MultiLadder4::out_lpf4(){
    return out("lpf4");
}

pdsp::Patchable& pdsp::MultiLadder4::out_lpf2(){
    return out("lpf2");
}

pdsp::Patchable& pdsp::MultiLadder4::out_bpf4(){
    return out("bpf4");
}

pdsp::Patchable& pdsp::MultiLadder4::out_bpf2(){
    return out("bpf2");
}

pdsp::Patchable& pdsp::MultiLadder4::out_hpf4(){
    return out("hpf4");
}

pdsp::Patchable& pdsp::MultiLadder4::out_hpf2(){
    return out("hpf2");
}


void pdsp::MultiLadder4::prepareUnit( int expectedBufferSize, double sampleRate ){
        z1_1 = 0.0f;
        z1_2 = 0.0f;
        z1_3 = 0.0f;
        z1_4 = 0.0f;
        
        K = 0.0f;
        
        halfT = 0.5/ sampleRate;
        twoSlashT = 1.0 / halfT;
}

void pdsp::MultiLadder4::releaseResources(){
    
}


void pdsp::MultiLadder4::process( int bufferSize ) noexcept {

        int inputBufferState;
        const float* inputBuffer = processInput(input_signal, inputBufferState);

        if(inputBufferState==AudioRate){
                
                int cutoffBufferState;
                const float* cutoffBuffer = processInput(input_cutoff, cutoffBufferState);
                int resoBufferState;
                const float* resoBuffer = processInput(input_reso, resoBufferState);

                int switcher = cutoffBufferState + resoBufferState*4;

                //process once
                switch (switcher & 21) {
                case 0 : //unchanged, unchanged, unchanged
                        process_once<false, false>( cutoffBuffer, resoBuffer );
                        break;
                case 1 : //changed, unchanged, unchanged
                        process_once<true, false>( cutoffBuffer, resoBuffer );
                        break;
                case 4 : //unchanged, changed, unchanged
                        process_once<false, true>( cutoffBuffer, resoBuffer );
                        break;
                case 5 : //changed, changed, unchanged
                        process_once<true, true>( cutoffBuffer, resoBuffer );
                        break;
                default:
                        break;
                }
                    
                //process audio rate
                switch ( switcher & 42 ) {
                case 0 : //unchanged, unchanged, unchanged
                        process_audio<false, false>(inputBuffer, cutoffBuffer, resoBuffer, bufferSize);
                        break;
                case 2 : //changed, unchanged, unchanged
                        process_audio<true, false>(inputBuffer, cutoffBuffer, resoBuffer, bufferSize);
                        break;
                case 8 : //unchanged, changed, unchanged
                        process_audio<false, true>(inputBuffer, cutoffBuffer, resoBuffer, bufferSize);
                        break;
                case 10 : //changed, changed, unchanged
                        process_audio<true, true>(inputBuffer, cutoffBuffer, resoBuffer, bufferSize);
                        break;
                default:
                        break;
                }
        }else{
                setOutputToZero(output_lpf4);
                setOutputToZero(output_lpf2);
                setOutputToZero(output_bpf4);
                setOutputToZero(output_bpf2);
                setOutputToZero(output_hpf4);
                setOutputToZero(output_hpf2);
        }
}


inline_f void pdsp::MultiLadder4::coefficientCalculation(float wa){
        //coefficient calculation
        g = wa * halfT;
        alpha = g / (1.0f + g);
        beta4 = 1.0f / (1.0f + g);
        beta3 = beta4 * alpha;
        beta2 = beta3 * alpha;
        beta1 = beta2 * alpha;
        gamma = alpha * alpha * alpha * alpha;  //interleaved operation for faster calculations
}

inline_f void pdsp::MultiLadder4::calculateAlphaZero(float reso){
        alpha0 = 1.0f / ( 1.0f  + (gamma*reso*4.0f) );  //reso dependent
}


template<bool cutoffChange, bool resoChange>
void pdsp::MultiLadder4::process_once(const float* cutoffBuffer, const float* resoBuffer)noexcept{

        if (cutoffChange) {
                float wa;
                //vect_pitchToFreq(&wa, cutoffBuffer, 1);
                vect_warpCutoff(&wa, cutoffBuffer, halfT, twoSlashT, 1);//cutoff warping
                
                coefficientCalculation(wa);
                calculateAlphaZero(K);

        }else if (resoChange){
                K = resoBuffer[0] * 4.0f;
                calculateAlphaZero(K);
        }
        
}


template<bool cutoffAR, bool resoAR>
void pdsp::MultiLadder4::process_audio(const float* inputBuffer, const float* cutoffBuffer, const float* resoBuffer, int bufferSize)noexcept{

        float* outputLPF4 = getOutputBufferToFill(output_lpf4);
        float* outputLPF2 = getOutputBufferToFill(output_lpf2);
        float* outputBPF4 = getOutputBufferToFill(output_bpf4);
        float* outputBPF2 = getOutputBufferToFill(output_bpf2);
        float* outputHPF4 = getOutputBufferToFill(output_hpf4);
        float* outputHPF2 = getOutputBufferToFill(output_hpf2);

        if (cutoffAR) {
                vect_warpCutoff(outputLPF4, cutoffBuffer, halfT, twoSlashT, bufferSize);//cutoff warping, now outputLPF4 contain wa
        }

        for (int n=0; n<bufferSize; ++n){

                if (cutoffAR==true) {
                        coefficientCalculation(outputLPF4[n]); //outputBuffer cointain wa
                        if (!resoAR) {
                                calculateAlphaZero(K);
                        }
                }
                
                if (resoAR) {
                        K = resoBuffer[n]*4.0f;
                        calculateAlphaZero(K);
                }
                
                
                //FILTER CALCULATIONS
                //input with feedback
                float sigma = z1_1*beta1 + z1_2*beta2 + z1_3*beta3 + z1_4*beta4;
                float u;
                
                u = (inputBuffer[n] - K*sigma) * alpha0; 
                /*
                if(resoAR){ // reso dependent
                        // u = inputBuffer[n]  * (1.0f+resoBuffer[n]); // decomment for passband gain compensation
                        // u = (u - resoBuffer[n]*sigma) * alpha0; // decomment for passband gain compensation, and comment next
                        u = (inputBuffer[n] - resoBuffer[n]*sigma) * alpha0; 
                }else{
                        // u = inputBuffer[n]   * (1.0f+resoBuffer[0]); // decomment for passband gain compensation
                        // u = (u - resoBuffer[0]*sigma) * alpha0; // decomment for passband gain compensation, and comment next
                        u = (inputBuffer[n] - resoBuffer[0]*sigma) * alpha0; 
                }
                */ 
                
                
                //first stage
                float vn = (u - z1_1) * alpha;
                float lpf1 = vn + z1_1;
                z1_1 = vn + lpf1;
                //second stage
                vn = (lpf1 - z1_2) * alpha;
                float lpf2 = vn + z1_2;
                z1_2 = vn + lpf2;
                //third stage
                vn = (lpf2 - z1_3) * alpha;
                float lpf3 = vn + z1_3;
                z1_3 = vn + lpf3;
                //fourth stage
                vn = (lpf3 - z1_4) * alpha;
                float lpf4 = vn + z1_4;
                z1_4 = vn + lpf4;
                
                outputLPF2[n] = lpf2;
                outputLPF4[n] = lpf4;
                outputBPF2[n] = lpf1 * 2.0f + lpf2 * -2.0f;
                outputBPF4[n] = lpf2 * 4.0f + lpf3 * -8.0f + lpf4 * 4.0f;
                outputHPF2[n] = u + (lpf1 * -2.0f) + lpf2;
                outputHPF4[n] = u + (lpf1 * -4.0f) + (lpf2 * 6.0f) + (lpf3 * -4.0f) + lpf4;
        }

    
}
