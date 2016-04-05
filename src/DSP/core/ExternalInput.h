
// ExternalInput.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_CORE_EXTERNALINPUT_H_INCLUDED
#define PDSP_CORE_EXTERNALINPUT_H_INCLUDED

#include "BasicNodes.h"
#include "PatchNode.h"


namespace pdsp{

    /*!
    @brief Unit to copy values from an external audio input and convert them to a patchable output.
    
    This class let you copy float values from an audio input callback and store them in an OutputNode ready to be patched.
    */   
    
class ExternalInput : public Unit  {

public:
        ExternalInput();
        ExternalInput(const ExternalInput & other);
        ~ExternalInput();
        
        /*!
        @brief copies input from a non-interleaved array
        @param[in] input a pointer to the array to copy
        @param[in] bufferSize number of samples to copy
        */    
        void copyInput(float* input, const int & bufferSize) noexcept;
        
        /*!
        @brief copies input from a interleaved array
        @param[in] input a pointer to the array to copy
        @param[in] index index of the channel to copy
        @param[in] channels number of channels interleaved into the array
        @param[in] bufferSize number of samples to copy
        */    
        void copyInterleavedInput(float* input, int index, int channels, const int & bufferSize) noexcept;
        
        /*!
        @brief Sets "signal" as selected output and return this Unit ready to be patched. This is the default output. This output contains the copied values, and if values have not been copied yet or the copy callback stopped it is set to a constant rate 0.0f.
        */ 
        Patchable& out_signal();
        
private:
        void process(int bufferSize) noexcept override;
        void prepareUnit( int expectedBufferSize, double sampleRate ) override;
        void releaseResources() override;

        float* buffer;
        PatchOutputNode output;
        bool inputUpdated;

};

 

}//END NAMESPACE


#endif  // PDSP_CORE_EXTERNALINPUT_H_INCLUDED
