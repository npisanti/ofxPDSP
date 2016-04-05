
// BasicNodes.h
// ofxPDSP
// Nicola Pisanti, MIT License, 2016


#ifndef PDSP_CORE_BASICNODES_H_INCLUDED
#define PDSP_CORE_BASICNODES_H_INCLUDED



#include <vector>
#include <math.h>
#include <cassert>
#include <stdint.h>
#include <limits>
#include "../pdspFunctions.h"
#include "../pdspConstants.h"
#include "../../math/header.h"
#include "Preparable.h"
#include <cstring>
#include <atomic>
#include <iostream>
#include "../../flags.h"

namespace pdsp {

class Patchable;
class InputNode;
class OutputNode;
class ValueNode;
class NullInput;
class NullOutput;

/*!
    @cond HIDDEN_SYMBOLS
*/

//-------------------------------------OUTPUT DATA------------------------------------------------
class OutputData {
public:
    OutputData( OutputNode* node, bool multiply, float multiplier )
        : node( node ), multiply( multiply ), multiplier( multiplier ) {  };

    OutputNode* node;
    bool multiply;
    float multiplier;
};

//----------------------------------NODE REFERENCES--------------------------

class NamedInput {
public:
    NamedInput( const char* tag, InputNode* pInput ) : tag( tag ), input( pInput ) {};

    const char*     tag;
    InputNode*      input;
};

class NamedOutput {
public:
    NamedOutput( const char* tag, OutputNode* pOutput ) : tag( tag ), output( pOutput ) {};

    const char*     tag;
    OutputNode*     output;
};

/*!
    @endcond
*/

//--------------------------------PATCHABLE------------------------------------------



/*!
@brief Abstract class for implementing Units and Modules.

Patchable is a base class that rapresent everything that has in_ and out_ methods and can be patched with the >> operator. It is a superclass of Unit and it can be used as superclass to implement modules ( classes made from pre-patched Units ) 

*/
class Patchable {
    friend class Unit;
    friend class InputNode;
    
public:
    Patchable();

    /*!
    @brief resets the selected input to the default.
    */ 
    void    resetInputToDefault();
    /*!
    @brief resets the selected output to the default.
    */ 
    void    resetOutputToDefault();

    /*!
    @brief sets the selected input to the one tagged with "tag", if tag is a valid string, and returns this Patchable ready to be patched.
    @param[in] tag tag of input to select
    
    sets the selected input to the one tagged with "tag", if tag is a valid string, and returns this Patchable ready to be patched. If an invalid tag is given the selected input is set to a dummy input that will invalidate the next >> operation, a message will be sent to std::cout and an assert will be raised if you are debugging.
    */
    Patchable&         in( const char* tag );
    
    
    /*!
    @brief sets the selected output to the one tagged with "tag", if tag is a valid string, and returns this Patchable ready to be patched.
    @param[in] tag tag of output to select
    
    sets the selected output to the one tagged with "tag", if tag is a valid string, and returns this Patchable ready to be patched. If an invalid tag is given the selected input is set to a dummy output that will invalidate the next >> operation, a message will be sent to std::cout and an assert will be raised if you are debugging.
    */   
    Patchable&         out( const char* tag );
    
    /*!
    @brief disconnects all the outputs connected to the selected input, then resets the selected input to the default.
    */ 
    void disconnectIn();  
    
    /*!
    @brief disconnects all the inputs connected to the selected output, then resets the selected output to the default.
    */   
    void disconnectOut();
    
    /*!
    @brief disconnects all the inputs and outputs to their connected outputs and inputs. Also resets the selected input and output to the default.
    */   
    void disconnectAll();

    /*!
    @brief returns the selected input, you can patch a Patchable or OutputNode to it with the >> operator.
    */   
    InputNode& getSelectedInput();
    
    /*!
    @brief returns the selected output, you can patch it to another Patchable or InputNode with the >> operator.
    */   
    OutputNode& getSelectedOutput();

    /*!
    @brief returns a vector of the valid input tags
    */  
    std::vector<std::string> getInputsList();
    
    /*!
    @brief returns a vector of the valid input tags
    */   
    std::vector<std::string> getOutputsList();

protected:

