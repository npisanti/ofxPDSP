

#include "SmoothInterpolator.h"

pdsp::SmoothInterpolator pdsp::SmoothInterpolator::smoothInterpolator = SmoothInterpolator();

float pdsp::SmoothInterpolator::interpolate(const float* table, const float &index, const int &tableLen) noexcept {
    
    
    uint32_t index_int = static_cast<uint32_t> (index);
    float mu = index - index_int;
    float x1 = table[index_int];
    float x2 = table[index_int+1];
    
    return interpolate_smooth(x1, x2, mu);
    
    
}
