
#include "ScoreSection.h"
#include <iostream>
#include <float.h>
#include <limits.h>


pdsp::GateSequencer pdsp::ScoreSection::invalidGate = pdsp::GateSequencer();
pdsp::ValueSequencer pdsp::ScoreSection::invalidValue = pdsp::ValueSequencer();
pdsp::Sequence pdsp::ScoreSection::dummySeq  = pdsp::Sequence();

pdsp::ScoreSection::ScoreSection() {
    
    patterns.clear();
    scheduledPattern = -1;
    scheduledTime  = 10000000000000000000.0;
    launchSchedule = std::numeric_limits<double>::infinity();

    patternIndex = -1;
    scorePlayHead = 0.0;
    scoreIndex = 0;
    clearOnChangeFlag = true;
    run = false; //change to false in definitive version
    clear = true;
    quantizedLaunch = false;
    launchingCell = false;
    
    atomic_meter_current.store(-1);
    atomic_meter_next.store(-1);
    atomic_meter_playhead.store(0.0f);    
    atomic_meter_length.store(0.0f);    
    atomic_meter_percent.store(0.0f);

    setOutputsNumber(0);    
    
    gates.clear();
    values.clear();
    
}

pdsp::ScoreSection::ScoreSection(const pdsp::ScoreSection &other) {
    std::cout<<"score section copy constructed\n";
    resizeCells( (int) other.patterns.size() );
    
    for(int i=0; i< (int) other.patterns.size(); ++i){
        patterns[i].sequence          = other.patterns[i].sequence;
        patterns[i].nextCell           = other.patterns[i].nextCell;
        patterns[i].quantizeLaunch     = other.patterns[i].quantizeLaunch;
        patterns[i].quantizeGrid       = other.patterns[i].quantizeGrid;
    } 
    
    this->setOutputsNumber( (int)other.outputs.size() );
}

pdsp::ScoreSection::~ScoreSection(){

    for( int i=0; i < (int)gates.size(); ++i ){
        if(gates[i]!=nullptr) delete gates[i];
    }

    for( int i=0; i < (int)values.size(); ++i ){
        if(values[i]!=nullptr) delete values[i];
    }

    for( int i=0; i < (int)outputs.size(); ++i ){
        if(outputs[i]!=nullptr) delete outputs[i];
    }

}


void pdsp::ScoreSection::resizePatterns(int size){
    resizeCells(size);
}

void pdsp::ScoreSection::resizeCells(int size){
    if(size>0){
        if(size<(int)patterns.size()){
            if(patternIndex>=size){
                patternIndex = size-1;
                if(patternIndex<0) patternIndex = 0;
            }
        }
        patterns.resize(size);   
    }
}

int pdsp::ScoreSection::getCellsNumber() const{
    return patterns.size();
}

int pdsp::ScoreSection::getPatternsNumber() const{
    return getCellsNumber();
}

void pdsp::ScoreSection::launchCell( int index, bool quantizeLaunch, double quantizeGrid ){
    if     ( index < 0        ) { index = -1; }
    else if( index >= (int)patterns.size() ) { index = (int)patterns.size()-1; }

    if(quantizeLaunch){
        this->quantizedLaunch = true;
        this->launchQuantization = quantizeGrid;
    }else{
        this->quantizedLaunch = false;
    }
    this->launchedPattern = index;
    
    atomic_meter_next.store(index);
    this->launchingCell = true;
}


void pdsp::ScoreSection::setCell( int index, Sequence* sequence, SeqChange* behavior, std::string label ){
    if(index>=0){
        if(index>=(int)patterns.size()){
            resizeCells(index+1);
            //all the new patterns are initialized with nullptr, nullptr, length=1.0, quantize = false and quantizeGrid=0.0
        }

        patterns[index].sequence = sequence;
        patterns[index].nextCell  = behavior;
        //sequence->label = label;

    }
}

void pdsp::ScoreSection::setChange( int index, SeqChange* behavior ){
    if(index>=0 && index < (int) patterns.size()){
        patterns[index].nextCell  = behavior;
    }
}

void pdsp::ScoreSection::setLooping( int index ) {
    setChange(index, Behavior::Loop);
}

void pdsp::ScoreSection::setOneShot( int index ) {
    setChange(index, Behavior::OneShot);    
}

void pdsp::ScoreSection::setLabel( int index, std::string label ){
    if(index < 0) index = 0;
    if(index >= (int) patterns.size()) index = patterns.size() -1;
    if( patterns[index].sequence!= nullptr ) patterns[index].sequence->label = label;
}

std::string pdsp::ScoreSection::getLabel( int index){
    if(index < 0) index = 0;
    if(index >= (int) patterns.size()) index = patterns.size() -1;
    if( patterns[index].sequence!= nullptr ){
        return patterns[index].sequence->label;
    }else{
        return std::string("");
    } 
}

