
#include "random.h"

namespace pdsp{


std::random_device randomDev;
std::mt19937 globalRandom(randomDev());
std::uniform_real_distribution<float> unipolar (0.0f, 1.0f);
std::uniform_real_distribution<float> bipolar (-1.0f,  std::nextafter(1.0f, std::numeric_limits<float>::max() ) );


void seedGlobalRandom(int seed){
        globalRandom = std::mt19937 (seed);
}

float randomUnipolar(){
        return unipolar(globalRandom);
}

float randomBipolar(){
        return bipolar(globalRandom);
}

int randomInt(){
        return globalRandom();
}

int randomUnipolarInt(int maxExclusive){
    std::uniform_int_distribution<int> unipolarInt (0, maxExclusive);
    return unipolarInt(globalRandom);
    //also test globalRandom%maxExclusive
}

int randomBipolarInt(int min, int maxInclusive){
    std::uniform_int_distribution<int> bipolarInt (min, maxInclusive+1);
    return bipolarInt(globalRandom);
}


}//END NAMESPACE

