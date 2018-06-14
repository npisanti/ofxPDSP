
#include "random.h"

namespace pdsp{

std::random_device randomDev;
std::mt19937 globalRandom(randomDev());
std::uniform_real_distribution<float> rdist_unipolar (0.0f, 1.0f);
std::uniform_real_distribution<float> rdist_unipolarInclusive ( 0.0f, std::nextafter(1.0f, std::numeric_limits<float>::max() ) );
std::uniform_real_distribution<float> rdist_bipolar (-1.0f,  std::nextafter(1.0f, std::numeric_limits<float>::max() ) );

void seedGlobalRandom(int seed){
        globalRandom = std::mt19937 (seed);
}

float randomUnipolar(){
        return rdist_unipolar(globalRandom);
}

float randomUnipolarInclusive(){
        return rdist_unipolarInclusive(globalRandom);
}

float randomBipolar(){
        return rdist_bipolar(globalRandom);
}

int randomInt(){
        return globalRandom();
}

int dice(int size){
    std::uniform_int_distribution<int> unipolarInt (0, size-1);
    return unipolarInt(globalRandom);
}

int randomBipolarInt(int min, int maxInclusive){
    std::uniform_int_distribution<int> bipolarInt (min, maxInclusive);
    return bipolarInt(globalRandom);
}

bool chance(float value){
    uint_fast32_t chanceInt = static_cast<uint_fast32_t>( value * static_cast<float>(globalRandom.max()) );
    if ( globalRandom() <= chanceInt ){
        return true;
    }else{
        return false;
    }
}


}//END NAMESPACE