//remember to check for quantizeGrid to be no greater than pattern length
void pdsp::ScoreSection::setCellQuantization( int index, bool quantizeLaunch, double quantizeGrid ){
    if(index>=0){
        if(index>=(int) patterns.size()){
            resizeCells(index+1);
            //all the new patterns are initialized with nullptr, nullptr, length=1.0, quantize = false and quantizeGrid=0.0
        }

        patterns[index].quantizeLaunch     = quantizeLaunch;
        patterns[index].quantizeGrid       = quantizeGrid;   
    }   
}

   
void pdsp::ScoreSection::enableQuantization(int index, double quantizeGrid ){
    setCellQuantization(index, true, quantizeGrid);
}
   
void pdsp::ScoreSection::disableQuantization(int index){
    setCellQuantization(index, false);
}


void pdsp::ScoreSection::processSection(const double &startPlayHead, 
                                const double &endPlayHead, 
                                const double &playHeadDifference, 
                                const double &maxBars,
                                const double &barsPerSample, 
                                const int &bufferSize) noexcept {
    
        if( scheduledTime >= maxBars+playHeadDifference ){ scheduledTime -= maxBars; } //wraps scheduled time around
        
        
        // if we have launched a cell schedules the triggering
        if(launchingCell){
            
            if(quantizedLaunch && startPlayHead!=0.0){
                double timeToQuantize = startPlayHead + launchQuantization;
                int rounded = static_cast<int> ( timeToQuantize / launchQuantization ); 
                launchSchedule = static_cast<double>(rounded) * launchQuantization ;
                launchedPattern2 = launchedPattern;
            }else{
                launchSchedule = std::numeric_limits<double>::infinity();
                scheduledTime = startPlayHead;
                scheduledPattern = launchedPattern;
            }
            launchingCell = false;
            run = true;
        }
        
        // activate a scheduled launch when it's time
        if( launchSchedule <= scheduledTime ){
            scheduledTime = launchSchedule;
            scheduledPattern = launchedPattern2;
            if(patterns[scheduledPattern].sequence!=nullptr && scheduledPattern!=-1) {
                patterns[scheduledPattern].sequence->resetCount();
            }
            launchSchedule = std::numeric_limits<double>::infinity();
        }


        if( run && ((int)patterns.size())>0 ){
            clearBuffers();
            double oneSlashBarsPerSample = 1.0 / barsPerSample;

            if(scheduledTime >= endPlayHead){   //more likely
                if(patternIndex!=-1 && patterns[patternIndex].sequence!=nullptr) playScore(playHeadDifference, 0.0, oneSlashBarsPerSample);
                
            }else if(scheduledTime == startPlayHead){ 
                
                onSchedule();
                if(clearOnChangeFlag) allNoteOff(0.0, oneSlashBarsPerSample); 
                if(patternIndex!=-1 && patterns[patternIndex].sequence!=nullptr) playScore(playHeadDifference, 0.0, oneSlashBarsPerSample);

            }else if(scheduledTime > startPlayHead && scheduledTime < endPlayHead ){
                double schedulePoint = scheduledTime - startPlayHead;

                if(patternIndex!=-1 && patterns[patternIndex].sequence!=nullptr) playScore(schedulePoint, 0.0, oneSlashBarsPerSample); //process old clip
                onSchedule();
                if(clearOnChangeFlag) allNoteOff(schedulePoint, oneSlashBarsPerSample);

                if(patternIndex!=-1 && patterns[patternIndex].sequence!=nullptr) playScore(playHeadDifference, schedulePoint, oneSlashBarsPerSample);//process new clip
            }
            
            processBuffersDestinations(bufferSize);
            
        }else if(clear==true){
            //empty message buffers and process destinations
            clearBuffers();
            processBuffersDestinations(bufferSize);
            clear = false;
        }

        atomic_meter_playhead.store(scorePlayHead);  
         
        if(atomic_meter_length > 0.0f){
            float percent = scorePlayHead / atomic_meter_length;
            atomic_meter_percent.store( percent );
            patterns[patternIndex].sequence->atomic_meter_percent.store( percent );
        } else{
            atomic_meter_percent.store(0.0f);
        }
    
}

//-----------------------------------------------INTERNAL FUNCTIONS---------------------------------------------

