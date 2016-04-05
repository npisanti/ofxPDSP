
// leftSum.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_CORE_LEFTSUM_H_INCLUDED
#define PDSP_CORE_LEFTSUM_H_INCLUDED

#include "BasicNodes.h"
#include "operators.h"
#include <vector>


namespace pdsp{

/*!
    @cond HIDDEN_SYMBOLS
*/

enum sumNodePtrType_t { Output, Patch, Float };

class sumNodePtr
{
public:
        sumNodePtr(void* pointer, sumNodePtrType_t type)
                : pointer(pointer), floatValue(0.0f), type(type){};

        sumNodePtr(float value, sumNodePtrType_t type)
                : pointer(nullptr), floatValue(value), type(type){};

        void* pointer;
        float floatValue;
        sumNodePtrType_t type;
};

/*!
    @endcond
*/


std::vector<sumNodePtr>* operator+ (float left, OutputNode& right);
std::vector<sumNodePtr>* operator+ (float left, Patchable& right);
std::vector<sumNodePtr>* operator+ (OutputNode& left, OutputNode& right);
std::vector<sumNodePtr>* operator+ (OutputNode& left, Patchable& right);
std::vector<sumNodePtr>* operator+ (OutputNode& left, float right);
std::vector<sumNodePtr>* operator+ (Patchable& left, OutputNode& right);
std::vector<sumNodePtr>* operator+ (Patchable& left, Patchable& right);
std::vector<sumNodePtr>* operator+ (Patchable& left, float right);
std::vector<sumNodePtr>* operator+ (std::vector<sumNodePtr>* left, OutputNode& right);
std::vector<sumNodePtr>* operator+ (std::vector<sumNodePtr>* left, Patchable& right);

void operator>> (std::vector<sumNodePtr>* left, InputNode& right);
//void operator>>= (std::vector<sumNodePtr>* left, InputNode& right);
Patchable& operator>> (std::vector<sumNodePtr>* left, Patchable& right);
//Patchable& operator>>= (std::vector<sumNodePtr>* left, Patchable& right);


static std::vector<sumNodePtr> _leftSumVector;

}

#endif  // LEFTSUM_H_INCLUDED
