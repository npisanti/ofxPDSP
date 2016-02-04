
#include "SineFB.h"
//#include <iostream>

namespace pdsp {

float* SineFB::sineTable = nullptr;
uint64_t SineFB::sineOscillatorsCreated = 0;
const float SineFB::tableFakeSize = 4096.0f;

SineFB::SineFB(){

    input_shape.setDefaultValue(0.0f);
    
    if (sineOscillatorsCreated == 0){
        //init wavetable
        int max = static_cast<int> (tableFakeSize);
        sineTable = new float[max + 1];
        for (int i = 0; i < max; i++){
                sineTable[i] = sin((static_cast<float>(i) / tableFakeSize) * M_PI * 2.0);
        }
        sineTable[max] = sineTable[0]; //extra position to eliminate an if / else into the oscillator processing
    }
    sineOscillatorsCreated++;

    if(dynamicConstruction){
        prepareOscillator( globalSampleRate * getOversampleLevel() );
    }

};
    
SineFB::~SineFB(){
    sineOscillatorsCreated--;
    if (sineOscillatorsCreated == 0){
        delete[] sineTable;
        sineTable = nullptr;
    }
};

pdsp::Patchable& pdsp::SineFB::set(float fmFeedback){
    if(fmFeedback>4.0f){
        input_shape.setDefaultValue(4.0f);
    }else if(fmFeedback<0.0f){
        input_shape.setDefaultValue(0.0f);
    }else{
        input_shape.setDefaultValue(fmFeedback);
    }
    return *this;
}

void SineFB::prepareOscillator( double sampleRate){
    fb_path=0.0f;
    z1 = 0.0f;
    
    // INIT FB FILTER
    float freq  = sampleRate * 0.25;
    float halfT = 0.5 / sampleRate;
    float twoSlashT = 1.0f / halfT;
    float wa;
    vect_warpCutoff(&wa, &freq, halfT, twoSlashT, 1);//cutoff warping
    float g = wa * halfT;
    alpha = g / (1.0 + g);

};
    
void SineFB::releaseOscillator(){};


void SineFB::oscillateShapeCR(float* outputBuffer, const float* phaseBuffer, const float shape, int bufferSize) noexcept
{    
       
    if( shape == 0.0f ){ //we don't need self FM so we don't process the filter
        
        for (int n = 0; n < bufferSize; ++n){
            //linear interpolation and table lookup
            float index_f = phaseBuffer[n] * tableFakeSize;
            int index_i = static_cast<int>(index_f);
            float fract = index_f - index_i;
            outputBuffer[n] = interpolate_linear(sineTable[index_i], sineTable[index_i + 1], fract);
            fb_path = outputBuffer[n];
        }
        
    }else{ //processing also the filter in the fm path
        
        for (int n = 0; n < bufferSize; ++n){

            //fb filtering
            float vn = (fb_path - z1) * alpha;
            fb_path = vn + z1;
            z1 = vn + fb_path;

            //phase offset and rebound
            float phaseOffset = shape * fb_path;
            float phase = phaseBuffer[n] +  phaseOffset;

            float floor = floorf(phase);
            phase = phase - floor;

            //linear interpolation and table lookup
            float index_f = phase * tableFakeSize;
            int index_i = static_cast<int>(index_f);
            float fract = index_f - index_i;
            outputBuffer[n] = interpolate_linear(sineTable[index_i], sineTable[index_i + 1], fract);
            fb_path = outputBuffer[n];
        }                       
            
    }
        
}


void SineFB::oscillateShapeAR(float* outputBuffer, const float* phaseBuffer, const float* shapeBuffer, int bufferSize) noexcept
{
    
    for (int n = 0; n < bufferSize; ++n){

        //fb filtering
        float vn = (fb_path - z1) * alpha;
        fb_path = vn + z1;
        z1 = vn + fb_path;

        //phase offset and rebound
        float phaseOffset = shapeBuffer[n] * fb_path;
        float phase = phaseBuffer[n] +  phaseOffset;

        float floor = floorf(phase);
        phase = phase - floor;

        //linear interpolation and table lookup
        float index_f = phase * tableFakeSize;
        int index_i = static_cast<int>(index_f);
        float fract = index_f - index_i;
        outputBuffer[n] = interpolate_linear(sineTable[index_i], sineTable[index_i + 1], fract);
        fb_path = outputBuffer[n];
    }

}


}//END NAMESPACE

