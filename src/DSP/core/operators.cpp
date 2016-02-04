
#include "operators.h"


void pdsp::operator>> (OutputNode& output, InputNode& input){
    input.connect(output);
}

pdsp::Patchable& pdsp::operator>> (OutputNode& output, Patchable& input){
    input.getSelectedInput().connect(output);
    input.resetInputToDefault();
    return input;
}

void pdsp::operator>> (float value, InputNode& input){
    input.connectFloat(value);
}

pdsp::OutputNode& pdsp::operator>> (float lhs, ValueNode& node){
    return node.setAndGetNode(lhs);
}


void pdsp::operator>> (Patchable& output, InputNode& input){
    input.connect(output.getSelectedOutput());
    output.resetOutputToDefault();
}

pdsp::Patchable& pdsp::operator>> (Patchable& output, Patchable& input){
    input.getSelectedInput().connect(output.getSelectedOutput());
    output.resetOutputToDefault();
    input.resetInputToDefault();
    return input;
}

pdsp::Patchable& pdsp::operator>> (float value, Patchable& input){
    input.getSelectedInput().connectFloat(value);
    input.resetInputToDefault();
    return input;
}


//DISCONNECT
void pdsp::operator!= (InputNode& input, OutputNode& output){
    input.disconnect(output);
}

void pdsp::operator!= (OutputNode& output, InputNode& input){
    input.disconnect(output);
}

void pdsp::operator!= (Patchable& input, OutputNode& output){
    input.getSelectedInput().disconnect(output);
    input.resetInputToDefault();
}

void pdsp::operator!= (OutputNode& output, Patchable& input){
    output.disconnect(input.getSelectedInput());
    input.resetInputToDefault();
}

void pdsp::operator!=  (InputNode& input, Patchable& output){
    input.disconnect(output.getSelectedOutput());
    output.resetOutputToDefault();
}

void pdsp::operator!=  (Patchable& output, InputNode& input){
    input.disconnect(output.getSelectedOutput());
    output.resetOutputToDefault();
}


//MULTIPLY
pdsp::OutputNode& pdsp::operator* (OutputNode& output, float multiplier){
    output.setMultiplyOnConnection(multiplier);
    return output;
}

pdsp::Patchable& pdsp::operator* (Patchable& patch, float multiplier){
    patch.getSelectedOutput().setMultiplyOnConnection(multiplier);
    return patch;
}


//assignment from left operator

void pdsp::operator<< (InputNode& input, float value){
    input.connectFloat(value);
}

void pdsp::operator<< (ValueNode& node, float value){
    node.set(value);
}
