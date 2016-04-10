

#include "BasicNodes.h"


pdsp::NullOutput pdsp::Patchable::invalidOutput = NullOutput();
pdsp::NullInput pdsp::Patchable::invalidInput = pdsp::NullInput();


int pdsp::OutputNode::globalProcessingTurnId = 42; // is the answer

//------------------------INPUT NODE--------------------------------

pdsp::InputNode::InputNode( int oversample ) {
    connections = 0;
    defaultValue = 0.0f;
    lastValue = -32000.0f;
    requiredOversampleLevel = oversample;
    baseBufferSize = 16;
    sumBuffer = nullptr;
    buffer = nullptr;

    internalScalarIsConnected = false;
    state = Changed;
    inputs.clear();
    inputs.reserve( PDSP_NODE_POINTERS_RESERVE );

    clampToBoundaries = false;
    lowBoundary = 0.0f;
    highBoundary = 1.0f;

    if( dynamicConstruction ) {
        prepareToPlay( globalBufferSize, globalSampleRate );
    }

}

pdsp::InputNode::InputNode() : InputNode( Preparable::initOversampleLevel ) {}

pdsp::InputNode::InputNode( const InputNode& other ) : InputNode() { //forbid copy constructor
    defaultValue = other.defaultValue;
    lastValue = other.lastValue;
    requiredOversampleLevel = other.requiredOversampleLevel;
    clampToBoundaries = other.clampToBoundaries;
    lowBoundary = other.lowBoundary;
    highBoundary = other.highBoundary;

    if( other.internalScalarIsConnected ) {
        internalScalarIsConnected = true;
        internalScalar.buffer[0] = other.internalScalar.buffer[0];
    }

    for( OutputData odata : other.inputs ) {
        inputs.push_back( odata );
    }
}


pdsp::InputNode& pdsp::InputNode::operator= ( const InputNode& other ) {
    this->disconnectAll();

    defaultValue = other.defaultValue;
    lastValue = other.lastValue;
    requiredOversampleLevel = other.requiredOversampleLevel;
    clampToBoundaries = other.clampToBoundaries;
    lowBoundary = other.lowBoundary;
    highBoundary = other.highBoundary;

    if( other.internalScalarIsConnected ) {
        internalScalarIsConnected = true;
        internalScalar.buffer[0] = other.internalScalar.buffer[0];
    } else {
        internalScalarIsConnected = false;
    }

    for( OutputData odata : other.inputs ) {
        inputs.push_back( odata );
    }

    return *this;
}


pdsp::InputNode::~InputNode() {
    disconnectAll();
    buffer = nullptr;
    state = Changed;

    if( sumBuffer!=nullptr ) {
        ofx_deallocate_aligned( sumBuffer );
    }
}

void pdsp::InputNode::prepareToPlay( int expectedBufferSize, double sampleRate ) {
    baseBufferSize = expectedBufferSize;

    if( sumBuffer!=nullptr ) {
        ofx_deallocate_aligned( sumBuffer );
    }

    ofx_allocate_aligned( sumBuffer, ( expectedBufferSize * requiredOversampleLevel * PDSP_BUFFERS_EXTRA_DIM)/(PDSP_BUFFERS_EXTRA_DIM-1));
    buffer = sumBuffer;
}

void pdsp::InputNode::releaseResources() {

    if( sumBuffer!=nullptr ) {
        ofx_deallocate_aligned( sumBuffer );
    }
}

void pdsp::InputNode::setRequiredOversampleLevel( int newOversample ) {
    disconnectAll();
    requiredOversampleLevel = newOversample;

    if( sumBuffer!=nullptr ) {
        ofx_deallocate_aligned( sumBuffer );
        ofx_allocate_aligned( sumBuffer, ( baseBufferSize * 3 * requiredOversampleLevel )/2 );
    }
}