    /*!
    @brief method to add outputs when constructing a Unit
    @param[in] tag tag name to assign to the output
    @param[in] output output to add to available Unit outputs
        
    When you are constructing a new Unit you use this method to add an output to the ones available for patching for this unit. Remember also to call the Unit updateOutputNodes() method. The first output added will became the default output.
    */  
    void    addOutput( const char* tag, OutputNode & output );
    
    /*!
    @brief method to add inputs when constructing a Unit
    @param[in] tag tag name to assign to the input
    @param[in] input input to add to available Unit inputs
        
    When you are constructing a new Unit you use this method to add an input to the ones available for patching for this unit.
    */ 
    void    addInput( const char* tag, InputNode & input );

    /*!
    @brief method to add outputs when constructing a module
    @param[in] tag tag name to assign to the output.
    @param[in] unit the selected output of this unit will be added.
        
    When you are constructing a new module you use this method to add an output to the ones available for patching for this module. The actually selected output will be added, so you can use the in("tagname") or in_tagname() methods on the unit that you are passing as argument. The method will also reset the Unit selected output to the default.
    */  
    void    addModuleOutput( const char* tag, Patchable & unit );
    
    /*!
    @brief method to add outputs when constructing a module
    @param[in] tag tag name to assign to the output.
    @param[in] unit the selected input of this unit will be added.
        
    When you are constructing a new module you use this method to add an input to the ones available for patching for this module. The actually selected input will be added, so you can use the out("tagname") or out_tagname() methods on the unit that you are passing as argument. The method will also reset the Unit selected input to the default.
    */  
    void    addModuleInput( const char* tag, Patchable & unit );

/*!
    @cond HIDDEN_SYMBOLS
*/
    [[deprecated("Replaced by addModuleOutput() for a less ambigous nomenclature")]]
    void    addUnitOutput( const char* tag, Patchable & unit );
    
    [[deprecated("Replaced by addModuleInput() for a less ambigous nomenclature")]]
    void    addUnitInput( const char* tag, Patchable & unit );
/*!
    @endcond
*/    

private:
    std::vector<NamedInput>         inputs;
    std::vector<NamedOutput>        outputs;

    static NullInput        invalidInput;
    static NullOutput       invalidOutput;
    
    InputNode*      selectedInput;
    OutputNode*     selectedOutput;
    
    void    clearAllAddedNodes();
};


/*!
@brief Abstract class for implementing Units

Unit is the base class for implementing all the DSPs. It has inputs and outputs for patching and process them in the process() code.
*/

//-------------------------------------Unit------------------------------------------------

class Unit :  public Preparable, public Patchable {
    friend class InputNode;

public:
    Unit();
    Unit(const Unit & other);
    Unit& operator= (const Unit & other);
    Unit (Unit&& other);
    Unit& operator= (Unit&& other); 
    
    /*!
    @brief sets the unit oversample level
    @param[in] newOversampleLevel the oversample level, 2 for 2x, 4 for 4x, etc etc

    This method let you set the Unit oversample level, the Unit will run at a multiple of the sample rate. Remember to upsample the inputs with an UpSampler and downsample the outputs with a DownSampler
    */     
    virtual void setOversampleLevel( int newOversampleLevel );
    
    /*!
    @brief returns the Unit oversample level
    */  
    int getOversampleLevel() const;

protected:

    /*!
    @brief the unit DSP method to override
    @param[in] bufferSize the size of the buffersize, already multiplied if the Unit is oversampling

    This is the method to implement for calling the inputs processing methods, process the signal and render then in the output buffers. Remember that some clients not use a buffer size of constant size, and you can't be even shure that is a multiple of 2. Remember to set all the methods inside process() to noexcept, and absolutely avoid allocating and deallocating memory. Avoid branches if possible, and if needed "switch" is preferred to "if".
    */  
    virtual void process( int bufferSize ) noexcept = 0;
    
