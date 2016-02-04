
// message.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_MESSAGES_MESSAGE_H_INCLUDED
#define PDSP_MESSAGES_MESSAGE_H_INCLUDED

namespace pdsp{

/*!
    @cond HIDDEN_SYMBOLS
*/
class ControlMessage{
public:
    ControlMessage(float value, int sample) : value(value), sample(sample){};
    ControlMessage() : value(0.0f), sample(0) {};
    
    float value;
    int sample;
};

/*!
    @endcond
*/

}//END NAMESPACE


#endif  // PDSP_MESSAGES_MESSAGE_H_INCLUDED
