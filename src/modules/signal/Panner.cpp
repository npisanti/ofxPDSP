
#include "Panner.h"

pdsp::Panner::Panner() { patch(); }
pdsp::Panner::Panner(const Panner& other){ patch(); }
pdsp::Panner& pdsp::Panner::operator=(const Panner& other){ return *this; }

//pdsp::Panner::Panner(Panner&& other){ patch(); }
//pdsp::Panner::Panner& operator=( Panner&& other){ return *this; }

        
float pdsp::Panner::PannerFormula::formula(const float &x) noexcept {
    if(x>=1.0f){
        return M_PI_2;
    }else if(x<=-1.0f){
        return 0.0f;
    }else{
        return M_PI_4 * (x+1.0f);        
    }

}

void pdsp::Panner::PannerFormula::formulaAudioRate(float* &output, const float* &input, const int &bufferSize) noexcept {
    ofx_Aeq_clipB(output, input, -1.0f, 1.0f, bufferSize);
    ofx_Aeq_BaddS(output, output, 1.0f, bufferSize);
    ofx_Aeq_BmulS(output, output, M_PI_4, bufferSize);
}

float pdsp::Panner::SinFormula::formula(const float &x)noexcept {
    return sinf(x);
}

void pdsp::Panner::SinFormula::formulaAudioRate(float* &output, const float* &input, const int &bufferSize) noexcept {
    ofx_Aeq_sinB(output, input, bufferSize);
}

float pdsp::Panner::CosFormula::formula(const float &x)noexcept {
    return cosf(x);
}

void pdsp::Panner::CosFormula::formulaAudioRate(float* &output, const float* &input, const int &bufferSize) noexcept {
    ofx_Aeq_cosB(output, input, bufferSize);
}


void pdsp::Panner::patch(){
    
    addModuleInput( "signal",  input ); 
    addModuleInput("pan", panning_step1);
    addModuleOutput("0", amp1);
    addModuleOutput("1", amp2);    
    
    panning_step1.enableBoundaries(-1.0f, 1.0f);
    
    input >> amp1;
    input >> amp2;
    
    panning_step1 >> panning_step2L >> amp1.in_mod();
    panning_step1 >> panning_step2R >> amp2.in_mod();    

}

float pdsp::Panner::meter_pan_input(){
    return panning_step1.meter_input();
}

pdsp::Patchable& pdsp::Panner::in_signal(){
    return in("signal");
}

pdsp::Patchable& pdsp::Panner::in_pan(){
    return in("pan");
}

pdsp::Patchable& pdsp::Panner::out_0(){
    return out("0");
}

pdsp::Patchable& pdsp::Panner::out_1(){
    return out("1");
}
