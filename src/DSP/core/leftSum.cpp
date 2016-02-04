

#include "leftSum.h"


//pdsp::_leftSumVector.reserve(16);

namespace pdsp{

std::vector<sumNodePtr>* operator+ (float left, OutputNode& right)
{
    std::vector<sumNodePtr>* toReturn = &_leftSumVector;
    toReturn->clear();
    toReturn->push_back(sumNodePtr(left, Float));
    toReturn->push_back(sumNodePtr(reinterpret_cast<void*>(&right), Output));
    return toReturn;
}

std::vector<sumNodePtr>* operator+ (float left, Patchable& right)
{
    std::vector<sumNodePtr>* toReturn = &_leftSumVector;
    toReturn->clear();
    toReturn->push_back(sumNodePtr(left, Float));
    toReturn->push_back(sumNodePtr(reinterpret_cast<void*>(&right), Patch));
    return toReturn;
}


std::vector<sumNodePtr>* operator+ (OutputNode& left, OutputNode& right)
{
    std::vector<sumNodePtr>* toReturn = &_leftSumVector;
    toReturn->clear();
    toReturn->push_back(sumNodePtr(reinterpret_cast<void*>(&left), Output));
    toReturn->push_back(sumNodePtr(reinterpret_cast<void*>(&right), Output));
    return toReturn;
}

std::vector<sumNodePtr>* operator+ (OutputNode& left, Patchable& right)
{
    std::vector<sumNodePtr>* toReturn = &_leftSumVector;
    toReturn->clear();
    toReturn->push_back(sumNodePtr(reinterpret_cast<void*>(&left), Output));
    toReturn->push_back(sumNodePtr(reinterpret_cast<void*>(&right), Patch));
    return toReturn;
}

std::vector<sumNodePtr>* operator+ (OutputNode& left, float right)
{
    std::vector<sumNodePtr>* toReturn = &_leftSumVector;
    toReturn->clear();
    toReturn->push_back(sumNodePtr(reinterpret_cast<void*>(&left), Output));
    toReturn->push_back(sumNodePtr(right, Float));
    return toReturn;
}

std::vector<sumNodePtr>* operator+ (Patchable& left, OutputNode& right)
{
    std::vector<sumNodePtr>* toReturn = &_leftSumVector;
    toReturn->clear();
    toReturn->push_back(sumNodePtr(reinterpret_cast<void*>(&left), Patch));
    toReturn->push_back(sumNodePtr(reinterpret_cast<void*>(&right), Output));
    return toReturn;
}

std::vector<sumNodePtr>* operator+ (Patchable& left, Patchable& right)
{
    std::vector<sumNodePtr>* toReturn = &_leftSumVector;
    toReturn->clear();
    toReturn->push_back(sumNodePtr(reinterpret_cast<void*>(&left), Patch));
    toReturn->push_back(sumNodePtr(reinterpret_cast<void*>(&right), Patch));
    return toReturn;
}

std::vector<sumNodePtr>* operator+ (Patchable& left, float right)
{
    std::vector<sumNodePtr>* toReturn = &_leftSumVector;
    toReturn->clear();
    toReturn->push_back(sumNodePtr(reinterpret_cast<void*>(&left), Patch));
    toReturn->push_back(sumNodePtr(right, Float));
    return toReturn;
}


std::vector<sumNodePtr>* operator+ (std::vector<sumNodePtr>* left, OutputNode& right)
{
    left->push_back(sumNodePtr(reinterpret_cast<void*>(&right), Output));
    return left;
}

std::vector<sumNodePtr>* operator+ (std::vector<sumNodePtr>* left, Patchable& right)
{
    left->push_back(sumNodePtr(reinterpret_cast<void*>(&right), Patch));
    return left;
}


void operator>> (std::vector<sumNodePtr>* left, InputNode& right)
{
    for (sumNodePtr ptr : *left){
        if (ptr.type == Output){
            *(reinterpret_cast<OutputNode *> (ptr.pointer)) >> right;
        }
        else if (ptr.type == Patch){
            *(reinterpret_cast<Patchable *> (ptr.pointer)) >> right;
        }
        else if (ptr.type == Float){
            ptr.floatValue >> right;
        }
    }
}

Patchable& operator>> (std::vector<sumNodePtr>* left, Patchable& right)
{
    left >> right.getSelectedInput();
    right.resetInputToDefault();
    return right;
}

/*
void operator>>= (std::vector<sumNodePtr>* left, InputNode& right)
{
    right.disconnectAll();

    for (sumNodePtr ptr : *left){
        if (ptr.type == Output){
            *(reinterpret_cast<OutputNode *> (ptr.pointer)) >> right;
        }
        else if (ptr.type == Patch){
            *(reinterpret_cast<Patchable *> (ptr.pointer)) >> right;
        }
        else if (ptr.type == Float){
            ptr.floatValue >> right;
        }
    }
    
}



Patchable& operator>>= (std::vector<sumNodePtr>* left, Patchable& right)
{
    right.getSelectedInput().disconnectAll();
    left >> right.getSelectedInput();
    right.resetInputToDefault();
    return right;
}
*/

}//END NAMESPACE