//range is the quantity in bars of score to be elaborated, offset displace the start of processing
void pdsp::ScoreSection::playScore(double const &range, double const &offset, const double &oneSlashBarsPerSample) noexcept{

    Sequence* patternToProcess = patterns[patternIndex].sequence;
        
    double patternMax = scorePlayHead + range - offset;
    
    while( (scoreIndex < patternToProcess->score.size()) && (patternToProcess->score[scoreIndex].time < patternMax) ){
        
        if( (patternToProcess->score[scoreIndex].time >= scorePlayHead) && (patternToProcess->score[scoreIndex].lane < (int)outputs.size() )   ){ //check if we are inside the outputs boundaries and inside the processed time
            int sample = static_cast<int>( (patternToProcess->score[scoreIndex].time - scorePlayHead + offset) * oneSlashBarsPerSample);
            outputs[patternToProcess->score[scoreIndex].lane]->addMessage(patternToProcess->score[scoreIndex].value, sample);
            //std::cout<<"added message with sample value:"<<sample<<"\n";
        }
        
        scoreIndex++;
        
    }
    
    scorePlayHead = patternMax;
}

void pdsp::ScoreSection::onSchedule() noexcept{
    
    //clear score--------------------------------------------------------------------------
    //score.clear(); //clear score buffer
    scoreIndex = 0;
    
    //clip change routines--------------------------------------------------------------------------
    bool reset = ( patternIndex != scheduledPattern ) ? true : false;
    
    patternIndex = scheduledPattern; 

    if( patternIndex >=0 && patterns[patternIndex].sequence!=nullptr){ //if there is a pattern, execute it's generative routine
        if(reset) patterns[patternIndex].sequence->resetCount();
        
        patterns[patternIndex].sequence->executeGenerateScore( );
        
        atomic_meter_current.store(patternIndex);
        atomic_meter_length.store(patterns[patternIndex].sequence->length());
     
        if(patterns[patternIndex].nextCell!=nullptr){ //we have a behavior to get next pattern
            scheduledPattern = patterns[patternIndex].nextCell->getNextPattern(patternIndex, (int) patterns.size());
            atomic_meter_next.store(scheduledPattern);

            if(scheduledPattern<0 ){                scheduledPattern=-1; }
            else if( scheduledPattern>=(int) patterns.size() ){  scheduledPattern = ((int)patterns.size())-1; }
            
            if( scheduledPattern!=-1 ){

                if( patterns[patternIndex].quantizeLaunch ){
                    double timeToQuantize = (scheduledTime + patterns[patternIndex].quantizeGrid);
                    int rounded = static_cast<int> ( timeToQuantize /  patterns[patternIndex].quantizeGrid ); 
                    scheduledTime = static_cast<double>(rounded) * patterns[patternIndex].quantizeGrid ;
                }else{
                    scheduledTime = scheduledTime + patterns[patternIndex].sequence->length(); //+ patterns[scheduledPattern].quantizeGrid;
                }                
            }else{
                scheduledTime = scheduledTime + patterns[patternIndex].sequence->length(); 
            }
            
        }else{ //we don't have a behavior to get a next pattern --------> STOPPING ROW AFTER EXECUTION
            atomic_meter_next.store(-1);        
            scheduledTime = scheduledTime + patterns[patternIndex].sequence->length();
            scheduledPattern = -1;
        }   
              
    }else{
        atomic_meter_current.store(-1);
        atomic_meter_next.store(-1);
        atomic_meter_length.store(0.0f);   
        run = false;
        clear = true;
        scheduledPattern = -1;
        scheduledTime  = 10000000000000000000.0;
    }

    //reset score playhead
    scorePlayHead = 0.0; // reset pattern playhead to zero

}

void pdsp::ScoreSection::allNoteOff(double const &offset, const double &oneSlashBarsPerSample) noexcept{
    
    int sample = static_cast<int>( offset * oneSlashBarsPerSample);

    for(MessageBuffer* &buffer : outputs){
        if(buffer->connectedToGate){
            buffer->addMessage(0.0f, sample);
        }
    }
}

void pdsp::ScoreSection::clearBuffers() noexcept {
    for(MessageBuffer* &buffer : outputs){
        buffer->clearMessages();
    }
}

void pdsp::ScoreSection::processBuffersDestinations(const int &bufferSize) noexcept {
    for(MessageBuffer* &buffer : outputs){
        buffer->processDestination(bufferSize);
    }
}

//---------------------------------------------PATCHING---------------------------------


void pdsp::ScoreSection::setOutputsNumber(int size){
    if( size > (int) outputs.size()){
        int oldSize = outputs.size();
        outputs.resize(size);        
        for(int i=oldSize; i<size; ++i){
            outputs[i] = new MessageBuffer();
        }        
    }
}


pdsp::MessageBuffer& pdsp::ScoreSection::out_message( int index ){
    if(index<0) index = 0;

    setOutputsNumber(index+1); // make the array larger if needed
    return *outputs[index];
}

pdsp::MessageBuffer& pdsp::ScoreSection::out( int index ){
    return out_message(index);
}


