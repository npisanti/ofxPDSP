

#include "LinearInterpolator.h"

pdsp::LinearInterpolator pdsp::LinearInterpolator::linearInterpolator = LinearInterpolator();


float pdsp::LinearInterpolator::interpolate(const float* table, const float &index, const int &tableLen) noexcept {
    
    
     int index_int = static_cast<int> (index);
     float mu = index - index_int;
     float x1 = table[index_int];
     float x2 = table[index_int+1];
     
    return interpolate_linear(x1, x2, mu);
     
    
}
