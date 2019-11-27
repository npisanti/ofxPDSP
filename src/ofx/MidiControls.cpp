
#include "MidiControls.h"

#ifndef __ANDROID__

pdsp::midi::Controls::Controls(){
    setCCSlew(50.0f);
}

pdsp::midi::Controls::Controls(int maxCC){
    setCCSlew(50.0f);
}

void pdsp::midi::Controls::processMidi(const pdsp::midi::Input &midiInProcessor, const int &bufferSize ) noexcept{
    midiCC.processMidi(midiInProcessor.getMessageVector(), bufferSize);
}

void pdsp::midi::Controls::setCCSlew(float slewTimeMs){
    this->slewTime = slewTimeMs;

    for (auto& cc : outs_cc) {
        cc.second.setSlewRateModeReference(1.0f);
        cc.second.setSlewTime(slewTimeMs, pdsp::Rate);
    }
}

void pdsp::midi::Controls::setMaxCCNum(int ccNum) {
}

int pdsp::midi::Controls::getMinCCNum() {
    return outs_cc.begin()->first;
}

int pdsp::midi::Controls::getMaxCCNum() {
    return outs_cc.rbegin()->first;
}

int pdsp::midi::Controls::getCCSize() {
    return midiCC.getCCSize();
}

pdsp::SequencerValueOutput & pdsp::midi::Controls::out(int cc) {
    if (cc < 0) {
        cc = -cc;
    }
    if (outs_cc.find(cc) == outs_cc.end()) {
        outs_cc[cc].setSlewRateModeReference(1.0f);
        outs_cc[cc].setSlewTime(slewTime, pdsp::Rate);
        midiCC.out(cc) >> outs_cc[cc];
    }
    return outs_cc[cc];
}

#endif
