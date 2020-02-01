
#include "Automaton1D.h"
#include "ofxPDSP.h"

Automaton1D::Automaton1D(){
    setRule( 60 );
    clear();
}

void Automaton1D::setRule( int rule ) noexcept { 
    // init wolfram rules from number bits
    this->rule = rule;
    int rulebits = rule;
    for(int i=0; i<8; ++i){             // inits the rules
        wrules[i] = rulebits & 0x0001;  // take just the lowest bit
        rulebits = rulebits>>1;         // bitshift by one
    }
}

void Automaton1D::advance() noexcept{ // update the automaton
    for( int y=CA_HEIGHT-1; y>0; --y ){
        for( int x=0; x<CA_WIDTH; ++x){
            CA[y][x] = CA[y-1][x];    
        }
    }

    for( int x=0; x<CA_WIDTH; ++x){
        int xprev = x-1;
        if (xprev==-1) xprev = CA_WIDTH-1;
        int xnext = x+1;
        if(xnext==CA_WIDTH) xnext = 0;

        int cellRule = CA[1][xprev] * 4 
                    + CA[1][x] * 2 
                    + CA[1][xnext];
                    
        CA[0][x] = wrules[cellRule]; 
    }
}

void Automaton1D::random( float density, pdsp::Function & seq ) noexcept{
    clear();
    for( int x=0; x<CA_WIDTH; ++x ){
        if(x%2==0){
            // pdsp has its own random generator 
            CA[0][x] = seq.chance(density) ? 1 : 0;  // just odd    
        }else{
            CA[0][x] = 0;         
        }
    }        

    for( int i=0; i<CA_HEIGHT-1; ++i ){
        advance();
    }
}

void Automaton1D::clear() noexcept{
    for( int y=0; y<CA_HEIGHT; ++y ){
        for( int x=0; x<CA_WIDTH; ++x ){
            CA[y][x] = 0;
        }        
    }    
}


