
#include "WaveTable.h"


pdsp::WaveTable::WaveTable() {
    length = -1;
    tableSize = 0;
    buffer = nullptr;
    verbose = false;
}


pdsp::WaveTable::~WaveTable(){
 
    if(buffer!=nullptr){
        
        for(int i=0; i<tableSize+1; ++i){
            ofx_deallocate_aligned(buffer[i]);
        }
        delete[] buffer;
 
    }
    
}

float* pdsp::WaveTable::table( int index ){
    if(index<0) index = 0;
    if(index>=tableSize) index = tableSize-1;
    return buffer[index];
}

int pdsp::WaveTable::tableLength() const{
    return length;
}

void pdsp::WaveTable::initLength( int len ) {
    if(length==-1){
        length = len;
    }else{
        std::cout<< "[pdsp] warning! table length already set automatically, initLength() failed\n";
        pdsp_trace(); 
    }
}

int pdsp::WaveTable::size() const{
    return tableSize;
}

void pdsp::WaveTable::setVerbose( bool verbose ){
    loader.setVerbose( verbose );
    this->verbose = verbose;
}


void pdsp::WaveTable::addEmpty( ) {
    if( length!= -1 ){
        if(buffer == nullptr){
            
            buffer = new float*[2]; // guard point
            for( int i=0; i<2; ++i){
                ofx_allocate_aligned(buffer[i], length+1);                
            }
            tableSize = 1;
            
        }else{
            
            float** newHandles = new float* [tableSize+2];
            
            for(int i=0; i<tableSize+1; ++i){
                newHandles[i] = buffer[i];
            }
            
            tableSize++;            
            ofx_allocate_aligned( newHandles[tableSize], length+1 );   
            delete [] buffer;
            buffer = newHandles;

        }
        if(verbose) std::cout<< "[pdsp] wavetable size: "<< tableSize << " waveforms\n";
    }else{
    
        std::cout<< "[pdsp] warning! adding table without setting table size first, routine not performed, use initTable() first\n";
        pdsp_trace();
    
    }
}    

void pdsp::WaveTable::addSample( std::string path ) {
    
    loader.load(path);
    
    if(loader.loaded()){
        if(length == -1){
            length = loader.length;
        }
        
        if( length != loader.length ){
            std::cout<< "[pdsp] warning! loading wavetables of different sizes!\n";
            pdsp_trace();
        } else {
            addEmpty();
            int index = tableSize-1;
            for(int n=0; n<length; ++n){
                buffer[index][n] = loader.buffer[0][n];
            }
            buffer[index][length] = buffer[index][0];
        }        
    }
    
}


void pdsp::WaveTable::addAdditiveWave( std::initializer_list<double> partials, bool harmonicScale ){
   
    addEmpty();

    int index = tableSize-1;
    
    int partial_i = 1; 
    double multiply = 1.0 / (double) length;
    
    for (const double & value : partials){
        
        double harmonic_amp = value;
        double partial = (double) partial_i;

        if(harmonicScale){
            double harmonic = 1.0 / partial;
            if(partial_i%2 == 0) harmonic = -harmonic;
            harmonic_amp *= harmonic;  
        }
        
        for(int n=0; n<length; ++n){
            double theta = static_cast<double>(n) * multiply * M_TAU_DOUBLE * partial;
            buffer[index][n] += sin ( theta ) * harmonic_amp;
        }        
        
        partial_i++;
    }

    float normalize = -1.0f;
    for(int n=0; n<length; ++n){
        float value = std::abs ( buffer[index][n] );
        if(value > normalize) normalize = value;
    }  
    
    normalize = 1.0f / normalize;
   
    for(int n=0; n<length; ++n){
        buffer[index][n] *= normalize;
    }  
    
    buffer[index][length] = buffer[index][0];


}