    /*!
    @brief this method will be called to prepare the unit to play.
    @param[in] expectedBufferSize the size of the expected buffersize, already multiplied if the Unit is oversampling
    @param[in] sampleRate the sample rate, already multiplied if the Unit is oversampling

    This method will be called before playing to prepare the Unit to play, remember that in some cases (when programming vst/au) it could be called more than once without even calling releaseResources()
    */ 
    virtual void prepareUnit( int expectedBufferSize, double sampleRate ) = 0;
  
  
    /*!
    @brief this method will be called to release the unit resoruces.

    This method will be called when pdsp::releaseAll() is called, so in some cases it could be called more than once in succession without a corresponding prepareToPlay() call, and it is also called on destruction, so make it robust enough.
    */ 
    virtual void releaseResources( ) override = 0;    
    
    /*!
    @brief you have to use this method only in the constructor, inside if(dynamicConstruction){ }
    @param[in] expectedBufferSize the size of the expected buffersize
    @param[in] sampleRate the sample rate

    This method will be called before playing to prepare the Unit to play, it will call also prepareUnit(...) with the correct sample rates and buffer sizes, the method is protected so you can use it in your constructor method, it will resolve some problems that could arise only when allocating and deallocating Units when the program is already running (more on this later in some wiki...)
    */  
    void prepareToPlay( int expectedBufferSize, double sampleRate ) override;
    
    //functions to manipulate ins/outs---------------------------------------------
    
    /*!
    @brief process the given input, gives you a pointer to the processed buffer as result and its state
    @param[in] node InputNode to process
    @param[in] stateNow this int will be set to the input state after processing (Unchanged==0, Changed==1, AudioRate==2)

    This method will process the given input and give you a pointer as result. If the state is Unchanged or Changed only the first value of the given array is meaningful, otherwise the full array rapresent the buffer to process.
    */  
    const float*    processInput( InputNode& node, int& stateNow );
    
    /*!
    @brief process the given input, gives you a pointer to the processed buffer as result and its state
    @param[in] node InputNode to process
    @param[in] pos position of the value inside the buffer to get. WARNING: boundary not checked!!!

    This method will process the given input and give you the value at pos as result. If the input state after processing is Unchanged or Changed it will give the value at index 0 as result.
    */  
    float           processAndGetSingleValue( InputNode& input, int pos ); //WARNING! pos is not checked

    //const float*    getOutputBuffer( OutputNode& node );
    
    
    /*!
    @brief gets a pointer to the output buffer for rendering audio to it, and sets the OutputNode state to AudioRate
    @param[in] node OutputNode to render

    This method will give you a buffer to fill with the result of the DSP
    */  
    float*          getOutputBufferToFill( OutputNode& node );
    
    /*!
    @brief sets the OutputNode buffer to "Changed" and its first value to scalarValue
    @param[in] node OutputNode to render
    @param[in] scalarValue OutputNode value
    
    If you know that the output of your DSP will be constant for all the buffer size use this method. This method flags also the OutputNode as "Changed", it means that it isn't running at audio rate and this will save some cpu for other Unit that have this output patched to them.
    */  
    void            setControlRateOutput( OutputNode& node, float scalarValue );
    
    /*!
    @brief sets the OutputNode buffer to "Changed" and its first value to 0.0f
    @param[in] node OutputNode to render
    
    If you know that the output of your DSP will be constant for all the buffer size use this method. This method flags also the OutputNode as "Changed", it means that it isn't running at audio rate and this will save some cpu for other Unit that have this output patched to them. As you are also setting the value to 0.0f, this will save even more cpu... for example when Amp know that has to multiply for 0.0f it doesn't even process the in_signal() inputs and directly set the result to 0.0f.
    */   
    void            setOutputToZero( OutputNode& node );

    /*!
    @brief call this method in the Unit constructor after adding all the Outputs, this is mandatory for the inner working of the patching framework.
    */  
    void            updateOutputNodes();
    
    virtual ~Unit() {};    
    

    
private:

    int oversample;

};


//-------------------------------------OUTPUT NODE------------------------------------------------

/*!
@brief Output of a Unit, contains a buffer of rendered floats and has a variable state flag.

OutputNode is the output point of a Unit. It has an internal buffer of floats and a state that can be "Changed" or "AudioRate".
*/

class OutputNode : public Preparable {
    friend class InputNode;
    friend class Unit;
    friend class OutputData;
    friend class PatchNode;
    friend class Switch;
    friend class ExternalInput;
    friend class Processor;
    friend class UpSampler;
    friend class DownSampler;
    friend class Patchable;
    
public:

    OutputNode( int oversample );
    OutputNode();
    OutputNode( const OutputNode& other );
    OutputNode& operator= ( const OutputNode& other );

    virtual ~OutputNode();
    
    /*!
    @brief patch this OutputNode to an InputNode
    @param[in] node InputNode to patch
    */   
    void connect( InputNode& inputNode );
    
    /*!
    @brief disconnect this OutputNode from an InputNode
    @param[in] node InputNode to disconnect
    */   
    void disconnect( InputNode& inputNode );
    
    /*!
    @brief disconnect this OutputNode from all the inputs to witch it is connected
    */   
    void disconnectAll();

    /*!
    @brief returns the first value of the buffer, to use if the state is "Changed"
    */ 
    virtual float getCRValue();

    /*!
    @brief get the OutputNode buffer
    */ 
    const float* getBuffer() const;
    
    /*!
    @brief get the OutputNode state
    */ 
    const int getState() const;
    
    /*!
    @brief returns true if this OutputNode is connected to at least an InputNode
    */
    bool isConnected();
    
    /*!
    @brief returns true if this OutputNode is not connected to any InputNode
    */
    bool notConnected();
    
    /*!
    @brief returns the oversample level of this OutputNode
    */
    const int getOversampleLevel() const;
    
    /*!
    @brief this is basically what the * operator does when used with a Patchable or OutputNode
    */
    OutputNode& setMultiplyOnConnection( float multiplier );

protected:

    float* buffer;
    int state;
    Unit* parent;   
    int oversampleLevel;
    
private:

    void prepareToPlay( int expectedBufferSize, double sampleRate ) override;
    void releaseResources() override;
    virtual void setOversampleLevel( int newOversample );

    virtual bool exists();

    void addInputToList( InputNode* node );
    void removeOutputFromList( InputNode* toRemove );

    int connections;
    std::vector<InputNode*> outputs;

    static int globalProcessingTurnId;

    int baseBufferSize;
    
    static void setTurn( int newTurn ) ;
    
    static void nextTurn();
    static const int getGlobalProcessingTurnId();

    Unit* getParent() const;
    void setParent( Unit* parent );
    void updateTurnId();
    
    bool multiply;
    float nextMultiplier;

    int lastProcessedTurnId;

};



//-------------------------------------VALUE NODE------------------------------------------------
/*!
@brief An OutputNode that is always set at control rate with a set value. Thread-Safe to set.

ValueNode can be use as OutputNode but it holds a single scalar value that is thread-safe to set even when the processing is running.
*/

class ValueNode :  public OutputNode {

public:
    ValueNode();
    ValueNode( const ValueNode& other );
    ValueNode& operator= ( const ValueNode& other );
    ~ValueNode();


    /*!
    @brief returns the ValueNode value
    */ 
    float getCRValue() override;
    
    /*!
    @brief set the value
    @param[in] value value to set
    */ 
    void set( float value );
    
    /*!
    @brief returns the ValueNode value
    */ 
    float value();
    
