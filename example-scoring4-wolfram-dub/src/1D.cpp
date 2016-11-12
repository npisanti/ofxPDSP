
// 1D.cpp
// Nicola Pisanti, GPLv3 License, 2016

#include "1D.h"

void Automaton1D::resizeGraphics(int w, int h) {
    gridFbo.allocate(w, h, GL_RGBA);
    gridFbo.begin();
        ofClear(255,255,255, 0);
        ofTranslate( 1, 1);
        cellW = (float)(w-2) / (float)xCells; 
        cellH = (float)(h-2) / (float)generationsNum; 
        ofSetColor(100);
        ofNoFill();
        for(int y = 0; y<generationsNum; ++y){
            for(int x = 0; x<xCells; ++x){
                ofDrawRectangle( x*cellW, y*cellH, cellW, cellH);
            }
        }
        
    gridFbo.end();
    
    cellsFbo.allocate(w, h, GL_RGBA);
    cellsFbo.begin();
        ofClear(255,255,255, 0);
    cellsFbo.end();
  
    updateCellsFbo = true;
    
}


void Automaton1D::setup(int w, int h, int cells, int generationsNum, int rule ){

    xCells = cells;
    this->generationsNum = generationsNum;

    wrules.resize(8);
    setRule( rule );

    caGenerationIndex = 0;
    CA.resize(generationsNum); // 8 different generatiosn
    for( vector<int> & v: CA ){
        v.resize(cells); 
    }
    
    resizeGraphics( w, h );
}

    
int Automaton1D::cell( const int & x, const int & y ) noexcept {
    return CA[y][x];
}

//--------------------------------------------------------------
void Automaton1D::setRule( int rule ) noexcept { // init wolfram rules from number bits
    this->rule = rule;
    int rulebits = rule;
    for(int i=0; i<8; ++i){ // inits the rules
        wrules[i] = rulebits & 0x0001;  // take just the lowest bit
        rulebits = rulebits>>1;             // bitshift by one
    }
    //ruleChanged = false;
}

//--------------------------------------------------------------
void Automaton1D::advance() noexcept{ // update the automaton
    
    int oldGeneration = caGenerationIndex;
    caGenerationIndex--;
    if(caGenerationIndex < 0){
        caGenerationIndex = generationsNum - 1;
    }
    
    int max = (int) (CA[caGenerationIndex].size());
    for(int x = 0; x < max; ++x){
        int xprev = x-1;
        if (xprev==-1) xprev = max-1;
        int xnext = x+1;
        if(xnext==max) xnext = 0;
        
        int cellRule = CA[oldGeneration][xprev] * 4 
                    + CA[oldGeneration][x] * 2 
                    + CA[oldGeneration][xnext];
                    
        CA[caGenerationIndex][x] = wrules[cellRule]; 
    }     
    updateCellsFbo = true;
    
}


void Automaton1D::draw(){
        

    
    if( updateCellsFbo ){
        cellsFbo.begin();
            ofClear(255,255,255, 0);
            ofTranslate(1,1);
            ofFill();
            ofSetColor(255);
            int genIndex = caGenerationIndex;
            for(int y = 0; y<generationsNum; ++y){
                for(int x = 0; x<xCells; ++x){
                    if(CA[genIndex][x]==1){
                        ofDrawRectangle( x*cellW, y*cellH, cellW, cellH);
                    }
                }
                genIndex++; 
                if(genIndex==generationsNum) genIndex = 0;
            }        

        cellsFbo.end();

        updateCellsFbo = false;
    }

    cellsFbo.draw(0, 0);
    gridFbo.draw( 0, 0);

}

void Automaton1D::initRandom( float density ) noexcept{
    for( vector<int> & v: CA ){
        for( int i=0; i < (int)v.size(); ++i){
            v[i] = 0;
        }
    }        
    
    for( int x=0; x < (int)(CA[caGenerationIndex].size()); ++x ){
        // pdspChange(float value) controls the chance of having an alive cell
        if(x%2==0){
            CA[caGenerationIndex][x] = pdspChance(density) ? 1 : 0;                     
        }else{
            CA[caGenerationIndex][x] = 0;          
        }

    }
    
    // always also make the first active
    CA[caGenerationIndex][0] = 1;   
    updateCellsFbo = true;
}

void Automaton1D::clear() noexcept{
    for( vector<int> & v: CA ){
        for( int i=0; i < (int)v.size(); ++i){
            v[i] = 0;
        }
    }    
    updateCellsFbo = true;    
}


void Automaton1D::initCanonical() noexcept{
    for( vector<int> & v: CA ){
        for( int i=0; i < (int)v.size(); ++i){
            v[i] = 0;
        }
    }        

    for( int x=0; x < (int)(CA[caGenerationIndex].size()); ++x ){
        // pdspChange(float value) controls the chance of having an alive cell
        if(x%2==0){
            CA[caGenerationIndex][x] = pdspChance(0.25f) ? 1 : 0;                     
        }else{
            CA[caGenerationIndex][x] = 0;          
        }

    }

    // always also make the first active
    CA[caGenerationIndex][0] = 1;  
     
    updateCellsFbo = true;
}


const int Automaton1D::generations() const {
    return generationsNum;
}

const int Automaton1D::cells() const {
    return xCells;
}

void Automaton1D::draw( int x, int y) {
    ofPushMatrix();
    ofTranslate( x, y );
        draw();
    ofPopMatrix();
}
    
int Automaton1D::getRule( ) const {
    return rule;
}
    
