
#include "Scope.h"


pdsp::Scope::Scope(){
        addInput("signal", input_signal);
        addOutput("signal", output);
        updateOutputNodes();
    
        bufferLen = -1;
        if(dynamicConstruction){
                prepareToPlay(globalBufferSize, globalSampleRate);
        }
        bufferPos = 0;
}

pdsp::Scope::Scope(const pdsp::Scope & other) : pdsp::Scope(){
    std::cout<<"[pdsp] warning! pdsp::Scope copy constructed\n"; 
    pdsp::pdsp_trace();
}
 
pdsp::Scope& pdsp::Scope::operator=(const pdsp::Scope& other){
    return *this;
    std::cout<<"[pdsp] warning! pdsp::Scope moved constructed\n";
    pdsp::pdsp_trace();
}


pdsp::Patchable& pdsp::Scope::set( int samplesLength) {
    bufferLen = samplesLength;
    return *this;
}


pdsp::Patchable& pdsp::Scope::in_signal(){
    return in("signal");
}

pdsp::Patchable& pdsp::Scope::out_signal(){
    return out("signal");
}



void pdsp::Scope::prepareUnit ( int expectedBufferSize, double sampleRate ) {
    if(bufferLen < expectedBufferSize ){
        bufferLen = expectedBufferSize;
    }
    buffer.resize(bufferLen);
}

void pdsp::Scope::releaseResources () {

}

void pdsp::Scope::process (int bufferSize) noexcept {

    int signalState;
    const float* signalBuffer = processInput(input_signal, signalState);

    switch( signalState ){
        case pdsp::Changed : case pdsp::Unchanged:
            for (int n=0; n<bufferSize; ++n){
                    buffer[bufferPos] = 0.0f;
                    bufferPos++;
                    bufferPos = (bufferPos == bufferLen) ? 0 : bufferPos;
            }

        break;
        case pdsp::AudioRate :
        {
            for (int n=0; n<bufferSize; ++n){
                    buffer[bufferPos] = signalBuffer[n];
                    bufferPos++;
                    bufferPos = (bufferPos == bufferLen) ? 0 : bufferPos;
            }
        }              
        break;
    }
    
    setOutputToZero(output);
    
}

void pdsp::Scope::draw( int x, int y, int w, int h) const {
    
	ofPushStyle();
    ofPushMatrix();
		ofTranslate(x, y);
        ofNoFill();
		ofSetLineWidth(1);
        
        float xMult = (float) bufferLen / (float) w; 
        float yHalf = h / 2;
        float yMult = -yHalf;
					
        ofBeginShape();
        for ( int xx = 0; xx < w; xx++ ){
            int index = xx*xMult; 
            float value = buffer[index];
            ofVertex(xx, yHalf + value*yMult);
        }
        ofEndShape(false);
    
		ofDrawRectangle(0, 0, w, h);
        
    ofPopMatrix();
	ofPopStyle(); 
    
}

const vector<float> & pdsp::Scope::getBuffer() const {
    return buffer;
}