void pdsp::InputNode::process() noexcept {

    int bufferSize = Preparable::turnBufferSize;

    //process input buffers first
    for( OutputData &odata : inputs ) {
        if( odata.node->lastProcessedTurnId != OutputNode::getGlobalProcessingTurnId() && odata.node->parent != nullptr ) {
            for( NamedOutput &outnode : odata.node->parent->outputs ) {
                outnode.output->updateTurnId();
            }
            //process with the right buffer lenght according to oversample
            odata.node->parent->process( bufferSize * odata.node->parent->getOversampleLevel() );
        }
    }

    switch( connections ) {
        bool multiplyNow;

    case 0: //NO INPUTS---------------------------------------

        buffer = &defaultValue;

        if( lastValue == buffer[0] ) {
            state = Unchanged;
        } else {
            state = Changed;
        }
        lastValue = buffer[0];
        break;

    case 1: { //JUST ONE INPUT----------------------------------
        //starting extra scope
        state = inputs.front().node->state;
        multiplyNow = inputs.front().multiply;

        int switcher = multiplyNow ? 1 : 0 ;
        if( clampToBoundaries ) {
            switcher += 2;
        };

        switch( switcher ) {
        case 0: // all false
            //this should be the most likely
            switch(state){
            case Unchanged:
            case Changed:
                buffer = sumBuffer;
                sumBuffer[0] = inputs.front().node->getCRValue();   //this is necessary for a thread safe ValueNode class
                break;
            case AudioRate:
                buffer = inputs.front().node->buffer;
                break;
            default: break;
            }
            
            break;
        case 1: // multiplyNow is true
            buffer = sumBuffer;
            
            switch(state){
            case Unchanged:
            case Changed:
                sumBuffer[0] = inputs.front().node->getCRValue() * inputs.front().multiplier;
                break;
            case AudioRate:
                ofx_Aeq_BmulS( sumBuffer, inputs.front().node->buffer, inputs.front().multiplier, bufferSize * requiredOversampleLevel );
                break;
            default: break;
            }
            
            break;
        case 2: // clampToBoundaries is true;
            buffer = sumBuffer;

            switch(state){
            case Unchanged:
            case Changed:
                sumBuffer[0] = inputs.front().node->getCRValue();
                if( sumBuffer[0]<lowBoundary ) {
                    sumBuffer[0] = lowBoundary;
                } else if( sumBuffer[0] > highBoundary ) {
                    sumBuffer[0] = highBoundary;
                }
                break;
            case AudioRate:
                ofx_Aeq_clipB( sumBuffer, inputs.front().node->buffer, lowBoundary, highBoundary, bufferSize * requiredOversampleLevel );
                break;
            default: break;
            }
            
            break;
        case 3: // all true, multiply and then clamp
            buffer = sumBuffer;

            switch(state){
            case Unchanged:
            case Changed:
                sumBuffer[0] = inputs.front().node->getCRValue() * inputs.front().multiplier;
                if( sumBuffer[0]<lowBoundary ) {
                    sumBuffer[0] = lowBoundary;
                } else if( sumBuffer[0] > highBoundary ) {
                    sumBuffer[0] = highBoundary;
                }
                break;
            case AudioRate:
                ofx_Aeq_BmulS( sumBuffer, inputs.front().node->buffer, inputs.front().multiplier, bufferSize * requiredOversampleLevel );
                ofx_Aeq_clipB( sumBuffer, sumBuffer, lowBoundary, highBoundary, bufferSize * requiredOversampleLevel );
                break;
            default: break;
            }
            
            break;
            
        default: break;
        }

        }//end of extra scope

        //check if value has changed and update lastValue
        switch(state){
        case Unchanged:
        case Changed:
            if( buffer[0] == lastValue ) {
                state = Unchanged;
            } else {
                state = Changed;
            }
            lastValue = buffer[0];
            break;
        case AudioRate:
            lastValue = buffer[bufferSize*requiredOversampleLevel -1 ];
            break;
        default: break;
        }
        
        break; // SINGLE INPUT BREAK

    default: // MORE THAN ONE INPUT-----------------------------
        buffer = sumBuffer;
        state = Changed;
        float scalarSum = 0.0f;



        //for( const OutputData &odata : inputs ) {
        for(int i=0; i<connections; ++i ) {
            OutputData& odata = inputs[i];
            OutputNode* nodei = odata.node;
            //combined switch
            int tripleSwitch = nodei->state + state*4; 
            if(odata.multiply) tripleSwitch += 16;
            
            
            switch(tripleSwitch){
            case 4: case 5: // odata CR, state Changed
                scalarSum += nodei->getCRValue();
                break;
            case 6: // odata AR, state Changed
                state = AudioRate;
                ofx_Aeq_B(sumBuffer, nodei->buffer, bufferSize * requiredOversampleLevel);
                break;
            case 8: case 9: // odata CR, state AudioRate
                scalarSum += nodei->getCRValue();
                break;
            case 10: // odata AR, state AudioRate
                ofx_Aeq_BaddC( sumBuffer, sumBuffer, nodei->buffer, bufferSize * requiredOversampleLevel);
                break;
            case 20: case 21: // odata CR, state Changed, multiply
                scalarSum += nodei->getCRValue() * odata.multiplier;
                break;
            case 22: // odata AR, state Changed, multiply
                state = AudioRate;
                ofx_Aeq_BmulS(sumBuffer, nodei->buffer, odata.multiplier, bufferSize * requiredOversampleLevel);
                break;
            case 24: case 25: // odata CR, state AudioRate, multiply
                scalarSum += nodei->getCRValue() * odata.multiplier;
                break;
            case 26: // odata AR, state AudioRate , multiply           
                ofx_Aeq_Badd_CmulS(sumBuffer, sumBuffer, nodei->buffer, odata.multiplier, bufferSize * requiredOversampleLevel);
                break;
            default: break;
            }
        }
            // original branched as reference
            /*
            if( nodei->state == AudioRate ) { //we sum to the buffer
                if( state==Changed ) { //we init the buffer and change state
                    state = AudioRate;
                    if( odata.multiply ) {
                        ofx_AeqBmulS(sumBuffer, nodei->buffer, odata.multiplier, bufferSize * requiredOversampleLevel);
                    } else {
                        ofx_AeqB(sumBuffer, nodei->buffer, bufferSize * requiredOversampleLevel);
                    }
                } else { //we just add to the buffer
                    if( odata.multiply ) {
                        ofx_AeqBadd_CmulS(sumBuffer, sumBuffer, nodei->buffer, odata.multiplier, bufferSize * requiredOversampleLevel);
                    } else {
                        ofx_AeqBaddC( sumBuffer, sumBuffer, nodei->buffer, bufferSize * requiredOversampleLevel);
                    }
                }
            } else {//we sum to the scalars
                if( odata.multiply ) {
                    scalarSum += nodei->getCRValue() * odata.multiplier;
                } else {
                    scalarSum += nodei->getCRValue();
                }
            }
            */ 


        //now we sum / assign the scalar
        switch(state){
        case Unchanged:
        case Changed:
            sumBuffer[0] = scalarSum;
            if( clampToBoundaries ) {
                if( sumBuffer[0]<lowBoundary ) {
                    sumBuffer[0] = lowBoundary;
                } else if( sumBuffer[0] > highBoundary ) {
                    sumBuffer[0] = highBoundary;
                }
            }

            if( buffer[0] == lastValue ) {
                state = Unchanged;
            } else {
                state = Changed;
            }

            lastValue = buffer[0];
            break;
        
        case AudioRate:
            if( scalarSum!=0.0f ) {
                ofx_Aeq_BaddS( sumBuffer, sumBuffer, scalarSum, bufferSize * requiredOversampleLevel );
            }
            if( clampToBoundaries ) {
                ofx_Aeq_clipB( sumBuffer, sumBuffer, lowBoundary, highBoundary, bufferSize * requiredOversampleLevel );
            }
            lastValue = sumBuffer[bufferSize*requiredOversampleLevel -1 ];
            break;
      
        default: break;
        }
    
        break; //MULTIPLE INPUTS BREAK

    } // END SWITCH----------------------------------------------------------
}


