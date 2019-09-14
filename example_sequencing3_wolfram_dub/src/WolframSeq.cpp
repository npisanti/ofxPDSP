
// WolframSeq.cpp
// Nicola Pisanti, MIT License, 2016

#include "WolframSeq.h"

int WolframSeq::number = 0;

void WolframSeq::setup( int maxSteps, int maxOutputs, int rule, int generations, int caSide, int totalHeight, bool sendGateOff ) {
    
    this->caSide = caSide;
    int caw = caSide * maxOutputs * maxSteps;
    this->cah = caSide * generations;
    this->maxSteps = maxSteps;
    this->maxOutputs = maxOutputs;
    this->bars = ((double) maxSteps) / division;

    barHeight = totalHeight - cah - 40;
    
    ca.setup( caw, cah, maxSteps * maxOutputs, generations, rule );
    
    values.resize(maxOutputs);
    stepbars.resize(maxOutputs*maxSteps);

    string name = "WSEQ n";
    name+= ofToString(number++); 
    parameters.setName( name );
    parameters.add( this->rule.set( "rule", 60, 0, 150 ));
    parameters.add( this->steps.set( "steps", 16, 0, maxSteps ));
    parameters.add( this->division.set( "division", 16, 0, 32 ));
    parameters.add( this->activeOuts.set( "active outs", maxOutputs, 0, maxOutputs ));
    parameters.add( this->threshold.set( "threshold", 4, 0, 8 ));
    parameters.add( this->seedsDensity.set( "seeds density", 0.33f, 0.0f, 1.0f ));
    parameters.add( this->reverse.set( "reverse stepbars", false) );
    parameters.add( this->limiting.set( "limiting", (float)maxOutputs, 0.0f, (float)maxOutputs ));
    parameters.add( this->dbRange.set( "dB range", 18, 48, 6) );
    
    if(sendGateOff){
        gateOff = true;
        parameters.add( this->gateLen.set( "gate len", 0.5f, 0.001f, 0.999f) );
    }else{
        gateOff = false;
    }
    
    parameters.add( this->remake.set( "remake", false) );


    this->storedRule = rule;
    actionCode = 3; // init

    this->thresholdStored = -1;
    activeOutsStored = maxOutputs;

    dbRange = 18.0f;    
    gate = 0.5f;
    gateOff = false;
    
    stepbarsFbo.allocate(caw+2, barHeight + 20 + 2);
    stepbarsFbo.begin();
        ofClear(0, 0, 0, 0);
    stepbarsFbo.end();
    
}

WolframSeq::WolframSeq(){
    
    // it is useful to have this inside the constructor
    // otherwise it could happen that the lambda take references not to the internal members
    // but to to arguments of the setup function, giving an UNDEFINED behavior
    
    code = [&] () noexcept {
        
        int nextRule = rule;
        if(nextRule != storedRule){
            ca.setRule( nextRule );
            storedRule = nextRule;
        }
        
        int newCode = actionCode;
        
        if(remake){
            newCode = 3;
            remake = false;
        }
        
        switch( newCode ){
            
            case 0: 
                ca.advance();
                break;

            case 1:
                ca.clear();
                actionCode = 0;
                break;
                
            case 2:
                ca.initRandom(seedsDensity);
                actionCode = 0;
                break;
                
            case 3:
                ca.initRandom(seedsDensity);
                for(int i=0; i<ca.generations(); ++i) ca.advance();
                actionCode = 0;
                break;
                
            default: break;
        }
        
        int nextThreshold = threshold;
        if(nextThreshold != thresholdStored){
            thresholdStored = nextThreshold;
            float gen = ca.generations();
            scaling = gen / (float) (gen-thresholdStored);
        }
    
        if(activeOutsStored != activeOuts){
            activeOutsStored = activeOuts;
        } 
        
        gate = gateLen;

        // wolfram to seqs ---------------------------------
        int b;
        if(reverse){
            b = stepbars.size()-1;
        }else{
            b=0;
        }
        for(int x=0; x < ca.cells(); ++x){

            int sum = -thresholdStored;
            for(int y = 0; y<ca.generations(); ++y){
                sum += ca.cell(x, y);
            }
            if(sum<0) sum = 0;            
          
            float intensity = (float)sum / (float)ca.generations(); 
            intensity *= scaling;
            if(intensity > 1.0f) intensity = 1.0f;

            stepbars[b] = intensity;
            
            if(reverse){
                b--;
            }else{
                b++;
            }
        }
        

        // seqs array to messages ---------------------------
        bars = ((double) steps) / division;
                
        this->begin();
       
        float db = dbRange;
        
        for(int x=0; x<steps; ++x){
            
            float sum = 0.0f;
            
            for(int o=0; o < activeOutsStored; ++o) {

                float value = stepbars[ (o*steps) + x ];
                if( value > 0.0f ){
                    values[o] = dB( -db + db * value );
                }else{
                    values[o] = 0.0f;
                }
                sum+= values[o];
           
            }
            
            if( sum > limiting ){
                float multiplier = limiting / sum;
                for(int o=0; o < activeOutsStored; ++o){
                    values[o] *= multiplier;
                }
            }
            
            double d = division;
            
            for(int o=0; o < activeOutsStored; ++o){
                if(values[o]!=0.0f){
                     this->out( o ).delay( x / d ).bang( values[o] );         
                     // gate on
                     if(gateOff){
                         this->out( o ).delay( (x + gate)/d ).bang( 0.0f );
                     }
                }
            }
        }
        this->end();
    };    
    
}



void WolframSeq::draw(int x, int y) {
    
    // update stepbars graphics
    stepbarsFbo.begin();
        ofClear(0, 0, 0, 0);
        ofTranslate(1,1);

        float playhead = meter_percent(); 
        
        int sequenceSteps = steps;
        
        for(int x =0; x<maxOutputs*maxSteps; ++x){
            ofSetColor( 100 );
            ofNoFill();
            ofDrawRectangle(caSide*x, 0, caSide, barHeight); 
            
            if( (x < sequenceSteps*activeOutsStored ) &&
                ((int) (playhead * sequenceSteps)) == x%sequenceSteps ){
                ofSetColor( 255 ); 
            }
            ofFill();
            
            float height = stepbars[x] * barHeight;
            float yy = barHeight - height;
            ofDrawRectangle(caSide*x, yy, caSide, height); 
       
        }
        
        ofSetColor( 255 ); 
        ofTranslate(0, barHeight);  
        
        // draw the playhead and envelope meters

        ofNoFill();
        float playheadW = caSide*sequenceSteps;
        
        for(int i=0; i<maxOutputs; ++i){
            
            if( i < activeOutsStored) {
                ofSetColor( 255 ); 
                
                float x = playhead * playheadW;
                ofDrawLine(x, 0, x, 20);                                 
            }
            
            string label = "out ";
            label+= to_string(i);
            ofDrawBitmapString(label, 5, 15);

            ofSetColor( 100 );
            ofDrawRectangle( 0, 0, playheadW, 20 );
            
            ofTranslate(playheadW,0);
        }
    stepbarsFbo.end();
    
    // draw everything
    ofPushMatrix();
        ofTranslate(x, y);
        
        ca.draw(0, 0);
        
        ofTranslate(0, cah+20);
        
        stepbarsFbo.draw(0, 0);
    
    ofPopMatrix();
}
