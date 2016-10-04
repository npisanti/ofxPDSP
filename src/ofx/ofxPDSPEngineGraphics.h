
#ifndef OFXPDSP_ENGINEGRAPHICS_H_INCLUDED
#define OFXPDSP_ENGINEGRAPHICS_H_INCLUDED

#include "ofMain.h"
#include "../sequencer/ScoreSection.h"
#include "../sequencer/ScoreProcessor.h"
#include "../sequencer/Sequence.h"
#include "ofxScoreSectionPlotter.h"

/*!
@brief utility class to display a score section active sequence and its output values
*/    

class ofxPDSPEngineGraphics {
    friend class ofxPDSPEngine;
    
public:

    /*!
    @brief set ups the class
    @param[in] w width of the graphics

    */
    void setup( int w, int hFold, std::initializer_list<int> sectionsH, std::initializer_list<int> sectionsOuts );


    /*!
    @brief sets the color of the graphics
    @param color color to set
    */    
    void setColor( ofColor color );

    /*!
    @brief sets the position of the graphics
    @param x
    @param y
    */      
    void setPosition( int x, int y );

    /*!
    @brief displays the ofxScoreSectionPlotter
    */
    void draw();

    /*!
    @brief display the ofxScoreSectionPlotter at the given coordinates
    @param[in] x x coordinate
    @param[in] y y coordinate
    */
    void draw( int x, int y );

    /*!
    @brief set the range of the displayed values for the given output
    @param[in] section section of the values to set
    @param[in] index index of the output
    @param[in] min minimum plotted value
    @param[in] max maximum plotted value
    */    
    void setRange(int section, int index, float min, float max );
    
    /*!
    @brief set ups the keyboard control for the score
    @param initArray an inlined two dimensional array of chars
    @param stopAndPlayKey key for starting and stopping the score
    @param quantize true for activating the quantization when launching sequences with the key
    @param quantizeTime grid divison of quantization when launching sequences with the key
    */             
    void keys( std::initializer_list<std::initializer_list<char>> initArray, int stopAndPlayKey, bool quantize = false, double quantizeTime = 1.0 );
    
    /*!
    @brief set ups the keyboard control for the score, with indipendent quantize times for each section
    @param initArray an inlined two dimensional array of chars
    @param stopAndPlayKey key for starting and stopping the score
    @param quantizeArray quantize time for each section
    */             
    void keys( std::initializer_list<std::initializer_list<char>> initArray, int stopAndPlayKey, std::initializer_list<double> quantizeArray);
    
    /*!
    @brief disables the key control
    */
    void disableKeys();
        
 
private:

    void keyPressed ( ofKeyEventArgs& eventArgs );   
    void drawGraphics();
    
    void setParent ( pdsp::ScoreProcessor & score );    
    void updateGraphics( const pdsp::Sequence & seq );
    void clearGraphics ();

    vector<ofxScoreSectionPlotter> plotters;
    vector<int> sectionsHeights;

    ofColor color;
    
    int hFold;
    int width;
    
    pdsp::ScoreProcessor * score;
    
    vector<vector<int>> assignedKeys;
    
    bool quantize;
    vector<double> quantizeTime;
    
    int blink;
    
    int posX;
    int posY;
    
    int playStopKey;
    
};



#endif // OFXPDSP_ENGINEGRAPHICS_H_INCLUDED