void pdsp::InputNode::connect( OutputNode& output ) {

    if( connections==0 ) {
        inputs.push_back( OutputData( &output, output.multiply, output.nextMultiplier ) );
        output.multiply = false;
        connections++;
        output.addInputToList( this );

    } else if( connections==1 ) {
        if( &output == inputs.front().node ) {	//just updating the already added input
            if( output.multiply ) {
                inputs.front().multiply = output.multiply;
                inputs.front().multiplier = output.nextMultiplier;
                output.multiply = false;
            }
        } else { //new input
            inputs.push_back( OutputData( &output, output.multiply, output.nextMultiplier ) );
            connections++;
            output.addInputToList( this );
            output.multiply = false;
        }
    } else {
        std::vector<OutputData>::iterator it = inputs.begin();
        bool notFound = true;
        while( it != inputs.end() ) {
            if( ( *it ).node == &output ) {
                //just update
                if( output.multiply ) {
                    ( *it ).multiply = output.multiply;
                    ( *it ).multiplier = output.nextMultiplier;
                    output.multiply = false;
                }
                notFound = false;
                break;
            }
            it++;
        }
        if( notFound ) {
            //new input
            inputs.push_back( OutputData( &output, output.multiply, output.nextMultiplier ) );
            connections++;
            output.addInputToList( this );
            output.multiply = false;
        }
    }

}


