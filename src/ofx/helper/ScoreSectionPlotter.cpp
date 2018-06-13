
#include "ScoreSectionPlotter.h"

namespace pdsp { namespace helper {

ScoreSectionPlotter::ScoreSectionPlotter() {
    setColor( ofColor(255, 255, 255) );
}

void ScoreSectionPlotter::setColor( ofColor color ){
    this->color = color;
    this->colorDim = color.getLerped( ofColor::black, 0.6f);
}


void ScoreSectionPlotter::setup( int w, int graphH, int outputs, pdsp::ScoreSection &assignedSection) {

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

void ScoreSectionPlotter::update() {
    
    int seqCurrent = section->meter_current();

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

void ScoreSectionPlotter::updateGraphics( const pdsp::Sequence & seq ){
                
        timeMult = 1.0 / seq.length();
        


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

        
        const vector<pdsp::ScoreMessage> score = seq.getScore(); 

        for (int out=0; out<outputs; ++out){
            ofPushStyle();
            
            lastX[out]     = 0.0f;
            lastY[out]     = outH;
            
            float hlow = outH;
            float hhigh = 0.0f;

            ofFill();
            ofSetColor(150);
            
            for(const pdsp::ScoreMessage & message : score ){
                
                if(message.lane == out ){
                   
                    float posX = message.time * timeMult * width;                    

                    float posY = ofMap( message.value, rangeMin[out], rangeMax[out], hlow, hhigh, true );

                    ofDrawRectangle(lastX[out], lastY[out], posX-lastX[out], hlow - lastY[out]);
                    
                    lastX[out] = posX;
                    lastY[out] = posY;
                }
            }
            // draw last output
            ofDrawRectangle(lastX[out], lastY[out], width-lastX[out], hlow - lastY[out]);
        
            // draws the border
            ofNoFill();
            ofSetColor(255);
            ofDrawRectangle(0, 0, width, outH);

            ofTranslate(0, outH);   
            ofPopStyle();         
        
        }
        
        ofPopMatrix();
        fbo.end();
}

void ScoreSectionPlotter::clearGraphics (){
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

void ScoreSectionPlotter::draw() {
    ofPushStyle();
    ofSetColor(colorDim);
    float playhead = section->meter_playhead() * timeMult;
    float lineX = playhead * width;
    ofFill(); 
    ofDrawRectangle( headerX, 0, headerW*playhead, headerH);
    ofNoFill();
    ofSetColor(color);
    fbo.draw( 0, 0);
    if(outputs != 0){  
        ofNoFill();
        ofDrawLine(lineX+1, headerH, lineX, height);
    }
    ofPopStyle();
}


void ScoreSectionPlotter::draw( int x, int y ) {
    ofPushMatrix();
    ofTranslate(x, y);
    draw();
    ofPopMatrix();
}

void ScoreSectionPlotter::setRange( int index, float min, float max ){
    if(index<0) index = 0;
    if(index>=outputs) index = outputs-1;
    
    rangeMin[index] = min;
    rangeMax[index] = max;
}

float ScoreSectionPlotter::getCellWidth() const {
    return headerW;
}

int   ScoreSectionPlotter::getCellsNumber() const {
    return cells;
}


}} // end namespaces 
