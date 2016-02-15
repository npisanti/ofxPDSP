
// Clockable.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_MESSAGES_CLOCKABLE_H_INCLUDED
#define PDSP_MESSAGES_CLOCKABLE_H_INCLUDED

namespace pdsp {
    

/*!
@brief Global transport and tempo interface
*/      

class Clockable{
    friend class ScoreProcessor;
    friend void prepareAllToPlay(int expectedBufferSize, double sampleRate);
    
public:

    /*!
    @brief returns how many bars you advance for each sample
    */       
    static float getBarsPerSample();
    
    
    /*!
    @brief returns the global transport playhead in bars
    */           
    static float getGlobalBarPosition();


    /*!
    @brief returns the time in milliseconds for one bar.
    */           
    static float getOneBarTimeMs();
    

    /*!
    @brief returns true if the transport is not stopped or paused
    */ 
    static bool isPlaying();
    
    
    
    //static void setGlobalFramePosition( long framePosition);
    //static void addToGlobalFramePosition( int samples);
    //static void setPlayingState(bool playing);
        
private:

    static void setTempo(float tempo);

    static float globalBarPosition;

    static float barTimeMs;    

    static bool playing;
    static float globalTempo;
    static double clockBaseSampleRate;

    static void changeSampleRate(double sampleRate);

    static float masterInc; //increment time for  192/192
                            //also number of bars per sample

    static void updateMasterInc();

    //static long globalFramePosition;
};


//void setTempo(float tempo);

//void pauseTransport();
//void stopTransport();
//void startTransport();
//void updateTransport( int bufferSize );


}





#endif  // CLOCKABLE_H_INCLUDED
