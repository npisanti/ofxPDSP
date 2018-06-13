

#include "OnePole.h"


pdsp::OnePole::OnePole(){

        addInput("signal", input_signal);
        addInput("freq", input_cutoff);
        addOutput("lpf", output_lpf);
        addOutput("hpf", output_hpf);
        updateOutputNodes();
        
        input_cutoff.setDefaultValue(8000.0f);
        if(dynamicConstruction){
                prepareToPlay(globalBufferSize, globalSampleRate);
        }
}

pdsp::Patchable& pdsp::OnePole::set(float freq){
    input_cutoff.setDefaultValue(freq);
    return *this;
}

pdsp::Patchable& pdsp::OnePole::in_signal(){
    return in("signal");
}

pdsp::Patchable& pdsp::OnePole::in_cutoff(){
    return in("freq");
}

pdsp::Patchable& pdsp::OnePole::in_freq(){
    return in("freq");
}

pdsp::Patchable& pdsp::OnePole::out_lpf(){
    return out("lpf");
}

pdsp::Patchable& pdsp::OnePole::out_hpf(){
    return out("hpf");
}


void pdsp::OnePole::prepareUnit( int expectedBufferSize, double sampleRate ){
            z1 = 0.0f;
            
            halfT = (0.5/sampleRate);
            twoSlashT = 1.0f / halfT;
}

void pdsp::OnePole::releaseResources(){
    
}


void pdsp::OnePole::process(int bufferSize) noexcept {

        int inputBufferState;
        const float* inputBuffer = processInput(input_signal, inputBufferState);

        //if(inputBufferState==AudioRate){
        
                int cutoffBufferState;
                const float* cutoffBuffer = processInput(input_cutoff, cutoffBufferState);
                 //Logger::writeToLog("processing");

                if(cutoffBufferState==Changed){
                        vect_warpCutoff(&wa, cutoffBuffer, halfT, twoSlashT, 1);//cutoff warping
                        calculateCoefficient(wa);                   
                }

                int switcher = inputBufferState + cutoffBufferState*4;
                
                //process audio rate
                switch ( switcher & 42 ) {
                case 0 : //unchanged, unchanged, unchanged
                        process_audio<false, false>(inputBuffer, cutoffBuffer, bufferSize);
                        setOutputToZero(output_hpf);
                        break;
                case 2 : //changed, unchanged, unchanged
                        process_audio<true, false>(inputBuffer, cutoffBuffer, bufferSize);
                        break;
                case 8 : //unchanged, changed, unchanged
                        process_audio<false, true>(inputBuffer, cutoffBuffer, bufferSize);
                        setOutputToZero(output_hpf);
                        break;
                case 10 : //changed, changed, unchanged
                        process_audio<true, true>(inputBuffer, cutoffBuffer, bufferSize);
                        break;
                default:
                        break;
                }
                
                //switch (cutoffBufferState) {
                //case Unchanged: case Changed:
                //        process_audio<true, false>(inputBuffer, cutoffBuffer, bufferSize);
                //        break;
                //case AudioRate:
                //        process_audio<true, true>(inputBuffer, cutoffBuffer, bufferSize);
                //        break;
                //default:
                //        break;
                //}
        //}else{
        //        setOutputToZero(output_hpf);
        //        setOutputToZero(output_lpf);
        //}
    
}


inline_f void pdsp::OnePole::calculateCoefficient (float wa){

        float g = wa * halfT;
        alpha = g / (1.0 + g);

}



template<bool inputAR, bool cutoffAR>
void pdsp::OnePole::process_audio(const float* &inputBuffer, const float* &cutoffBuffer, const int &bufferSize) noexcept {

        float* hpfBuffer = getOutputBufferToFill(output_hpf);
        float* lpfBuffer = getOutputBufferToFill(output_lpf);


        if(cutoffAR){
                //vect_pitchToFreq(lpfBuffer, cutoffBuffer, bufferSize);//we use the output buffer as temporary storage
                vect_warpCutoff(lpfBuffer, cutoffBuffer, halfT, twoSlashT, bufferSize);//cutoff warping, now outputBuffer contain wa
        }
    
        for (int n=0; n<bufferSize; ++n){
                if (cutoffAR) {
                        calculateCoefficient(lpfBuffer[n]);
                }

                //filter calculations
                float vn;
                if(inputAR){
                    vn = (inputBuffer[n] - z1) * alpha;                    
                }else{
                    vn = (inputBuffer[0] - z1) * alpha;                    
                }

                float lpf = vn + z1;
                z1 = vn + lpf;
                lpfBuffer[n] = lpf;
                hpfBuffer[n] = inputBuffer[n] - lpf;

        }

}