pdsp::GateSequencer& pdsp::ScoreSection::out_trig( int index ){
    
    if(index<0){ index = 0; }
    
    if(index < (int) values.size()){
        if(values[index]!=nullptr){
            std::cout<<"[pdsp] score section output already used as value out, assignation invalid\n";
            pdsp_trace();
            return invalidGate;
        }
    }
    
    if(index >= (int) gates.size()){
        int oldSize = gates.size();
        gates.resize(index+1);
        for(int i=oldSize; i<=index; ++i){
            gates[i] = nullptr;
        }
    }
    
    if(gates[index]==nullptr){
        gates[index] = new GateSequencer();
        setOutputsNumber(index+1);
        *outputs[index] >> *gates[index];
    }
    
    return *gates[index];
    
}

pdsp::ValueSequencer& pdsp::ScoreSection::out_value( int index ){
    
    if(index<0){ index = 0; }
    
    if(index < (int) gates.size()){
        if(gates[index]!=nullptr){
            std::cout<<"[pdsp] score section output already used as trigger out, assignation invalid\n";
            pdsp_trace();
            return invalidValue;
        }
    }
    
    if(index >= (int) values.size()){
        int oldSize = values.size();
        values.resize(index+1);
        for(int i=oldSize; i<=index; ++i){
            values[i] = nullptr;
        }
    }
    
    if(values[index]==nullptr){
        values[index] = new ValueSequencer();
        setOutputsNumber(index+1);
        *outputs[index] >> *values[index];
    }
    
    return *values[index];  
    
}

void pdsp::ScoreSection::linkSlewControl( int valueOutIndex, int slewControlIndex ){
    
    if( valueOutIndex <= 0 ) valueOutIndex = 0;
    
    if (valueOutIndex >= (int)values.size() ){
        std::cout<<"[pdsp] invalid value out index for slew linking\n";
        pdsp_trace();
        return;
    }
    
    out_message(slewControlIndex) >> (*values[valueOutIndex]).in_slew();
    
}

int pdsp::ScoreSection::meter_current() const {
    return atomic_meter_current.load();
}

int pdsp::ScoreSection::meter_next() const {
    return atomic_meter_next.load();
}

float pdsp::ScoreSection::meter_playhead() const {
    return atomic_meter_playhead.load(); 
}

float pdsp::ScoreSection::meter_percent() const {
    return atomic_meter_percent.load(); 
}

float pdsp::ScoreSection::meter_length() const {
    return atomic_meter_length.load(); 
}

void pdsp::ScoreSection::clearOnChange(bool active) {
    clearOnChangeFlag = active;
}

const pdsp::Sequence & pdsp::ScoreSection::getSequence( int i ) const{
    return *(patterns[i].sequence);
}


// DEPRECATED
void pdsp::ScoreSection::setPattern( int index, Sequence* sequence, SeqChange* behavior ){
    setCell(index, sequence, behavior);
}

// DEPRECATED
void pdsp::ScoreSection::setBehavior( int index, SeqChange* behavior ){
    setChange(index, behavior);
}


void pdsp::ScoreSection::autoInitCells(){
    
    seqs.resize( patterns.size() );
    for( int i=0; i<(int)seqs.size(); ++i){
        setCell(i, &seqs[i], pdsp::Behavior::Loop); 
    }
    
}

pdsp::Sequence& pdsp::ScoreSection::sequence( int index ){
    if( index >= 0 && index < (int) patterns.size() ){
        if (patterns[index].sequence == nullptr){
            std::cout<<"[pdsp] warning, asking for sequence not set yet, returning dummy sequence for avoiding fatal error\n";
            pdsp_trace();
            return dummySeq;
        }else{
            return *(patterns[index].sequence);
        }
    }else{
        std::cout<<"[pdsp] wrong index for getting sequence, returning dummy sequence for avoiding fatal error\n";
        pdsp_trace();     
        return dummySeq;   
    }
}

void pdsp::ScoreSection::behavior ( int index, pdsp::SeqChange* newBehavior ){
    if( index >= 0 && index < (int) patterns.size() ){
        setChange( index, newBehavior );        
    }else{
        std::cout<<"[pdsp] wrong index for setting behavior\n";
        pdsp_trace();        
    }
}

void pdsp::ScoreSection::quantize ( int index, double quantizeTime) {
    if( index >= 0 && index < (int) patterns.size() ){
        enableQuantization(index, quantizeTime );
    }else{
        std::cout<<"[pdsp] wrong index for quantization\n";
        pdsp_trace();
    } 
}

void pdsp::ScoreSection::label ( int index, std::string name ) {
    if( index >= 0 && index < (int) patterns.size() && patterns[index].sequence!= nullptr ){
        patterns[index].sequence->label = name;
    }else{
        std::cout<<"[pdsp] wrong index for setting label\n";
        pdsp_trace();
    } 
}

void pdsp::ScoreSection::resetCounterOnStop() {
    if(patternIndex!=-1 && patterns[patternIndex].sequence!=nullptr){
        patterns[patternIndex].sequence->resetCount();
    }
}
