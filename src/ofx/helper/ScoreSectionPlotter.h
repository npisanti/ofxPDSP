
#ifndef OFXPDSP_SCORESECTION_PLOTTER_H_INCLUDED
#define OFXPDSP_SCORESECTION_PLOTTER_H_INCLUDED

#include "ofMain.h"
#include "../sequencer/ScoreSection.h"
#include "../sequencer/Sequence.h"

/*!
@brief utility class to display a score section active sequence and its output values
*/    

namespace pdsp { namespace helper {

class EngineGraphics;

class ScoreSectionPlotter {
    friend class pdsp::helper::EngineGraphics;
    
public:
    ScoreSectionPlotter();

    /*!
    @brief set ups the class
    @param[in] w width of the graphics
    @param[in] graphH height of the graphics, header excluded
    @param[in] outputs number of outputs to monitor
    @param[in] assignedSection 
    */
    void setup( int w, int graphH, int outputs, pdsp::ScoreSection &assignedSection);

    /*!
    @brief this is mandatory in your app update() for the plotters to work
    */    
    void update();

    /*!
    @brief display the ScoreSectionPlotter
    */
    void draw();

    /*!
    @brief display the ScoreSectionPlotter at the given coordinates
    @param[in] x x coordinate
    @param[in] y y coordinate
    */
    void draw( int x, int y );

    /*!
    @brief set the range of the displayed values for the given output
    @param[in] index index of the output
    @param[in] min minimum plotted value
    @param[in] max maximum plotted value
    */    
    void setRange( int index, float min, float max );
    
    /*!
    @brief sets the color of the graphics
    @param color color to set
    */     
    void setColor( ofColor color );
    
    
private:

    void updateGraphics( const pdsp::Sequence & seq );
    void clearGraphics ();

    float getCellWidth() const;
    int   getCellsNumber() const;

    vector<float> lastX;
    vector<float> lastY;
    vector<float> rangeMin;
    vector<float> rangeMax;

    float timeMult;
    
    ofFbo fbo;
    int lastSeqNumber;
    int lastSeqID;
  
  
    int cells;
    float headerH;
    float headerW;
    float headerX;
    
    float width;
    float height;
    float outH;
  
    
    int outputs;
    
    pdsp::ScoreSection * section;
    
    ofColor color;
    ofColor colorDim;
    
};

}} // end namespaces

#endif // OFXPDSP_SCORESECTION_PLOTTER_H_INCLUDED
