#include "ofApp.h"

#define ZAPS_NUMBER 4
#define MAX_GENERATIONS 8
#define SIDE 8
#define BARH 60
#define METERH 25


//--------------------------------------------------------------
void ofApp::setup(){
    
    //------------------- graphics --------------
    ofBackground(0);
    ofSetFrameRate(30);
    ofDisableAntiAliasing();
    ofSetWindowShape(640, 360);
    offsetX = (640 - SIDE*64)/2;
    blink = 0;
            
    // ---- set up cellular automaton ----------
    wrules.resize(8);

    rule = 90;  
    initRule();
    
    bars.resize(64);
        
    caGenerationIndex = 0;
    wolframCA.resize(MAX_GENERATIONS); // 8 different generatiosn
    for( vector<int> & v: wolframCA ){
        v.resize(66);  // 64 pixels, we nee 16 steps for each generators + 2 guard points, 4 generator
        for( int i=0; i<66; ++i){
            v[i] = 0;
        }
    }
    
    initFirstFlag = true;

    // Setting up score ------------------
    //engine.score.stop();    // decomment if you want to start the app from stopped state
    engine.score.setTempo(172.0f);
    engine.score.sections.resize(ZAPS_NUMBER);
    engine.score.sections[0].setCell(0, &wolframSequence, pdsp::Behavior::Loop);
    engine.score.sections[0].enableQuantization( 0, 1.0 );
    engine.score.sections[0].launchCell(0);        


    // instead of a lambda we can assign a function of our sequence code this way
    wolframSequence.code = bind(&ofApp::processSequence, this); 
    
    //--------PATCHING-------
    zaps.resize(ZAPS_NUMBER);

    float scaleGain = 2.0f / ZAPS_NUMBER;
    float basePitch = 53.0f; // F3
    
    for ( int i=0; i<ZAPS_NUMBER; ++i ) {
        // we pan the zaps from left to the right
        float panWidth = 0.30f;
        float pan = (float(i) - ((float)(ZAPS_NUMBER-1))*0.5f) * panWidth; 
                    
        engine.score.sections[0].out_trig(i) >> zaps[i]; // patch the sequence outputs to the zaps
                    (basePitch + (i * 7.0f)) >> zaps[i].in("pitch");
        
        zaps[i] * (scaleGain * panL(pan)) >> engine.audio_out(0);
        zaps[i] * (scaleGain * panR(pan)) >> engine.audio_out(1);
    }
    
    //---------------------- audio setup -------------
    engine.listDevices();
    engine.setDeviceID(0); // REMEMBER TO SET THIS AT THE RIGHT INDEX!!!!
    engine.setup( 44100, 512, 3); 

}

//--------------------------------------------------------------
void ofApp::initRule() noexcept { // init wolfram rules from number bits
    int rulebits = rule;
    for(int i=0; i<8; ++i){ // inits the rules
        wrules[i] = rulebits & 0x0001;  // take just the lowest bit
        rulebits = rulebits>>1;             // bitshift by one
    }
    ruleChanged = false;
}

//--------------------------------------------------------------
void ofApp::newGeneration() noexcept{ // update the automaton
    
    int oldGeneration = caGenerationIndex;
    caGenerationIndex--;
    if(caGenerationIndex < 0){
        caGenerationIndex = MAX_GENERATIONS - 1;
    }
    
    for(int x = 1; x<65; ++x){
        int cellRule = wolframCA[oldGeneration][x-1] * 4 
                    + wolframCA[oldGeneration][x] * 2 
                    + wolframCA[oldGeneration][x+1];
                    
        wolframCA[caGenerationIndex][x] = wrules[cellRule]; 
    }     

}

void ofApp::wolframToPatterns() noexcept{ // convert the automaton generations sum to float values
    
    for(int x =0; x<64; ++x){
        int sum = 0;
        int xx = x+1;
        for(int y = 0; y<MAX_GENERATIONS; ++y){
            sum += wolframCA[y][xx];
        }
        
        float intensity = (float)sum / (float)MAX_GENERATIONS; 
        intensity *= triggerScaling;
        if(intensity > 1.0f) intensity = 1.0f;
        bars[x] = intensity;
    }

}
//--------------------------------------------------------------
void ofApp::initFirst() noexcept{ // init the automaton with just an active cell in the top left

    for( vector<int> & v: wolframCA ){
        for( int i=0; i<66; ++i){
            v[i] = 0;
        }
    }        

    wolframCA[caGenerationIndex][1] = 1;
            
    if(rule==90){
        triggerScaling = 3.0f;
    }else if(rule==150){
        triggerScaling = 1.5f;
    }else{
        triggerScaling = 1.0f;
    }

}

//--------------------------------------------------------------
void ofApp::initRandom() noexcept{ // randomly inits the automaton

    for( vector<int> & v: wolframCA ){
        for( int i=0; i<66; ++i){
            v[i] = 0;
        }
    }        
    
    for(int x=1; x<65; ++x){
        // pdspChange(float value) controls the chance of having an alive cell
        wolframCA[caGenerationIndex][x] = pdspChance(0.15f) ? 1 : 0; 
    }
    
    // always also make the first active
    wolframCA[caGenerationIndex][1] = 1;    
    
    triggerScaling = 1.0f;

}

