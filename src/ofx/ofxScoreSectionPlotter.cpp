
#include "ofxScoreSectionPlotter.h"

ofxScoreSectionPlotter::ofxScoreSectionPlotter() {
    setColor( ofColor(255, 255, 255) );
}

void ofxScoreSectionPlotter::setColor( ofColor color ){
    this->color = color;
    this->colorDim = color.getLerped( ofColor::black, 0.6f);
}


void ofxScoreSectionPlotter::setup( int w, int graphH, int outputs, pdsp::ScoreSection &assignedSection) {

    section = &assignedSection;

    headerH = 20;
    
    fbo.allocate(w+2, graphH+2+headerH, GL_RGBA);
    fbo.begin();
    ofClear(255,255,255, 0);
    fbo.end();
    
    rangeMin.resize(outputs);
    for( float & value : rangeMin ){
        value = 0.0f;
    }
        
    rangeMax.resize(outputs);
    for( float & value : rangeMax ){
        value = 1.0f;
    }
        
    lastX.resize(outputs);
    for( float & value : lastX ){
        value = 0.0f;
    }

    lastY.resize(outputs);
    for( float & value : lastY ){
        value = 0.0f;
    }

    this->outputs = outputs;



    width = w;
    height = graphH + headerH;
    outH = (float)(graphH) / (float)outputs;
    
    lastSeqNumber = -1;
    lastSeqID = -1;
    timeMult = 1.0;

}

void ofxScoreSectionPlotter::update() {
    
    int seqCurrent = section->meter_current();
    //cout<<"seq current id = "<<seqCurrent<<"\n";

    if( seqCurrent != lastSeqNumber  ){
        
        if(seqCurrent == -1){
            clearGraphics();
            
        }else{
            const pdsp::Sequence & seq = section->getSequence(seqCurrent);
            updateGraphics( seq );
        }
    
    }else if(lastSeqNumber!=-1) {
        const pdsp::Sequence & seq = section->getSequence(seqCurrent);
        int seqID = seq.getChangeID();
        if(seqID != lastSeqID){
            updateGraphics( seq );
        }
        lastSeqID = seqID;
    }
    
    lastSeqNumber = seqCurrent;
    
}

void ofxScoreSectionPlotter::updateGraphics( const pdsp::Sequence & seq ){
                
        timeMult = 1.0 / seq.length();
        
        for( int i=0; i<outputs; ++i ){
            lastX[i]     = 0.0f;
            lastY[i]     = outH;
        }

        fbo.begin();
        ofNoFill();
        ofClear(255,255,255, 0);
        ofPushMatrix();
        ofSetColor(255);
        ofTranslate(1,1);
        
        cells = section->getCellsNumber();
        headerW = width / (float) cells;
        
        for(int i=0; i<cells; ++i){
            if(section->meter_current() == i){
                headerX = headerW*i;
            }else if(section->meter_next() == i ){
                ofSetColor(40);
                ofFill();
                ofDrawRectangle(headerW*i, 0, headerW, headerH);
            }
            ofNoFill();
            ofSetColor(255);            
            ofDrawRectangle(headerW*i, 0, headerW, headerH);

        }
        
        ofTranslate(0, headerH);
        ofSetColor(150);
        
        const vector<pdsp::ScoreMessage> score = seq.getScore(); 

        ofFill();
        for(const pdsp::ScoreMessage & message : score ){
            if(message.lane < outputs ){
                int out = message.lane;
                float posX = message.time * timeMult * width;
                float posY = ofMap( message.value, rangeMin[out], rangeMax[out], outH*static_cast<float>(out+1), outH*static_cast<float>(out) );
                //ofDrawLine(lastX[out], lastY[out], posX, lastY[out]);
                ofDrawRectangle(lastX[out], lastY[out], posX-lastX[out], outH*static_cast<float>(out+1) - lastY[out]);

                lastX[out] = posX;
                lastY[out] = posY;
            }
        }
        
        for(int out=0; out<outputs; ++out){ // draws the last values
            //ofDrawLine(lastX[out], lastY[out], width, lastY[out])
            ofDrawRectangle(lastX[out], lastY[out], width-lastX[out], outH*static_cast<float>(out+1) - lastY[out]);
        }
        
        ofNoFill();
        ofSetColor(255);
        for(int out=0; out<outputs; ++out){ // draws the last values
            ofDrawRectangle(0, outH*static_cast<float>(out), width, outH);
        }
        ofPopMatrix();
        fbo.end();
}

void ofxScoreSectionPlotter::clearGraphics (){
    fbo.begin();
    ofNoFill();
    ofClear(255,255,255, 0);
       
        ofSetColor(255);
        ofTranslate(1,1);
        
        cells = section->getCellsNumber();
        headerW = width / (float) cells;
        for(int i=0; i<cells; ++i){
            
            if(section->meter_current() == i){
                headerX = headerW*i;
            }else if(section->meter_next() == i ){
                ofSetColor( 40 );
                ofFill();
                ofDrawRectangle(headerW*i, 0, headerW, headerH);
                ofNoFill();
                ofSetColor( 255 );
            }
            
            ofDrawRectangle(headerW*i, 0, headerW, headerH);

            
        }
        ofDrawRectangle( 0, headerH, width, height - headerH);
    
    fbo.end();
}

void ofxScoreSectionPlotter::draw() {
    
    ofSetColor(colorDim);
    float playhead = section->meter_playhead() * timeMult;
    float lineX = playhead * width;
    ofFill(); 
    ofDrawRectangle( headerX, 0, headerW*playhead, headerH);
    
    ofSetColor(color);
    fbo.draw( 0, 0);
    if(outputs != 0){  
        ofNoFill();
        ofDrawLine(lineX, headerH, lineX, height);
    }

}


void ofxScoreSectionPlotter::draw( int x, int y ) {
    ofPushMatrix();
    ofTranslate(x, y);
    draw();
    ofPopMatrix();
}

void ofxScoreSectionPlotter::setRange( int index, float min, float max ){
    if(index<0) index = 0;
    if(index>=outputs) index = outputs-1;
    
    rangeMin[index] = min;
    rangeMax[index] = max;
}

float ofxScoreSectionPlotter::getCellWidth() const {
    return headerW;
}

int   ofxScoreSectionPlotter::getCellsNumber() const {
    return cells;
}
