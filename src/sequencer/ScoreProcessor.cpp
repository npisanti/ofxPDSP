
#include "ScoreProcessor.h"



pdsp::ScoreProcessor::ScoreProcessor(){
    
    //synchronizeClockable = true;
    tempo = 120.0;
    playHead = 0.0;
    playHeadEnd = 0.0;
    newPlayHead = 0.0f;
    
    maxBars = 32000.0;
    
    clearToken = 0;
    
    playhead_meter.store(0.0f);
    
    sections.clear();
    sections.reserve(16);
    sections.clear();
    
    playing.store(true);
    
    if(dynamicConstruction){
        prepareToPlay(globalBufferSize, globalSampleRate);
    }
}

void pdsp::ScoreProcessor::setSections(int sectionsNumber){
    sections.resize(sectionsNumber);
}

void pdsp::ScoreProcessor::setMaxBars(double maxBars){
    this->maxBars = maxBars;
    while(playHead > maxBars){
        playHead -= maxBars;
    }
}


void pdsp::ScoreProcessor::process(int const &bufferSize) noexcept{
   
    if( playing.load() ){
        
        if(newPlayHead >= 0.0f){
            playHeadEnd = newPlayHead;
            newPlayHead = -1.0f;
        }
    
        playHead = playHeadEnd;
        if( playHead > maxBars ) { playHead -= maxBars; } //wrap score
        double playHeadDifference = bufferSize * barsPerSample;
        playHeadEnd = playHead + playHeadDifference;

        playhead_meter.store(playHead);
        
        //now process sections-----------------
        for(ScoreSection &sect : sections){
            sect.processSection(playHead, playHeadEnd, playHeadDifference, maxBars, barsPerSample, bufferSize);
        }
        //---------------------------------

        Clockable::globalBarPosition = playHead;
        
    }else{
        // on pause we send trigger offs to gates and set all the sequencers to control rate
        if(clearToken>0){
            for(ScoreSection &sect : sections){
                sect.clearBuffers();
                if(clearToken==2){ sect.allNoteOff(0.0, 0.5); } //0.5 * 0.0 = 0.0 the message will be on the first sample
                sect.processBuffersDestinations(bufferSize);
            } 
            clearToken--;
        }
    }
}

void pdsp::ScoreProcessor::prepareToPlay( int expectedBufferSize, double sampleRate ){
    this->sampleRate = sampleRate;
    setTempo(tempo);
}


void pdsp::ScoreProcessor::releaseResources() {}


void pdsp::ScoreProcessor::setTempo( float tempo ){
    this->tempo = tempo;
    barsPerSample = static_cast<double>(tempo)  / ((60.0 * 4.0) * sampleRate )  ;

    Clockable::setTempo(tempo);

}


void pdsp::ScoreProcessor::setPlayHead(float newPlayHead){

    this->newPlayHead = newPlayHead;

}

void pdsp::ScoreProcessor::pause(){
    clearToken = 2;
    playing.store(false);
    Clockable::playing = false;
}


void pdsp::ScoreProcessor::stop(){
    clearToken = 2;
    playing.store(false);
    Clockable::playing = false;
    setPlayHead(0.0f);

    for(ScoreSection &sect : sections){
        sect.scoreIndex = 0;
        //double oldPlayhead = sect.scorePlayHead ;
        sect.scorePlayHead = 0.0;
        //sect.scheduledTime -= oldPlayhead;
        sect.atomic_meter_playhead.store(0.0f);
        sect.launchCell(sect.atomic_meter_next);
    } 
}    


void pdsp::ScoreProcessor::play(){
    playing.store(true);
    Clockable::playing = true;    
}

float pdsp::ScoreProcessor::meter_playhead(){
    return playhead_meter.load();
}

double pdsp::ScoreProcessor::getMaxBars() const{
    return maxBars;
}

bool pdsp::ScoreProcessor::isPlaying(){
    return playing.load();
}


void pdsp::ScoreProcessor::init ( int sections, int sequences, float tempo ){
    
    this->sections.resize( sections );
    for ( int s=0; s<sections; ++s ){
        this->sections[s].resizeCells(sequences);
        this->sections[s].autoInitCells();
    }
    
    setTempo(tempo);
    
}

void pdsp::ScoreProcessor::launchMultipleCells(int index, bool quantizeLaunch, double quantizeGrid) {
    for (int i=0; i<(int)sections.size(); ++i){
        sections[i].launchCell(index, quantizeLaunch, quantizeGrid );
    }
}