void pdsp::WaveTable::addAdditiveWave( const std::vector<double> & partials, bool harmonicScale ){
   
    addEmpty();

    int index = tableSize-1;
    
    int partial_i = 1; 
    double multiply = 1.0 / (double) length;
    
    for (const double & value : partials){
        
        double harmonic_amp = value;
        double partial = (double) partial_i;

        if(harmonicScale){
            double harmonic = 1.0 / partial;
            if(partial_i%2 == 0) harmonic = -harmonic;
            harmonic_amp *= harmonic;  
        }
        
        for(int n=0; n<length; ++n){
            double theta = static_cast<double>(n) * multiply * M_TAU_DOUBLE * partial;
            buffer[index][n] += sin ( theta ) * harmonic_amp;
        }        
        
        partial_i++;
    }

    float normalize = -1.0f;
    for(int n=0; n<length; ++n){
        float value = std::abs ( buffer[index][n] );
        if(value > normalize) normalize = value;
    }  
    
    normalize = 1.0f / normalize;
   
    for(int n=0; n<length; ++n){
        buffer[index][n] *= normalize;
    }  
    
    buffer[index][length] = buffer[index][0];

}


void pdsp::WaveTable::addSawWave( int partials ) {
  
    addEmpty();

    int index = tableSize-1;
    
    int partial_i = 1; 
    double multiply = 1.0 / (double) length;
    
    for ( ; partial_i < (partials+1); partial_i++ ){
        
        double partial = (double) partial_i;

        double harmonic = 1.0 / partial;
        if(partial_i%2 == 0) harmonic = -harmonic;
        
        for(int n=0; n<length; ++n){
            double theta = static_cast<double>(n) * multiply * M_TAU_DOUBLE * partial;
            buffer[index][n] += sin ( theta ) * harmonic;
        }        
        
    }

    float normalize = -1.0f;
    for(int n=0; n<length; ++n){
        float value = std::abs ( buffer[index][n] );
        if(value > normalize) normalize = value;
    }  
    
    normalize = 1.0f / normalize;
   
    for(int n=0; n<length; ++n){
        buffer[index][n] *= normalize;
    }  
    
    buffer[index][length] = buffer[index][0];  
    
}

  
void pdsp::WaveTable::addSquareWave ( int partials ) {
  
    addEmpty();

    int index = tableSize-1;
    
    int partial_i = 1; 
    double multiply = 1.0 / (double) length;
    
    for ( ; partial_i < (partials+1); partial_i+=2 ){
        
        double partial = (double) partial_i;

        double harmonic = 1.0 / partial;
        if(partial_i%2 == 0) harmonic = -harmonic;
        
        for(int n=0; n<length; ++n){
            double theta = static_cast<double>(n) * multiply * M_TAU_DOUBLE * partial;
            buffer[index][n] += sin ( theta ) * harmonic;
        }        
        
    }

    float normalize = -1.0f;
    for(int n=0; n<length; ++n){
        float value = std::abs ( buffer[index][n] );
        if(value > normalize) normalize = value;
    }  
    
    normalize = 1.0f / normalize;
   
    for(int n=0; n<length; ++n){
        buffer[index][n] *= normalize;
    }  
    
    buffer[index][length] = buffer[index][0];  
        
}

      
void pdsp::WaveTable::addTriangleWave ( int partials ) {
  
    addEmpty();

    int index = tableSize-1;
    
    int partial_i = 1; 
    double multiply = 1.0 / (double) length;
    
    for ( ; partial_i < (partials+1); partial_i+=2 ){
        
        double partial = (double) partial_i;

        double harmonic = 1.0 / partial;
        harmonic *= harmonic;
        
        if(partial_i%2 == 0) harmonic = -harmonic;
        
        for(int n=0; n<length; ++n) {
            double theta = static_cast<double>(n) * multiply * M_TAU_DOUBLE * partial;
            buffer[index][n] += sin ( theta ) * harmonic;
        }
        
    }

    float normalize = -1.0f;
    for(int n=0; n<length; ++n){
        float value = std::abs ( buffer[index][n] );
        if(value > normalize) normalize = value;
    }  
    
    normalize = 1.0f / normalize;
   
    for(int n=0; n<length; ++n){
        buffer[index][n] *= normalize;
    }  
    
    buffer[index][length] = buffer[index][0];  
            
}
      
void pdsp::WaveTable::addSineWave ( ) {
  
    addEmpty();

    int index = tableSize-1;
    double multiply = 1.0 / (double) length;    

    for(int n=0; n<length; ++n) {
        double theta = static_cast<double>(n) * multiply * M_TAU_DOUBLE;
        buffer[index][n] += sin ( theta );
    }
    
    buffer[index][length] = buffer[index][0];  
            
}
