
#include "ofxMidiCCProcessor.h"


ofxMidiCCProcessor::ofxMidiCCProcessor(){
    setMaxCCNum(midiCC.getCCSize());
    setCCSlew(50.0f);   
}


ofxMidiCCProcessor::ofxMidiCCProcessor(int maxCC){
    setMaxCCNum(maxCC);
    setCCSlew(50.0f);   
}

void ofxMidiCCProcessor::processMidi(const ofxMidiInProcessor &midiInProcessor, const int &bufferSize ) noexcept{
    midiCC.processMidi(midiInProcessor.readVector, bufferSize);
}


void ofxMidiCCProcessor::setCCSlew(float slewTimeMs){
    this->slewTime = slewTimeMs;
    
    for(int i=0; i<midiCC.getCCSize(); ++i){
        outs_cc[i].setSlewRateModeReference(1.0f);
        outs_cc[i].setSlewTime(slewTimeMs, pdsp::Rate);
    }
}


void ofxMidiCCProcessor::setMaxCCNum(int ccNum){
    midiCC.setMaxCCNum(ccNum);
    outs_cc.resize(midiCC.getCCSize());
    setCCSlew(slewTime);
    for(int i=0; i<midiCC.getCCSize(); ++i){
        midiCC.ccMessages[i] >> outs_cc[i];
    }
    midiCC.clearAll();
}

int ofxMidiCCProcessor::getMaxCCNum(){
    return midiCC.getCCSize();
}