void pdsp::InputNode::disconnect( OutputNode& output ) {
    //STILL TO TEST WELL
    std::vector<OutputData>::iterator it = inputs.begin();
    while( it != inputs.end() ) {
        if( ( *it ).node == &output ) {
            output.removeOutputFromList( this );
            inputs.erase( it );
            connections--;
            if( connections==1 ) {
                buffer = inputs.front().node->buffer;
            } else if( connections==0 ) {
                buffer = nullptr;
                state = Changed;
            }
            break;
        }
        it++;
    }
}


void pdsp::InputNode::disconnectAll() {

    for( const OutputData &odata : inputs ) {
        odata.node->removeOutputFromList( this );
    }
    inputs.clear();
    buffer = nullptr;
    connections = 0;
    state = Changed;
    internalScalarIsConnected = false;

}


void pdsp::InputNode::removeInputUnilateral( const OutputNode& outputNode ) {
    //this function is needed only on OutputNode deconstruction and disconnection
    std::vector<OutputData>::iterator it = inputs.begin();
    while( it != inputs.end() ) {
        if( ( *it ).node == &outputNode ) {
            inputs.erase( it );
            connections--;
            if( connections==1 ) {
                buffer = inputs.front().node->buffer;
            } else if( connections==0 ) {
                buffer = nullptr;
            }
            break;
        }
        it++;
    }
}

void pdsp::InputNode::connectFloat( float value ) {
    internalScalar.set( value );
    this->connect( internalScalar );
}

void pdsp::InputNode::disconnectFloat() {
    internalScalarIsConnected = false;
    this->disconnect( internalScalar );
}

void pdsp::InputNode::enableBoundaries( float lo, float hi ) {
    highBoundary = hi;
    lowBoundary = lo;
    clampToBoundaries = true;
}

void pdsp::InputNode::disableBoundaries() {
    clampToBoundaries = false;
}

const float* pdsp::InputNode::getBuffer() const {
    return buffer;
}

const int pdsp::InputNode::getState() const {
    return state;
}


void pdsp::InputNode::setDefaultValue( float newDefault ) {
    defaultValue = newDefault;
}

bool pdsp::InputNode::isConnected() {
    return ( connections!=0 );
}

int pdsp::InputNode::getRequiredOversampleLevel() const {
    return requiredOversampleLevel;
}

bool pdsp::InputNode::exists() {
    return true;
}

//------------------------OUTPUT NODE--------------------------------

pdsp::OutputNode::OutputNode( int oversample ) {
    connections = 0;
    state = Changed;
    oversampleLevel = oversample;
    multiply = false;
    nextMultiplier = 1.0f;
    baseBufferSize = 16;
    outputs.clear();
    outputs.reserve( PDSP_NODE_POINTERS_RESERVE );
    buffer = nullptr;
    parent = nullptr;

    lastProcessedTurnId = 0;

    if( dynamicConstruction ) {
        prepareToPlay( globalBufferSize, globalSampleRate );
    }
}

pdsp::OutputNode::OutputNode() : OutputNode( Preparable::initOversampleLevel ) { }

pdsp::OutputNode::OutputNode( const OutputNode& other ) : OutputNode() { //rule of three copy constructor
    parent = other.parent;
    lastProcessedTurnId = other.lastProcessedTurnId;
    setOversampleLevel( other.getOversampleLevel() );

    for( InputNode* node : other.outputs ) {
        ( *node ).connect( *this );
    }

}

pdsp::OutputNode& pdsp::OutputNode::operator= ( const OutputNode& other ) { //rule of three copy assignment
    parent = other.parent;
    lastProcessedTurnId = other.lastProcessedTurnId;
    outputs.clear();
    setOversampleLevel( other.getOversampleLevel() );

    for( InputNode* node : other.outputs ) {
        ( *node ).connect( *this );
    }
    return *this;
}


