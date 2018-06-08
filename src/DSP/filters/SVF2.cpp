
#include "SVF2.h"

pdsp::SVF2::SVF2(){
    
        addInput("signal", input_signal);
        addInput("cutoff", input_cutoff);
        addInput("reso", input_reso);
        addOutput("lpf", output_lpf);
        addOutput("hpf", output_hpf);
        addOutput("bpf", output_bpf);
        addOutput("notch", output_bsf);
        updateOutputNodes();
        
        input_cutoff.enableBoundaries( 20.0f, 20000.0f);
        input_cutoff.setDefaultValue(8000.0f);
        input_reso.setDefaultValue(0.0f);
        input_reso.enableBoundaries( 0.0f, 1.0f);
        
        alpha0 = 1.0f;
        alpha = 1.0f;
        rho = 1.0f;
        z1_1 = 0.0f;
        z1_2 = 0.0f;

        if(dynamicConstruction){
                prepareToPlay(globalBufferSize, globalSampleRate);
        }
}


pdsp::Patchable& pdsp::SVF2::in_signal(){
    return in("signal");
}

pdsp::Patchable& pdsp::SVF2::in_cutoff(){
    return in("cutoff");
}

pdsp::Patchable& pdsp::SVF2::in_reso(){
    return in("reso");
}


pdsp::Patchable& pdsp::SVF2::out_lpf(){
    return out("lpf");
}

pdsp::Patchable& pdsp::SVF2::out_hpf(){
    return out("hpf");
}

pdsp::Patchable& pdsp::SVF2::out_bpf(){
    return out("bpf");
}

pdsp::Patchable& pdsp::SVF2::out_notch(){
    return out("notch");
}
        
        
void pdsp::SVF2::prepareUnit( int expectedBufferSize, double sampleRate ){

        z1_1 = 0.0f;
        z1_2 = 0.0f;

        halfT =  0.5f / sampleRate ;
        twoSlashT = 1.0f / halfT;
    
}

void pdsp::SVF2::releaseResources(){
    
}


void pdsp::SVF2::process(int bufferSize) noexcept {
        
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
                case 0:
                        process_once<false, false>(cutoffBuffer, resoBuffer);
                                break;
                case 1:
                        process_once<true, false>(cutoffBuffer, resoBuffer);
                                break;
                case 4:
                        process_once<false, true>(cutoffBuffer, resoBuffer);
                                break;
                case 5:
                        process_once<true, true>(cutoffBuffer, resoBuffer);
                                break;
                }

                //process audio rate
                switch ( switcher & 42 ) {
                case 0:
                        process_audio<false, false>(inputBuffer, cutoffBuffer, resoBuffer, bufferSize);
                        break;
                case 2:
                        process_audio<true, false>(inputBuffer, cutoffBuffer, resoBuffer, bufferSize);
                        break;
                case 8:
                        process_audio<false, true>(inputBuffer, cutoffBuffer, resoBuffer, bufferSize);
                        break;
                case 10:
                        process_audio<true, true>(inputBuffer, cutoffBuffer, resoBuffer, bufferSize);
                        break;
                }
        }else{
                setOutputToZero(output_hpf);
                setOutputToZero(output_lpf);
                setOutputToZero(output_bpf);
                setOutputToZero(output_bsf);
        }
    
}



template<bool cutoffChange, bool resoChange>
void pdsp::SVF2::process_once(const float* &cutoffBuffer, const float* &resoBuffer)noexcept{

        if(cutoffChange){
                float wa;
                //vect_pitchToFreq(&wa, cutoffBuffer, 1);
                vect_warpCutoff(&wa, cutoffBuffer, halfT, twoSlashT, 1);//cutoff warping
                g = wa * halfT;
        }

        if(resoChange){
                Q = resoBuffer[0]*24.5f + 0.5f;
        }

        if(cutoffChange || resoChange){
                R = 1.0f/(2.0f*Q);
                alpha0 = 1.0f/(1.0f + 2.0f*R*g + g*g);
                alpha = g;
                rho = 2.0f*R + g;
        }

}

  
template<bool cutoffAR, bool resoAR>
void pdsp::SVF2::process_audio(const float* inputBuffer, const float* cutoffBuffer, const float* &resoBuffer, const int &bufferSize)noexcept{
   
        float* hpfBuffer = getOutputBufferToFill(output_hpf);
        float* lpfBuffer = getOutputBufferToFill(output_lpf);
        float* bpfBuffer = getOutputBufferToFill(output_bpf);
        float* bsfBuffer = getOutputBufferToFill(output_bsf);

        if(cutoffAR){
                //vect_pitchToFreq(lpfBuffer, cutoffBuffer, bufferSize);//we use the output buffer as temporary storage
                vect_warpCutoff(lpfBuffer, cutoffBuffer, halfT, twoSlashT, bufferSize);//cutoff warping, now lpfBuffer contain wa
        }
    
        for (int n=0; n<bufferSize; ++n){

                if(cutoffAR){
                        g = lpfBuffer[n] * halfT; //if you put 2/T it becomes a trashy digital shitgenerator
                }
                if(resoAR){
                        Q = resoBuffer[n]*24.5f + 0.5f;
                }

                if(cutoffAR || resoAR){
                        R = 1.0f/(2.0f*Q);
                        alpha0 = 1.0f/(1.0f + 2.0f*R*g + g*g);
                        alpha = g;
                        rho = 2.0f * R+g;
                }

                //filter calculations
                hpfBuffer[n] = alpha0 * (inputBuffer[n] - rho*z1_1 - z1_2);
                
                bpfBuffer[n] = alpha * hpfBuffer[n] + z1_1;
                
                lpfBuffer[n] = alpha * bpfBuffer[n] + z1_2;
                
                R = 1.0f/(2.0f*Q);
                bsfBuffer[n] = inputBuffer[n] - 2.0f*R*bpfBuffer[n];

                z1_1 = alpha * hpfBuffer[n] + bpfBuffer[n];
                z1_2 = alpha * bpfBuffer[n] + lpfBuffer[n];
               
        }
    
}