    /*!
    @brief set the value and return self as OutputNode
    @param[in] value value to set
    */ 
    OutputNode& setAndGetNode( float value );

private:
    void prepareToPlay( int expectedBufferSize, double sampleRate ) override;
    void releaseResources() override;
    void setOversampleLevel( int newOversample ) override;    
    std::atomic<float> nodeValue;

};


//-------------------------------------INPUT NODE------------------------------------------------
/*!
@brief Input of a Unit, process all the connected Outputs and sum them. Has an internal variable state.

InputNode is the input point of a Unit, it has one or more OutputNode patched to it and sums them. It also has an internal state, so after summing the patched outputs it update the state. AudioRate is at least one OutputNode was running at audio rate, "Changed" if all the patched output were ranning at control rate and "Unchanged" if all the patched outputs were running at control rate and the summed value is the same as the last processed value.
*/
class InputNode : public Preparable
    //public Input 
    {
    friend class OutputNode;
    friend class Unit;
    friend class PatchNode;
    friend class Switch;
    friend class UpSampler;
    friend class DownSampler;
    friend class Patchable;

public:
    InputNode( int oversample );
    InputNode();
    virtual ~InputNode();
    InputNode( const InputNode& other ); // copy constructor
    InputNode& operator= ( const InputNode& other ); // copy assignment
    
    
    /*!
    @brief patchs this InputNode to an OutputNode
    @param[in] node OutputNode to patch
    */   
    void connect( OutputNode& outputNode );


  
    /*!
    @brief disconnects this InputNode from an OutputNode
    @param[in] outputNode OutputNode to disconnect
    */   
    virtual void disconnect( OutputNode& outputNode );
    
    /*!
    @brief disconnects this InputNode from all the outputs to witch it is connected, including its internal ValueNode
    */   
    virtual void disconnectAll();

    /*!
    @brief patchs this InputNode to an internally stored ValueNode. This also override the value set by setDefaultValue().
    @param[in] value value to set to the internal ValueNode
    
    patch this InputNode to an internally stored ValueNode. This also override the value set by setDefaultValue(). This is what happens when you use the >> with a float to the left and an InputNode to the righ (or << with arguments reversed).
    */   
    virtual void connectFloat( float value );
    
    /*!
    @brief disconnects this InputNode from the internally store ValueNode
    */   
    void disconnectFloat();

    /*!
    @brief gets the InputNode buffer
    */ 
    const float* getBuffer() const;
    
    /*!
    @brief gets the InputNode state
    */ 
    const int getState() const;
    
    /*!
    @brief sets the value that this input will assume if there are no OuputNode patched to it.
    @param[in] newDefault default value to set
    
    Set the value that this input will assume if there are no OuputNode patched to it. This is different than connecting the internal float, with float >> InputNode, as the internal float will be summed to other OutputNodes, instead patching an OutputNode will override this default value. Ok so easy rule: everytime you use the >> operator will override the default value. Init default value is 0.0f.
    */   
    void setDefaultValue( float newDefault );
    
    
    /*!
    @brief returns true if this InputNode is connected to at least an OutputNode
    */  
    bool isConnected();
    
    /*!
    @brief sets some boundaries to which the values of the buffer will be clamped
    @param[in] lo low boundary (inclusive)
    @param[in] hi high boundary (inclusive)
    */  
    void enableBoundaries( float lo, float hi );
    
    /*!
    @brief disables the clamping of buffer values
    */  
    void disableBoundaries();
    
    /*!
    @brief returns the oversample level required by this InputNode
    */  
    int getRequiredOversampleLevel() const;
    
    //void connectExclusive( OutputNode& outputNode );    
    //virtual void connectFloatExclusive( float value );
protected:

    virtual void setRequiredOversampleLevel( int newOversample );

private:

    void prepareToPlay( int expectedBufferSize, double sampleRate ) override;
    void releaseResources() override;
    void process() noexcept;

    virtual void removeInputUnilateral( const OutputNode& outputNode );

    virtual bool exists();

    float* buffer;
    float* sumBuffer;
    int state;

    bool clampToBoundaries;
    float lowBoundary;
    float highBoundary;
    
    int connections;
    std::vector<OutputData> inputs;

    bool internalScalarIsConnected;
    ValueNode internalScalar;

    int baseBufferSize;

    float lastValue;
    float defaultValue;
    
    int requiredOversampleLevel;   
};

/*!
    @cond HIDDEN_SYMBOLS
*/

//-----------------------------------NULL OUTPUT----------------------------

class NullOutput : public ValueNode {

private:
    virtual bool exists() override {
        return false;
    }
    
};


//-------------------------------------NULL INPUT---------------------------------------

class NullInput : public InputNode {

public:
    NullInput() {}

private:
    bool exists()override {
        return false;
    }
    
    void prepareToPlay( int expectedBufferSize, double sampleRate ) override {}
    void releaseResources() override {}

};

/*!
    @endcond
*/

} // namespace pdps END



#endif  // BASICNODES_H_INCLUDED