pdsp::OutputNode::~OutputNode() {
    disconnectAll();
    if( buffer!=nullptr ) {
        ofx_deallocate_aligned( buffer );
    }
}

void pdsp::OutputNode::prepareToPlay( int expectedBufferSize, double sampleRate ) {

    baseBufferSize = expectedBufferSize;

    if( buffer!=nullptr ) {
        ofx_deallocate_aligned( buffer );
    }
    ofx_allocate_aligned( buffer, ( expectedBufferSize * oversampleLevel * PDSP_BUFFERS_EXTRA_DIM)/(PDSP_BUFFERS_EXTRA_DIM-1));

}

void pdsp::OutputNode::releaseResources() {
    if( buffer!=nullptr ) {
        ofx_deallocate_aligned( buffer );
    }
}

void pdsp::OutputNode::setOversampleLevel( int newOversample ) {
    disconnectAll();

    oversampleLevel = newOversample;

    if( buffer!=nullptr ) {
        ofx_deallocate_aligned( buffer );
        ofx_allocate_aligned( buffer, ( baseBufferSize * 3 * oversampleLevel )/2 );
    }
}

void pdsp::OutputNode::disconnectAll() {

    for( InputNode* &node : outputs ) {
        node->removeInputUnilateral( *this );
    }
    connections = 0;
    state = Changed;
    outputs.clear();
}


void pdsp::OutputNode::connect( InputNode& inputNode ) {
    inputNode.connect( *this );
}

void pdsp::OutputNode::disconnect( InputNode& inputNode ) {
    inputNode.disconnect( *this );
}

void pdsp::OutputNode::removeOutputFromList( InputNode* toRemove ) {
    std::vector<InputNode*>::iterator it = outputs.begin();
    while( it != outputs.end() ) {
        if( ( *it ) == toRemove ) {
            outputs.erase( it );
            connections--;
            if( connections == 0 ) state = Changed;
            break;
        }
        it++;
    }
}

void pdsp::OutputNode::addInputToList( InputNode* input ) {
    //check if input is already in the list
    bool notFound = true;
    for( InputNode* &node : outputs ) {
        if( node == input ) {
            notFound = false;
        }
    }
    if( notFound ) {
        outputs.push_back( input );
        connections++;
    }
}


pdsp::OutputNode&  pdsp::OutputNode::setMultiplyOnConnection( float multiplier ) {
    nextMultiplier = multiplier;
    multiply = true;
    return *this;
}


float pdsp::OutputNode::getCRValue() const{
    return buffer[0];
}

const float* pdsp::OutputNode::getBuffer() const {
    return buffer;
}

const int pdsp::OutputNode::getState() const {
    return state;
}

bool pdsp::OutputNode::isConnected() {
    return !( connections==0 );
}
bool pdsp::OutputNode::notConnected() {
    return ( connections==0 );
}

const int pdsp::OutputNode::getOversampleLevel() const {
    return oversampleLevel;
}

bool pdsp::OutputNode::exists() {
    return true;
}

void pdsp::OutputNode::setTurn( int newTurn ) {
    globalProcessingTurnId = newTurn;
}

void pdsp::OutputNode::nextTurn() {
    globalProcessingTurnId++;
}

const int pdsp::OutputNode::getGlobalProcessingTurnId() {
    return globalProcessingTurnId;
}

pdsp::Unit* pdsp::OutputNode::getParent() const {
    return parent;
}
void pdsp::OutputNode::setParent( Unit* parent ) {
    this->parent = parent;
}

void pdsp::OutputNode::updateTurnId() {
    lastProcessedTurnId = globalProcessingTurnId;
}
    
//----------------------------VALUE NODE--------------------------------

pdsp::ValueNode::ValueNode() {
    state = Changed;
    nodeValue.store(0.0f);
    buffer = nullptr;
    parent = nullptr;
    oversampleLevel = -1;
}

pdsp::ValueNode::ValueNode( const ValueNode& other ) {
    state = Changed;
    nodeValue.store(other.nodeValue.load());
    buffer = nullptr;
    parent = nullptr;
    oversampleLevel = -1;
}

pdsp::ValueNode& pdsp::ValueNode::operator= ( const ValueNode& other ) {
    nodeValue.store(other.nodeValue.load());
    return *this;
}

