
#include "SequencerProcessor.h"



pdsp::SequencerProcessor::SequencerProcessor(){

    //synchronizeClockable = true;
    tempo = -120.0;
    tempoControl = 120.0f;
    playHead = 0.0;
    playHeadEnd = 0.0;
    newPlayHead = 0.0f;

    maxBars = 32768.0;

    clearToken = 0;

    playhead_meter.store(0.0f);

    playing.store(true);

    if(dynamicConstruction){
        prepareToPlay(globalBufferSize, globalSampleRate);
    }
}

void pdsp::SequencerProcessor::setSections(int sectionsNumber){
    sections.resize(sectionsNumber);
}

void pdsp::SequencerProcessor::setMaxBars(double maxBars){
    this->maxBars = maxBars;
    while(playHead > maxBars){
        playHead -= maxBars;
    }
}


void pdsp::SequencerProcessor::process(int const &bufferSize) noexcept{

    if( tempo != tempoControl.load() ){
        tempo = tempoControl;
        barsPerSample = static_cast<double>(tempo)  / ((60.0 * 4.0) * sampleRate )  ;
        Clockable::setTempo(tempo);
    }

    if( playing ){

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
        for(SequencerSection &sect : sections){
            sect.processSection(playHead, playHeadEnd, playHeadDifference, maxBars, barsPerSample, bufferSize);
        }
        //---------------------------------

        //now process lambdas-----------------
        for( size_t i=0; i<pdsp::Function::instances.size(); ++i ){
            pdsp::Function::instances[i]->process( playHead, barsPerSample, bufferSize );
        }
        //---------------------------------

        Clockable::globalBarPosition = playHead;

    }else{
        // on pause we send trigger offs to gates and set all the sequencers to control rate
        if(clearToken>0){
            for(SequencerSection &sect : sections){
                sect.clearBuffers();
                if(clearToken==2){ sect.allNoteOff(0.0, 0.5); } //0.5 * 0.0 = 0.0 the message will be on the first sample
                sect.processBuffersDestinations(bufferSize);
            }
            clearToken--;
        }
    }
}

void pdsp::SequencerProcessor::prepareToPlay( int expectedBufferSize, double sampleRate ){
    this->sampleRate = sampleRate;
    barsPerSample = static_cast<double>(tempo) / ((60.0 * 4.0) * sampleRate);
}


void pdsp::SequencerProcessor::releaseResources() {}


void pdsp::SequencerProcessor::setTempo( double tempo ){
    tempoControl.store( tempo );
}


void pdsp::SequencerProcessor::setPlayHead(float newPlayHead){

    this->newPlayHead = newPlayHead;

}

void pdsp::SequencerProcessor::pause(){
    clearToken = 2;
    playing.store(false);
    Clockable::playing = false;
}


void pdsp::SequencerProcessor::stop(){
    clearToken = 2;
    playing.store(false);
    Clockable::playing = false;
    setPlayHead(0.0f);

    for(SequencerSection &sect : sections){
        sect.scoreIndex = 0;
        //double oldPlayhead = sect.scorePlayHead ;
        sect.scorePlayHead = 0.0;
        //sect.scheduledTime -= oldPlayhead;
        sect.atomic_meter_playhead.store(0.0f);
        sect.launchCell(sect.atomic_meter_next);
        sect.resetCounterOnStop();
    }
}


void pdsp::SequencerProcessor::play(){
    playing.store(true);
    Clockable::playing = true;
}

float pdsp::SequencerProcessor::meter_playhead(){
    return playhead_meter.load();
}

double pdsp::SequencerProcessor::getMaxBars() const{
    return maxBars;
}

bool pdsp::SequencerProcessor::isPlaying(){
    return playing.load();
}


void pdsp::SequencerProcessor::init ( int sections, int sequences, float tempo ){

    this->sections.resize( sections );
    for ( int s=0; s<sections; ++s ){
        this->sections[s].resizeCells(sequences);
        this->sections[s].autoInitCells();
    }

    setTempo(tempo);

}

void pdsp::SequencerProcessor::launchMultipleCells(int index, bool quantizeLaunch, double quantizeGrid) {
    for (int i=0; i<(int)sections.size(); ++i){
        sections[i].launchCell(index, quantizeLaunch, quantizeGrid );
    }
}

void pdsp::SequencerProcessor::setDefaultSteplen( double steplen ){
    pdsp::Sequence::defaultSteplen = steplen;
}

double pdsp::SequencerProcessor::getTempo() const{
    return tempo;
}
