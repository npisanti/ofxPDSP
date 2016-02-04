

#include "InterpolatorShell.h"

pdsp::InterpolatorShell::InterpolatorShell(){
    interpolator = reinterpret_cast<Interpolator*> ( &LinearInterpolator::linearInterpolator );
    type = Linear;
}

pdsp::InterpolatorShell::InterpolatorShell(const InterpolatorShell& other) : InterpolatorShell(){
    changeInterpolator(other.type);
    interpolator->reset();
}

pdsp::InterpolatorShell& pdsp::InterpolatorShell::operator= (const InterpolatorShell& other){
    changeInterpolator(other.type);
    interpolator->reset();
    return *this;
}

pdsp::InterpolatorShell::~InterpolatorShell(){
    if(interpolator->hasState()){
        delete interpolator;
    }
}


void pdsp::InterpolatorShell::resetInterpolator(){
    if( interpolator->hasState()){
        interpolator->reset();
    }
}

void pdsp::InterpolatorShell::changeInterpolator(Interpolator_t type){
    
    if (interpolator->hasState()) {
        delete interpolator;
    }
    
    if(type==Linear){
        interpolator = reinterpret_cast<Interpolator*> ( &LinearInterpolator::linearInterpolator );
        this->type = Linear;
    }else if(type==Smooth){
        interpolator = reinterpret_cast<Interpolator*> ( &SmoothInterpolator::smoothInterpolator );
        this->type = Smooth;
    }else{
        interpolator = reinterpret_cast<Interpolator*> ( &LinearInterpolator::linearInterpolator );
        this->type = Linear;
    }
    
}

