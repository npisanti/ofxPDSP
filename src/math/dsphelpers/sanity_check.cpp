
#include "sanity_check.h"


void pdsp::vect_sanity_check( float * samples, int len){
    
    uint32_t * samples_ip = reinterpret_cast<uint32_t*> ( samples );
    
    for(int i=0; i<len; i++){
        uint32_t sample = *samples_ip;
        uint32_t exponent = sample & 0x7F800000;
        int aNaN = exponent < 0x7F800000;
        int aDen = exponent > 0;
        *samples_ip++ = sample * ( aNaN & aDen );
    }
    
}
