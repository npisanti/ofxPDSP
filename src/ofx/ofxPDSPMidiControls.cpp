
#include "ofxPDSPMidiControls.h"


ofxPDSPMidiControls::ofxPDSPMidiControls(){
    setMaxCCNum(midiCC.getCCSize());
    setCCSlew(50.0f);   
}


ofxPDSPMidiControls::ofxPDSPMidiControls(int maxCC){
    setMaxCCNum(maxCC);
    setCCSlew(50.0f);   
}

void ofxPDSPMidiControls::processMidi(const ofxPDSPMidiIn &midiInProcessor, const int &bufferSize ) noexcept{
    midiCC.processMidi(midiInProcessor.getMessageVector(), bufferSize);
}


void ofxPDSPMidiControls::setCCSlew(float slewTimeMs){
    this->slewTime = slewTimeMs;
    
    for(int i=0; i<midiCC.getCCSize(); ++i){
        outs_cc[i].setSlewRateModeReference(1.0f);
        outs_cc[i].setSlewTime(slewTimeMs, pdsp::Rate);
    }
}


void ofxPDSPMidiControls::setMaxCCNum(int ccNum){
    midiCC.setMaxCCNum(ccNum);
    outs_cc.resize(midiCC.getCCSize());
    setCCSlew(slewTime);
    for(int i=0; i<midiCC.getCCSize(); ++i){
        midiCC.ccMessages[i] >> outs_cc[i];
    }
    midiCC.clearAll();
}

int ofxPDSPMidiControls::getMaxCCNum(){
    return midiCC.getCCSize();
}


pdsp::ValueSequencer & ofxPDSPMidiControls::out( int cc ) {
    if( cc < 0 ){
        cc = - cc;
    }
    if( cc > getMaxCCNum() ){
        setMaxCCNum(cc);
    }
    return outs_cc[cc];
}
    
