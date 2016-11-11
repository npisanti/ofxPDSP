
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

void pdsp::WaveTable::setup( int len, int maxPartials ) {
    
    if(length==-1){
        
        length = len;
        this->maxPartials = maxPartials;
        
        // init partials table
        double divider = 1.0 / (double) length;
        
        partialsTable = new float*[maxPartials];
         
        for( int i=0; i<maxPartials; ++i){
            ofx_allocate_aligned(partialsTable[i], length+1);     // guard point   

            for( int n=0; n<length; ++n){
                double theta = static_cast<double>(n) * (double)(i+1) * M_TAU_DOUBLE * divider ;
                partialsTable[i][n] = sin( theta );
            }
        }
    
    }else{
        std::cout<< "[pdsp] warning! table length already set automatically, setup() failed\n";
        pdsp_trace(); 
    }
}

void pdsp::WaveTable::initLength( int len ) {
    setup( len );
}

int pdsp::WaveTable::size() const{
    return tableSize;
}

void pdsp::WaveTable::setVerbose( bool verbose ){
    loader.setVerbose( verbose );
    this->verbose = verbose;
}


void pdsp::WaveTable::addEmpty( int numberOfWavesToAdd ) {
    
    if( length!= -1 ){
        
        if(buffer == nullptr){
            
            buffer = new float*[numberOfWavesToAdd+1]; // guard point
            for( int i=0; i<numberOfWavesToAdd+1; ++i){
                ofx_allocate_aligned(buffer[i], length+1);                
            }
            tableSize = numberOfWavesToAdd;
            
        }else{
            
            float** newHandles = new float* [tableSize+numberOfWavesToAdd+1];
            
            for(int i=0; i<tableSize+1; ++i){
                newHandles[i] = buffer[i];
            }
            for( int i=0; i<numberOfWavesToAdd; ++i){
                tableSize++;            
                ofx_allocate_aligned( newHandles[tableSize], length+1 );                                   
            }
            
            delete [] buffer;
            buffer = newHandles;
        }
        if(verbose) std::cout<< "[pdsp] wavetable size: "<< tableSize << " waveforms\n";
  
    }else{
    
        std::cout<< "[pdsp] warning! adding table without setting table size first, routine not performed, use setup() first\n";
        pdsp_trace();
    
    }
}    

void pdsp::WaveTable::setEmpty( int index ) {
    
    if(index<0 ) index = 0;
    if(index >= tableSize ) index = tableSize-1;
    
    for(int i=0; i<length; ++i){
        buffer[index][i] = 0.0f;
    }
}    

void pdsp::WaveTable::addSample( std::string path ) {
    addEmpty();
    int index = tableSize-1;
    setSample( index, path );
}

void pdsp::WaveTable::setSample(int index, std::string path ) {
    
    if(index<0 ) index = 0;
    if(index >= tableSize ) index = tableSize-1;
    
    loader.load(path);
    
    if(loader.loaded()){
        if(length == -1){
            length = loader.length;
        }
        
        if( length != loader.length ){
            std::cout<< "[pdsp] warning! loading wavetables of different sizes!\n";
            pdsp_trace();
        } else {

            for(int n=0; n<length; ++n){
                buffer[index][n] = loader.buffer[0][n];
            }
            buffer[index][length] = buffer[index][0];
        }        
    }
    
}



void pdsp::WaveTable::addAdditiveWave( const std::vector<float> & partials, bool harmonicScale ){
   
    addEmpty();

    int index = tableSize-1;
    setAdditiveWave(index, partials, harmonicScale);

}

void pdsp::WaveTable::setAdditiveWave(int index, const std::vector<float> & partials, bool harmonicScale ){
   
    if(index<0 ) index = 0;
    if(index >= tableSize ) index = tableSize-1;   

    int partial_i = 1; 
    
    ofx_Aeq_Zero(buffer[index], length);
    
    double signalMax = 0.0;
    
    for (const float & value : partials){
        
        double harmonic_amp = value;
        double partial = (double) partial_i;

        if(harmonicScale){
            double harmonic = 1.0 / partial;
            if(partial_i%2 == 0) harmonic = -harmonic;
            harmonic_amp *= harmonic;  
        }
        
        ofx_Aeq_Badd_CmulS( buffer[index], buffer[index], partialsTable[partial_i-1], harmonic_amp, length );
        signalMax += std::abs( harmonic_amp );
        
        partial_i++;
        if(partial_i>maxPartials) break;
    }

    float div = 1.0 / signalMax;

    ofx_Aeq_BmulS( buffer[index], buffer[index], div, length );
    
    buffer[index][length] = buffer[index][0];

}



void pdsp::WaveTable::addAdditiveWave( std::initializer_list<float> partials, bool harmonicScale ){
   
    addEmpty();

    int index = tableSize-1;
    setAdditiveWave(index, partials, harmonicScale);

}

