
#include "MidiControls.h"

#ifndef __ANDROID__

pdsp::midi::Controls::Controls(){
    setMaxCCNum(midiCC.getCCSize());
    setCCSlew(50.0f);   
}


pdsp::midi::Controls::Controls(int maxCC){
    setMaxCCNum(maxCC);
    setCCSlew(50.0f);   
}

void pdsp::midi::Controls::processMidi(const pdsp::midi::Input &midiInProcessor, const int &bufferSize ) noexcept{
    midiCC.processMidi(midiInProcessor.getMessageVector(), bufferSize);
}


void pdsp::midi::Controls::setCCSlew(float slewTimeMs){
    this->slewTime = slewTimeMs;
    
    for(int i=0; i<midiCC.getCCSize(); ++i){
        outs_cc[i].setSlewRateModeReference(1.0f);
        outs_cc[i].setSlewTime(slewTimeMs, pdsp::Rate);
    }
}


void pdsp::midi::Controls::setMaxCCNum(int ccNum){
    midiCC.setMaxCCNum(ccNum);
    outs_cc.resize(midiCC.getCCSize());
    setCCSlew(slewTime);
    for(int i=0; i<midiCC.getCCSize(); ++i){
        midiCC.ccMessages[i] >> outs_cc[i];
    }
    midiCC.clearAll();
}

int pdsp::midi::Controls::getMaxCCNum(){
    return midiCC.getCCSize();
}


pdsp::ValueSequencer & pdsp::midi::Controls::out( int cc ) {
    if( cc < 0 ){
        cc = - cc;
    }
    if( cc > getMaxCCNum() ){
        setMaxCCNum(cc);
    }
    return outs_cc[cc];
}
    
#endif
