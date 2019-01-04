

#include "Clockable.h"

//long pdsp::Clockable::globalFramePosition = 0L;
bool pdsp::Clockable::playing = false;
float pdsp::Clockable::globalTempo = 120.0f;
double pdsp::Clockable::clockBaseSampleRate = 44100.0f;
float pdsp::Clockable::masterInc = 120.0 / (44100.0 * (60.0 * 4.0));  
float pdsp::Clockable::globalBarPosition = 0.0f;
float pdsp::Clockable::barTimeMs = 60000 * 0.25f / 120.0f;
/*
void pdsp::Clockable::setGlobalFramePosition( long framePosition){
    globalFramePosition = framePosition;
    globalBarPosition = framePosition * masterInc;
}
*/

/*
void pdsp::Clockable::addToGlobalFramePosition( int samples){
    globalFramePosition += samples;
    globalBarPosition = globalFramePosition * masterInc;
}


void pdsp::Clockable::setPlayingState(bool playing){
    Clockable::playing = playing;
}



*/

float pdsp::Clockable::getGlobalBarPosition(){
    return globalBarPosition;
}

void pdsp::Clockable::changeSampleRate(double sampleRate){
    clockBaseSampleRate = sampleRate;
    updateMasterInc();
}

void pdsp::Clockable::updateMasterInc(){
    masterInc = (static_cast<double>(globalTempo)  / ((60.0 * 4.0) * clockBaseSampleRate) ) ;
}

void pdsp::Clockable::setTempo(float tempo){
    globalTempo = tempo;
    barTimeMs = (60000.0f * 4.0f) / tempo; // * 0.25f is for 4/4
    updateMasterInc();
}

float pdsp::Clockable::getOneBarTimeMs(){
    return barTimeMs;
}
    
float pdsp::Clockable::getBarsPerSample(){
    return masterInc;
}

bool pdsp::Clockable::isPlaying(){
    return playing;
}

/*
void pdsp::setTempo(float tempo){
    Clockable::setTempo(tempo);
}


void pdsp::pauseTransport(){
    Clockable::setPlayingState(false);
}

void pdsp::stopTransport(){
    Clockable::setPlayingState(false);
    //Clockable::setGlobalFramePosition( 0L );
}

void pdsp::startTransport(){
    Clockable::setPlayingState(true);
}


void pdsp::updateTransport( int bufferSize ){
    if(Clockable::isPlaying()){
        Clockable::addToGlobalFramePosition(bufferSize);
    }
}
*/
