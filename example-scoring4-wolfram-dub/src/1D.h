
#pragma once


#include "ofMain.h"
#include "ofxPDSP.h"

class Automaton1D  {
    
public:

    void setup(int w, int h, int cells, int generations, int rule );
        
    void resizeGraphics(int w, int h);

    void advance() noexcept;
        
    void draw();
    
    void draw( int x, int y);
    
    void setRule( int rule ) noexcept;
    int  getRule( ) const;

    void initRandom( float density ) noexcept;
    void initCanonical() noexcept;
    void clear() noexcept;

    int cell( const int & x, const int & y ) noexcept;
    
    const int generations() const;
    const int cells() const;
    
 
private:

    vector<vector<int>> CA;    
   
    int xCells;
    int generationsNum;

    int rule;
    vector<int> wrules;
    
    int caGenerationIndex;

    float cellW;
    float cellH;
    ofFbo gridFbo;
    ofFbo cellsFbo;
    atomic<bool> updateCellsFbo;
    

};

