
#include "DataTable.h"


pdsp::DataTable::DataTable() {
    length = -1;
    buffer = nullptr;
    bSetting = false;
    bAdditive = false;
	bLoaded  = false;
	bHarmonic = true;
    lastIndex = 0;
    lastTurn = -42;
    smooth = 0.0f;
}

pdsp::DataTable::~DataTable() {
    if(buffer!=nullptr){
		ofx_deallocate_aligned(buffer);
        buffer = nullptr;
    }    
}

void pdsp::DataTable::harmonicAdditiveMode( bool active ) {
	bHarmonic = active;
}

void pdsp::DataTable::smoothing( float value ) {
	if(value<0.0f) 	value = 0.0f;
	if(value>=1.0f)	value = 0.99999f;
	smooth = value;
}

int pdsp::DataTable::getTableLength() const{
    return length;
}

int pdsp::DataTable::getMaxPartials() const  {
	return maxPartials;
}

void pdsp::DataTable::setup( int len, int maxPartials, bool harmonicAdditive ) {
	length = len;
	this->maxPartials = maxPartials;
	bHarmonic = harmonicAdditive;
	
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

	ofx_allocate_aligned(buffer,    length+1);     // guard point 
	ofx_allocate_aligned(bufferNew, length+1);     // guard point 

	int max = (len > maxPartials) ? len : maxPartials;
	vData.resize( max );
	for (size_t i=0; i<vData.size(); ++i){
		vData[i] = 0.0f;
	}
}

bool pdsp::DataTable::ready() {
	return (!bSetting && !bLoaded ); 
}

void pdsp::DataTable::begin() {
	if( ready() ) {
		bSetting = true;
		bLoaded = false;
		for (size_t i=0; i<vData.size(); ++i){
			vData[i] = 0.0f;
		}
	}
}

void pdsp::DataTable::data( int i, float value ) {
	vData[i] = value;
	lastIndex = (lastIndex>i) ? lastIndex : i;
}

void pdsp::DataTable::end( bool additive ) {
	bAdditive = additive;
	bSetting = false;
	bLoaded  = true;
}
 
void pdsp::DataTable::update( ) {
	if( lastTurn!=OutputNode::getGlobalProcessingTurnId() && bLoaded ) {	
		// uploads the wave to bufferNew
		if( bAdditive ) {
			additive();
		} else {
			for(size_t n=0; n<length; ++n){
				bufferNew[n] = vData[n];
			}
		}
		bufferNew[length] = bufferNew[0]; // guard point
		
		// now interpolate the wave to the actual buffer with the given smooth
		// buffer = (buffer * smooth) + (bufferNew * (1.0f - smooth))
		// this is basically some kind of low pass filtering operating at buffer rate
		if( smooth!=0.0f ){
			ofx_Aeq_BmulS( buffer, buffer, smooth, length+1 );
			ofx_Aeq_BmulS( bufferNew, bufferNew, (1.0f-smooth), length+1 );
			ofx_Aeq_BaddC( buffer, buffer, bufferNew, length+1 );			
		}else{
			ofx_Aeq_B( buffer, bufferNew, length+1);
		}
		
		// cleans
		lastTurn = OutputNode::getGlobalProcessingTurnId();
		bLoaded = false;
	}
}

void pdsp::DataTable::additive() {
	ofx_Aeq_Zero( bufferNew, length);
    
    double signalMax = 0.0;
    
	for ( size_t i=0; i<lastIndex && i<maxPartials; ++i ){
		
		int partial_i = i+1;
		double harmonic_amp = vData[i];
		double partial = (double) partial_i;

		if(bHarmonic){
			double harmonic = 1.0 / partial;
			harmonic_amp *= harmonic;  
		}
		if(partial_i%2 == 0) harmonic_amp = -harmonic_amp;
		
		ofx_Aeq_Badd_CmulS( bufferNew, bufferNew, partialsTable[partial_i-1], harmonic_amp, length );
		signalMax += std::abs( harmonic_amp );
	}

	float div = 1.0 / signalMax;

	ofx_Aeq_BmulS( bufferNew, bufferNew, div, length );
}













/*
void pdsp::DataTable::addEmpty( int numberOfWavesToAdd ) {
    
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

void pdsp::DataTable::setEmpty( int index ) {
    
    if(index<0 ) index = 0;
    if(index >= tableSize ) index = tableSize-1;
    
    for(int i=0; i<length; ++i){
        buffer[index][i] = 0.0f;
    }
}    
*/
