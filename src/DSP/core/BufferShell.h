
// BufferShell.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_CORE_BUFFERSHELL_H_INCLUDED
#define PDSP_CORE_BUFFERSHELL_H_INCLUDED

#include "Preparable.h"
#include "../../flags.h"

namespace pdsp {

    /*!
    @brief A buffer of memory-aligned floats that resize itself when the units are prepared.
    
    This is a class that contain a buffer of memory-aligned floats that is prepared with all the units and nodes and resize itself according to the expectedBufferSize and an oversample factor.
    
    */   
    
class BufferShell : public Preparable {
        
public:
    BufferShell();
    BufferShell(const BufferShell& other);
    BufferShell& operator= (const BufferShell& other);
    ~BufferShell();
        
    /*!
    @brief sets the oversample factor that multiply the buffer size
    @param[in] newOversampleLevel the oversample level, 2 for 2x, 4 for 4x, etc etc
    */    
    void setOversampleLevel(int overSample);

    /*!
    @brief this is the buffer of floats 
    */
    float* buffer;

protected:
    void prepareToPlay (int expectedBufferSize, double sampleRate) override ;
    void releaseResources () override;

private:
    int overSample;
};

}




#endif  // BUFFERSHELL_H_INCLUDED
