
#include "incrementCalculator.h"

namespace pdsp {
    
    ofx::f128 calc_inc_kernel::divideByTwelve = ofx::m_rcp( ofx::m_set1( 12.0f ) );
    const float calc_inc_kernel::divideByTwelve_f = (1.0f / 12.0f);

}