pdsp::ValueNode::~ValueNode() {
    if( buffer != nullptr ) {
        ofx_deallocate_aligned( buffer );
    }
}

void pdsp::ValueNode::setOversampleLevel( int newOversample ) {
    oversampleLevel = -1;
}

void pdsp::ValueNode::set( float value ) {
    nodeValue.store(value);
}

float pdsp::ValueNode::value() {
    return nodeValue.load();
}

pdsp::OutputNode& pdsp::ValueNode::setAndGetNode( float value ) {
    nodeValue.store(value);
    return *this;
}

float pdsp::ValueNode::getCRValue() const{
    return nodeValue.load();   
}

void pdsp::ValueNode::prepareToPlay( int expectedBufferSize, double sampleRate ) {  }

void pdsp::ValueNode::releaseResources() {  }



//-------------------------------Unit METHODS---------------------------------

pdsp::Unit::Unit() {
    oversample = initOversampleLevel;
}

pdsp::Unit::Unit(const Unit & other){
    std::cout<<"[pdsp] warning, Unit copy constructed, undefined behavior\n";
    pdsp_trace();
}
pdsp::Unit& pdsp::Unit::operator= (const Unit & other){
    std::cout<<"[pdsp] warning, Unit copied, undefined behavior\n";
    pdsp_trace();
    return *this;
}

pdsp::Unit::Unit (Unit&& other){
    std::cout<<"[pdsp] warning, Unit move constructed, undefined behavior\n";
    pdsp_trace();
}

pdsp::Unit& pdsp::Unit::operator= (Unit&& other){
    std::cout<<"[pdsp] warning, Unit moved, undefined behavior\n";
    pdsp_trace();
    return *this;
}

void pdsp::Unit::prepareToPlay( int expectedBufferSize, double sampleRate ) {
    prepareUnit( expectedBufferSize * oversample, sampleRate * static_cast<double>( oversample ) );
}

const float* pdsp::Unit::processInput( InputNode& node, int& stateNow ) {
    node.process();
    stateNow = node.getState();
    return node.getBuffer();
}

float* pdsp::Unit::getOutputBufferToFill( OutputNode& node ) {
    node.state = AudioRate;
    return node.buffer;
}


void pdsp::Unit::setControlRateOutput( OutputNode& node, float scalarValue ) {
    node.state = Changed;
    node.buffer[0] = scalarValue;
}

void pdsp::Unit::setOutputToZero( OutputNode& node ) {
    node.state = Changed;
    node.buffer[0] = 0.0f;
}

float pdsp::Unit::processAndGetSingleValue( InputNode &input, int pos ) {
    input.process();
    if( input.state==AudioRate ) {
        return input.buffer[pos];
    } else {
        return input.buffer[0];
    }
}

void pdsp::Unit::setOversampleLevel( int newOversampleLevel ) {
    oversample = newOversampleLevel;

    for( NamedInput &item : inputs ) {
        item.input->setRequiredOversampleLevel( newOversampleLevel );
    }
    for( NamedOutput &item : outputs ) {
        item.output->setOversampleLevel( newOversampleLevel );
    }
    if( dynamicConstruction ) {
        prepareToPlay( globalBufferSize, globalSampleRate );
    }
}

int pdsp::Unit::getOversampleLevel() const {
    return oversample;
}


//--------------------------PATCHABLE----------------


pdsp::Patchable::Patchable() {
    inputs.reserve( 8 );
    outputs.reserve( 8 );
    inputs.clear();
    outputs.clear();
    resetInputToDefault();
    resetOutputToDefault();
}


void pdsp::Patchable::resetInputToDefault() {
    if( inputs.empty() ) {
        selectedInput = reinterpret_cast<InputNode*>( &invalidInput );
    } else {
        selectedInput = inputs[0].input;
    }
}

void pdsp::Patchable::resetOutputToDefault() {
    if( outputs.empty() ) {
        selectedOutput = reinterpret_cast<OutputNode*>( &invalidOutput );
    } else {
        selectedOutput = outputs[0].output;
    }
}


void pdsp::Patchable::addOutput( const char* tag, OutputNode &output ) {
    outputs.push_back( NamedOutput( tag, &output ) );
    resetOutputToDefault();
}

void pdsp::Patchable::addInput( const char* tag, InputNode &input ) {
    inputs.push_back( NamedInput( tag, &input ) );
    resetInputToDefault();
}