//--------------------------------------------------------------
void ofApp::processSequence() noexcept{
        
    // code for sequencing wolfram CA values
    // this code is running in the audio thread
    // we are also updating the wolfram automaton inside this code
   
    // update rules
    if(ruleChanged){ initRule(); }
    
    // update or init automaton
    if(initFirstFlag){
        initFirst();
        initFirstFlag = false;
    }else if(initRandomFlag){
        initRandom();
        initRandomFlag = false;
    }else{
        newGeneration();        
    }
    // convert the sums of the automaton generations to float values
    wolframToPatterns();
    // we use those float values to set the sequences steps
    // this sequence has 4 outputs, we have patched to the zaps
    wolframSequence.begin(16.0, 1.0);
    for(int out=0; out<ZAPS_NUMBER; ++out){
        for(int x=0; x<16; ++x){    //16 steps
            int bar = x + out*16;
            wolframSequence.message( (double)(x), bars[bar], out);
        }
    }
    wolframSequence.end();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    
    ofTranslate(offsetX,40);
    
    ofSetColor(0,255,255); //cyan
    
    int genIndex = caGenerationIndex;
    
    ofTranslate(-SIDE, 0); //needed as we start from x = 1
    
    // draw the automaton
    for(int i=0; i<MAX_GENERATIONS; ++i){

        for(int x = 1; x<65; ++x){
            if(wolframCA[genIndex][x] == 0){
                ofNoFill();
                ofDrawRectangle(SIDE*x, 0, SIDE, SIDE);    
            }else{
                ofFill();
                ofDrawRectangle(SIDE*x -1 , -1, SIDE+1, SIDE+1);    
            }
        } 
               
        genIndex++; 
        if(genIndex==MAX_GENERATIONS) genIndex = 0;
        ofTranslate(0, SIDE);
        
    }
    
    // draw the bars
    ofTranslate(SIDE, SIDE);    
    
    for(int x =0; x<64; ++x){
        
        ofNoFill();
        ofDrawRectangle(SIDE*x, 0, SIDE, BARH); 
        ofFill();
        float height = bars[x] * BARH;
        float yy = BARH - height;
        ofDrawRectangle(SIDE*x, yy, SIDE, height); 
   
    }
    
    ofTranslate(0, BARH);  

    // draw the playhead and envelope meters
    float playhead = engine.score.sections[0].meter_playhead();
    
    ofPushMatrix();    
    for(int i=0; i<4; ++i){

        float x = playhead * (float)(SIDE*16);
        
        ofDrawLine(x, 0, x, METERH); 
        
        string label = "zap ";
        label+= to_string(i);
        
        ofDrawBitmapString(label, 5, METERH-5);

        ofFill();
        ofSetColor(0, 150, 150);
        float envw = zaps[i].meter_env() * (float) (SIDE*16);
        ofDrawRectangle( 0, METERH, envw, METERH );        

        ofNoFill();
        ofSetColor(0, 255, 255);
        ofDrawRectangle( 0, 0, SIDE*16, METERH );
        ofDrawRectangle( 0, METERH, SIDE*16, METERH );
        
        ofTranslate(SIDE*16,0);
        
    }
    ofPopMatrix();

    // draw some infos
    ofTranslate(0, METERH*3);

    string info = "";
    info += "1 : init to default.";
    if(initFirstFlag && blink < 4){ 
        info+= " initializing..."; 
    }
    info += "\n";
    info += "2 : init to random.";
    if(initRandomFlag && blink < 4){ 
        info+= " initializing..."; 
    }
    info += "\n";
    info += "3 : change rule. selected: rule ";
    info += to_string(rule);
    if(ruleChanged && blink < 4){ 
        info+= " changing..."; 
    }
    info += "\n";
    info += "4/5 : decrement/increment dynamics scaling. scaling = ";
    info += to_string(triggerScaling);
    info += "\n";
    info += "spacebar: pause/play\n";    
    info += "s : stop\n";    

    ofDrawBitmapString(info, 5, 5); 

    blink++;
    if(blink==8){ blink = 0; }

}

//--------------------------------------------------------------
void ofApp::switchRule(){ // change the automata rules
    switch(rule){
        case 60:
            rule = 90;
            break;
        case 90:
            rule = 124;
            break;
        case 124:
            rule = 126;
            break;
        case 126:
            rule = 150;
            break;
        case 150:
            rule = 60;
            break;
    }
    ruleChanged = true;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch(key){
        case '1':
            initFirstFlag = true;
            initRandomFlag = false;
            break;
        case '2':
            initRandomFlag = true;
            initFirstFlag = false;
            break;
        case '3':
            switchRule();
        case '4':
            triggerScaling-=0.5f;
            break;
        case '5':
            triggerScaling+=0.5f;
            break;
        case ' ': // pause / play
            if(engine.score.isPlaying()){
                engine.score.pause();
            }else{
                engine.score.play();
            }
            break;
        case 's': // stop
            engine.score.stop();
            break;
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
