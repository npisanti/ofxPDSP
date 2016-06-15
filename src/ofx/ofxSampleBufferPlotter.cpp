
#include "ofxSampleBufferPlotter.h"


ofxSampleBufferPlotter::ofxSampleBufferPlotter(){
    
    fboFilled = false;
    width = 0;
    height = 0;
    
}

void ofxSampleBufferPlotter::setWaveform(pdsp::SampleBuffer& sampleBuffer, int channel, ofColor color, int width, int height){
    
    if(sampleBuffer.buffer != nullptr){
        if(channel < 0) { channel = 0; }
        if(channel >= sampleBuffer.channels ) { channel = sampleBuffer.channels-1; }
        
        waveformCanvas.allocate(width, height, GL_RGBA);
        int step = sampleBuffer.length / width;
     
        waveformCanvas.begin();
        
            ofClear(255,255,255, 0);
            ofSetColor(color);
    
            for(int i=0; i<width; ++i){
                float value = sampleBuffer.buffer[channel][i*step];
                int halfHeight = height/2;
                float yOffset = static_cast<float>(halfHeight) * value;
                ofDrawLine(i, halfHeight - yOffset, i, halfHeight + yOffset);
            }
            
        waveformCanvas.end();        
        
        fboFilled = true;
        this->width = width;
        this->height = height;
    }
    
}

void ofxSampleBufferPlotter::draw(int x, int y){
    waveformCanvas.draw(x, y);
}

void ofxSampleBufferPlotter::draw(int x, int y, int w, int h){
    waveformCanvas.draw(x, y, w, h);
}

const ofFbo& ofxSampleBufferPlotter::getFbo(){
    return waveformCanvas;
}