pdsp::InputNode& pdsp::Patchable::getSelectedInput()  {
    if ( selectedInput == reinterpret_cast<InputNode*>( &invalidInput ) ){
        std::cout<< "[pdsp] warning! patching to invalid input, probably this unit/module has no input or you have used an invalid tag\n";
        pdsp_trace();
    }  
    return *selectedInput;
}
pdsp::OutputNode& pdsp::Patchable::getSelectedOutput() {
    if ( selectedOutput == reinterpret_cast<OutputNode*>( &invalidOutput ) ){
        std::cout<< "[pdsp] warning! patching to invalid output, probably this unit/module has no output or you have used an invalid tag\n";
        pdsp_trace();
    }
    return *selectedOutput;
}



pdsp::Patchable&  pdsp::Patchable::in( const char* tag ) {
    for( NamedInput &item : inputs ) {
        if( std::strcmp( item.tag, tag )==0 ) {
            selectedInput = item.input;
            return *this;
        }
    }
    selectedInput = reinterpret_cast<InputNode*>( &invalidInput );
    std::cout<<"[pdsp] unexistent input selected!!!\n";
    pdsp_trace();
    return *this;
}

pdsp::Patchable&  pdsp::Patchable::out( const char* tag ) {
    for( NamedOutput &item : outputs ) {
        if( std::strcmp( item.tag, tag )==0 ) {
            selectedOutput = item.output;
            return *this;
        }
    }
    selectedOutput = reinterpret_cast<OutputNode*>( &invalidOutput );
    std::cout<<"[pdsp] unexistent output selected!!!";
    pdsp_trace();
    return *this;
}


void pdsp::Patchable::addUnitOutput( const char* tag, Patchable &unit ){
    addOutput(tag, unit.getSelectedOutput() );
    unit.resetOutputToDefault();
    resetOutputToDefault();
}
    
    
void pdsp::Patchable::addUnitInput( const char* tag, Patchable &unit ){
    addInput(tag, unit.getSelectedInput() );
    unit.resetInputToDefault();
    resetInputToDefault();
}

void pdsp::Patchable::addModuleOutput( const char* tag, Patchable &unit ){
    addOutput(tag, unit.getSelectedOutput() );
    unit.resetOutputToDefault();
    resetOutputToDefault();
}
    
    
void pdsp::Patchable::addModuleInput( const char* tag, Patchable &unit ){
    addInput(tag, unit.getSelectedInput() );
    unit.resetInputToDefault();
    resetInputToDefault();
}

void pdsp::Patchable::clearAllAddedNodes(){
    
    for( NamedInput &item : inputs ) {
        item.input->disconnectAll();
    }
    for( NamedOutput &item : outputs ) {
        item.output->disconnectAll();
    }

    inputs.clear();
    outputs.clear();
}
    
    
void pdsp::Patchable::disconnectIn(){
    if(selectedInput->exists()){
        selectedInput->disconnectAll();
    }
    this->resetInputToDefault();
}


void pdsp::Patchable::disconnectOut(){
    if(selectedOutput->exists()){
        selectedOutput->disconnectAll();
    }
    this->resetOutputToDefault();    
}


void pdsp::Patchable::disconnectAll(){
    for( NamedInput &item : inputs ) {
        item.input->disconnectAll();
    }
    for( NamedOutput &item : outputs ) {
        item.output->disconnectAll();
    }    
    this->resetInputToDefault();   
    this->resetOutputToDefault();   
}
    
std::vector<std::string> pdsp::Patchable::getInputsList(){
    std::vector<std::string> toReturn;
    toReturn.reserve(inputs.size());
    for(int i=0; i<inputs.size(); ++i){
        toReturn.push_back(inputs[i].tag);
    }
    return toReturn;
}

std::vector<std::string> pdsp::Patchable::getOutputsList(){
    std::vector<std::string> toReturn;
    toReturn.reserve(outputs.size());
    for(int i=0; i<outputs.size(); ++i){
        toReturn.push_back(outputs[i].tag);
    }
    return toReturn;
}


void pdsp::Unit::updateOutputNodes() {
    for( NamedOutput &item : outputs ) {
        item.output->setParent( this );
    }
    resetInputToDefault();
    resetOutputToDefault();
}

