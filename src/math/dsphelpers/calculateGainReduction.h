
// calculateGainReduction.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016

#ifndef PDSP_MATH_CALCULATEGAINREDUCTION_H_INCLUDED
#define PDSP_MATH_CALCULATEGAINREDUCTION_H_INCLUDED


namespace pdsp{
        
        inline void vect_calculateGainReductionDBWithKnee(float* output, const float* input, const float &threshold, const float &CS, const float &kneeWidth, const int &bufferSize){
                
                int n=0;
                
                float kneeLow = threshold - kneeWidth/2;
                float kneeHigh  = threshold + kneeWidth/2;
                float kneeMult = CS / (kneeLow - kneeHigh); //to multiply for input - kneeHigh
                
                //lagrange for 2 points simplified
                //        y = (0 / (kneeHigh - kneeLow)) * (x - kneeLow)
                //          + (CS / (kneeLow - kneeHigh)) * (x - kneeHigh); 
                     
                
                for(; n<bufferSize; ++n){
                        
                        float actualCS = CS;
                        
                        if(input[n] > kneeLow && input[n] < kneeHigh){
                                actualCS = kneeMult * (input[n] - kneeHigh);
                        } 
                        
                        float yG = actualCS * (threshold - input[n] );
                        
                        if(yG > 0.0f){ yG = 0.0f; } //min(yG, 0.0f)
                                
                        output[n] = yG;
                }
        }
        
        
        
        inline void vect_calculateGainReductionDB(float* output, const float* input, const float &threshold, const float &CS, const int &bufferSize){
                
                int n=0;
                                
                for(; n<bufferSize; ++n){
                        
                        float yG = CS * (threshold - input[n] );
                        
                        if(yG > 0.0f){ yG = 0.0f; } //min(yG, 0.0f)
                                
                        output[n] = yG;
                }
        }
}



#endif  // PDSP_MATH_CALCULATEGAINREDUCTION_H_INCLUDED
