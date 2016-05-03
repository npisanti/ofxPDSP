

#include "dsp_windows.h"


float* pdsp::windowTriangular( int len, int L_offset ){
    
    float* table;
    ofx_allocate_aligned(table, len+1);
    
    double L;
    if(L_offset>0){
        L = static_cast<float>(len+1);
    }else if(L_offset<0){
        L = static_cast<float>(len-1);
    }else{
        L = static_cast<float>(len);
    }
    double N = static_cast<double>(len);
    
    for(int i=0; i<len; ++i){
        double n = static_cast<double>(i);
        
        table[i] = static_cast<float>( 1.0 - fabs((n - (N-1.0)*0.5 ) / (L*0.5)) );
    }
    
    table[len] = table[0];
    return table;
}


float* pdsp::windowHann( int len ) {
    float* table;
    ofx_allocate_aligned(table, len+1);
    double N = static_cast<double>(len);
    
    for(int i=0; i<len; ++i){
        double n = static_cast<double>(i);
        
        table[i] = static_cast<float>( (1.0 - cos((M_TAU_DOUBLE*n) / (N-1.0)) ) * 0.5 );
    }
    
    table[len] = table[0];
    return table;
}


float* pdsp::windowHamming( int len, double alpha) {
    float* table;
    ofx_allocate_aligned(table, len+1);
    double N = static_cast<double>(len);
    
    //alpha is 0.54 in default
    double beta = 1.0 - alpha;
    
    for(int i=0; i<len; ++i){
        double n = static_cast<double>(i);
        
        table[i] = static_cast<float>( alpha - (beta * cos((M_TAU_DOUBLE*n) / (N-1.0)) ) );
    }
    
    table[len] = table[0];
    return table;
}


float* pdsp::windowBlackman( int len, double alpha) {
    float* table;
    ofx_allocate_aligned(table, len+1);
    double N = static_cast<double>(len);
    
    //alpha is 0.54 in default
    double a0 = (1.0-alpha)*0.5;
    double a1 = 0.5;
    double a2 = alpha*0.5;
    
    for(int i=0; i<len; ++i){
        double n = static_cast<double>(i);
        
        table[i] = static_cast<float>( a0
                                      - a1*cos((M_TAU_DOUBLE*n) / (N-1.0))
                                      + a2*cos((2.0*M_TAU_DOUBLE*n) / (N-1.0))
                                      );
    }
    
    table[len] = table[0];
    return table;
    
}



float* pdsp::windowBlackmanHarris( int len) {
    float* table;
    ofx_allocate_aligned(table, len+1);
    double N = static_cast<double>(len);
    
    for(int i=0; i<len; ++i){
        double n = static_cast<double>(i);
        
        table[i] = static_cast<float>(  0.35875
                                      - 0.48829*cos((M_TAU_DOUBLE*n) / (N-1.0))
                                      + 0.14128*cos((2.0*M_TAU_DOUBLE*n) / (N-1.0))
                                      - 0.01168*cos((3.0*M_TAU_DOUBLE*n) / (N-1.0))
                                      );
    }
    table[len] = table[0];
    return table;
    
}


float* pdsp::windowSine( int len){
    float* table;
    ofx_allocate_aligned(table, len+1);
    double N = static_cast<double>(len);
    
    for(int i=0; i<len; ++i){
        double n = static_cast<double>(i);
        
        table[i] = static_cast<float>( sin((M_PI_DOUBLE*n) / (N-1.0)) );
    }
    table[len] = table[0];
    return table;

}

float* pdsp::windowWelch(int len){
    float* table;
    ofx_allocate_aligned(table, len+1);
    double N = static_cast<double>(len);
    
    for(int i=0; i<len; ++i){
        double n = static_cast<double>(i);
        
        double a = (N-1.0)*0.5;
        double b = (n-a)/a;
        table[i] = static_cast<float>(1.0-(b*b));
    }
    table[len] = table[0];
    return table;
}

float* pdsp::window(Window_t type, int len){
    switch (type) {
            
        case Triangular:
            return windowTriangular(len); break;
            
        case Hann:
            return windowHann(len); break;
   
        case Hamming:
            return windowHamming(len); break;
            
        case Blackman:
            return windowBlackman(len); break;
            
        case BlackmanHarris:
            return windowBlackmanHarris(len); break;
            
        case SineWindow:
            return windowSine(len); break;
            
        case Welch:
            return windowWelch(len); break;
            
        default: //Rectangular
            float* table;
            ofx_allocate_aligned(table, len+1);
            for(int i=0; i<len+1; ++i){
                table[i] = 1.0f;
            }
            return table;
            break;
    }
}

//enum Window_t{Rectangular, Triangular, Hann, Hamming, Blackman, BlackmanHarris, Sine, Welch };
/*
 
 float* table = allocate_f(len);
 double N = static_cast<double>(len);
 
 for(int i=0; i<len; ++i){
 double n = static_cast<double>(i);
 
 table[i] = static_cast<float>( );
 }
 
 return table;

ä*/


