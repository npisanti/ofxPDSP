
#include "GrainWindow.h"


pdsp::GrainWindow::GrainWindow(){
    
        addInput("signal", input_signal);
        addInput("trig", input_trigger);
        addInput("length", input_length_ms);

        addOutput("signal", output);
        
        updateOutputNodes();

        //interpolatorShell.changeInterpolator(Linear);
        setWindowType(Triangular);
        
        input_length_ms.setDefaultValue(200.0f);

        windowMeter.store(0.0f);
        
        if(dynamicConstruction){
                prepareToPlay(globalBufferSize, globalSampleRate);
        }
}

pdsp::Patchable& pdsp::GrainWindow::set(float grainLengthMs){
    input_length_ms.setDefaultValue(grainLengthMs);  
    return *this;  
}

pdsp::Patchable& pdsp::GrainWindow::set(float grainLengthMs, Window_t type, int window_length){
    input_length_ms.setDefaultValue(grainLengthMs);  
    setWindowType(type, window_length);
    return *this;
}

pdsp::Patchable& pdsp::GrainWindow::in_signal(){
    return in("signal");
}

pdsp::Patchable& pdsp::GrainWindow::in_trig(){
    return in("trig");
}

pdsp::Patchable& pdsp::GrainWindow::in_length(){
    return in("length");
}
/*
void pdsp::GrainWindow::setInterpolatorType(Interpolator_t type){
        interpolatorShell.changeInterpolator(type);
}
*/
void pdsp::GrainWindow::setWindowType(Window_t type, int window_length){
        grainShape = getTable(type, window_length);
}

float pdsp::GrainWindow::meter_window() const{
    return windowMeter.load();
}

void pdsp::GrainWindow::prepareUnit( int expectedBufferSize, double sampleRate ) {
        this->sampleRate = sampleRate;
        //interpolatorShell.interpolator->reset();
        phase = 0.0f;
        run = false;
}

void pdsp::GrainWindow::changeLength( float newLength){

        length = sampleRate * newLength * 0.001f;
        baseInc = 1.0f / length;
        

}


void pdsp::GrainWindow::process(int bufferSize) noexcept{

        int triggerState;
        const float* triggerBuffer = processInput(input_trigger, triggerState);

        if( run || triggerState == AudioRate ){ //there are still samples to emit or new triggers     
             
                int inputBufferState;
                const float * inputBuffer = processInput(input_signal, inputBufferState);

                int lenState;
                const float* lenBuffer = processInput(input_length_ms, lenState);
                if(lenState==Changed ){
                    changeLength(lenBuffer[0]);
                }

                int switcher = inputBufferState + triggerState*4;

                //process once
                switch (switcher & 42) {
                case 0 : //unchanged, unchanged, unchanged
                        process_audio<false, false>( inputBuffer, triggerBuffer, bufferSize );
                        break;
                case 2 : //changed, unchanged, unchanged
                        process_audio<true, false>( inputBuffer, triggerBuffer, bufferSize );
                        break;
                case 8 : //unchanged, changed, unchanged
                        process_audio<false, true>( inputBuffer, triggerBuffer, bufferSize );
                        break;
                case 10 : //changed, changed, unchanged
                        process_audio<true, true>( inputBuffer, triggerBuffer, bufferSize );
                        break;
                default:
                        break;
                }
                            
        }else{
                setOutputToZero(output);
        }
    
}


template<bool signalAR, bool triggerAR>
void pdsp::GrainWindow::process_audio( const float* inputBuffer,  const float* triggerBuffer, int bufferSize) noexcept{
    
    float* outputBuffer = getOutputBufferToFill(output);

    for(int n=0; n<bufferSize; ++n){
        
        if(run){
            
            float index = phase*grainShape->length_f;
            int index_int = static_cast<int> (index);
            float mu = index - index_int;
            float x1 = grainShape->buffer[index_int];
            float x2 = grainShape->buffer[index_int+1];
            float winAmp = interpolate_linear(x1, x2, mu);
           
            //float winAmp = interpolatorShell.interpolate(grainShape->buffer, phase*grainShape->length_f, grainShape->length);
            windowMeter.store(winAmp);
           
            if(signalAR){
                outputBuffer[n] = inputBuffer[n] * winAmp;
            }else{
                outputBuffer[n] = inputBuffer[0] * winAmp;                
            }
            
        }else{
            outputBuffer[n] = 0.0f;
        }
        
        if(triggerAR){
            if(checkTrigger(triggerBuffer[n])){
                run = true;
                phase = 0.0f;
            }
        }
        
        phase+=baseInc;
        if(phase>=1.0f){ run = false; }
        
    }  

}