void pdsp::WaveTable::setAdditiveWave(int index, std::initializer_list<float> partials, bool harmonicScale ){
   
    if(index<0 ) index = 0;
    if(index >= tableSize ) index = tableSize-1;   

    int partial_i = 1; 
    
    ofx_Aeq_Zero(buffer[index], length);
    
    double signalMax = 0.0;
    
    for (const float & value : partials){
        
        double harmonic_amp = value;
        double partial = (double) partial_i;

        if(harmonicScale){
            double harmonic = 1.0 / partial;

            harmonic_amp *= harmonic;  
        }
        if(partial_i%2 == 0) harmonic_amp = -harmonic_amp;
        
        ofx_Aeq_Badd_CmulS( buffer[index], buffer[index], partialsTable[partial_i-1], harmonic_amp, length );
        signalMax += std::abs( harmonic_amp );
        
        partial_i++;
        if(partial_i>maxPartials) break;
    }

    float div = 1.0 / signalMax;
    //std::cout<<"signal max is "<< std::to_string(signalMax)<<"\n";

    //for( int n=0; n<length; ++n){
    //    buffer[index][n] *= div;
    //}

    ofx_Aeq_BmulS( buffer[index], buffer[index], div, length );
    
    buffer[index][length] = buffer[index][0];

}


void pdsp::WaveTable::addSawWave( int partials ) {
  
    addEmpty();

    int index = tableSize-1;
    setSawWave( index, partials );
    
}


void pdsp::WaveTable::setSawWave(int index, int partials ) {

    if(index<0 ) index = 0;
    if(index >= tableSize ) index = tableSize-1;

    int partial_i = 1; 
    
    ofx_Aeq_Zero(buffer[index], length);
    
    double signalMax = 0.0;
    
    for ( ; (partial_i < (partials+1) && partial_i<=maxPartials ); partial_i+=1 ){
        
        double partial = (double) partial_i;

        double harmonic_amp = 1.0 / partial;
        if(partial_i%2 == 0) harmonic_amp = -harmonic_amp;
        
        ofx_Aeq_Badd_CmulS( buffer[index], buffer[index], partialsTable[partial_i-1], harmonic_amp, length );
        signalMax += std::abs( harmonic_amp );
        
    }

    float div = 1.0 / signalMax;

    ofx_Aeq_BmulS( buffer[index], buffer[index], div, length );
    
    buffer[index][length] = buffer[index][0];
    
}

  
void pdsp::WaveTable::addSquareWave ( int partials ) {
  
    addEmpty();

    int index = tableSize-1;
    setSquareWave( index, partials);
        
}

  
void pdsp::WaveTable::setSquareWave (int index, int partials ) {
    
    if(index<0 ) index = 0;
    if(index >= tableSize ) index = tableSize-1;
    
    int partial_i = 1;     
    
    ofx_Aeq_Zero(buffer[index], length);
    
    double signalMax = 0.0;
    
    for ( ; (partial_i < (partials+1) && partial_i<=maxPartials ); partial_i+=2 ){
        
        double partial = (double) partial_i;

        double harmonic_amp = 1.0 / partial;
        
        ofx_Aeq_Badd_CmulS( buffer[index], buffer[index], partialsTable[partial_i-1], harmonic_amp, length );
        signalMax += std::abs( harmonic_amp );
        
    }

    float div = 1.0 / signalMax;

    ofx_Aeq_BmulS( buffer[index], buffer[index], div, length );
    
    buffer[index][length] = buffer[index][0];    
        
}

      
void pdsp::WaveTable::addTriangleWave ( int partials ) {
  
    addEmpty();

    int index = tableSize-1;
    setTriangleWave( index, partials );
            
}

      
void pdsp::WaveTable::setTriangleWave (int index, int partials ) {

    if(index<0 ) index = 0;
    if(index >= tableSize ) index = tableSize-1;

    int partial_i = 1; 
    
    ofx_Aeq_Zero(buffer[index], length);
    
    double signalMax = 0.0;
    
    for ( ; (partial_i < (partials+1) && partial_i<=maxPartials ); partial_i+=2 ){
        
        double partial = (double) partial_i;

        double harmonic_amp = 1.0 / partial;
        harmonic_amp *= harmonic_amp;
        
        ofx_Aeq_Badd_CmulS( buffer[index], buffer[index], partialsTable[partial_i-1], harmonic_amp, length );
        signalMax += std::abs( harmonic_amp );
    
    }

    float div = 1.0 / signalMax;

    ofx_Aeq_BmulS( buffer[index], buffer[index], div, length );
    
    buffer[index][length] = buffer[index][0];      
    
            
}
      
void pdsp::WaveTable::addSineWave ( ) {
  
    addEmpty();

    int index = tableSize-1;
    setSineWave( index );
            
}
      
void pdsp::WaveTable::setSineWave ( int index ) {

    if(index<0 ) index = 0;
    if(index >= tableSize ) index = tableSize-1;

    ofx_Aeq_B(buffer[index], partialsTable[0], length);
    
    buffer[index][length] = buffer[index][0];  
            
}
