
// operators.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_CORE_OPERATORS_H_INCLUDED
#define PDSP_CORE_OPERATORS_H_INCLUDED

#include "BasicNodes.h"


namespace pdsp{
    
//CONNECT
void operator>> (OutputNode& output, InputNode& input);
Patchable& operator>> (OutputNode& output, Patchable& input);
void operator>> (float value, InputNode& input);
OutputNode& operator>> (float lhs, ValueNode& node);
void operator>> (Patchable& output, InputNode& input);
Patchable& operator>> (Patchable& output, Patchable& input);
Patchable& operator>> (float value, Patchable& input);
    
//DISCONNECT
void operator!= ( InputNode&  input,  OutputNode& output);
void operator!= ( OutputNode& output, InputNode&  input);
void operator!= ( Patchable&  input,  OutputNode& output);
void operator!= ( OutputNode& output, Patchable&  input);
void operator!= ( InputNode&  input,  Patchable&  output);
void operator!= ( Patchable&  output, InputNode&  input);

//MULTIPLY
OutputNode& operator* (OutputNode& output, float multiplier);
Patchable& operator* (Patchable& patch, float multiplier);

//assignment from left operator
void operator<< (InputNode& input, float value);
void operator<< (ValueNode& node, float value);
void operator<<= (InputNode& input, float value );
void operator<<=(ValueNode& node, float value);

    
}



#endif  // OPERATORS_H_INCLUDED
