

#include "Preparable.h"



namespace pdsp{

std::vector<Preparable*>  Preparable::constructionVector = std::vector<Preparable*> ();

bool Preparable::dynamicConstruction = false;
int Preparable::globalBufferSize = 64;
double Preparable::globalSampleRate = 22050.0;
int Preparable::initOversampleLevel = 1;
int Preparable::turnBufferSize = 0;


Preparable::Preparable(){
        constructionVector.push_back(this);
};


Preparable::~Preparable()
{
        //remove this handle from the vector
        std::vector<Preparable*>::iterator it = constructionVector.begin();

        while (it != constructionVector.end())
        {
                if ((*it) == this)
                {
                        constructionVector.erase(it);
                        break;
                }
                it++;
        }
};

void Preparable::setTurnBufferSize(int turnBufferSize){
        Preparable::turnBufferSize = turnBufferSize;
}

void prepareAllToPlay(int expectedBufferSize, double sampleRate)
{
#ifndef PDSP_AUDIO_PLUGIN    
        ofx_activate_denormal_flush();
#endif
        Clockable::changeSampleRate(sampleRate);

        Preparable::dynamicConstruction = true;
        Preparable::globalBufferSize = expectedBufferSize;
        Preparable::globalSampleRate = sampleRate;

        for (Preparable* &node : Preparable::constructionVector){
                node->prepareToPlay(expectedBufferSize, sampleRate);
        }
}
    
void releaseAll()
{
        for (Preparable* &node : Preparable::constructionVector){
                node->releaseResources();
        }
}

void setInitOversample(int initOversample){
        Preparable::initOversampleLevel = initOversample;
}
 
 
 
double Preparable::getGlobalSampleRate() {
    return globalSampleRate;
}
 
}//END NAMESPACE

