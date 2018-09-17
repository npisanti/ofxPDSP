
#include "StereoDelay.h"

StereoDelay::Submodule::Submodule(){
    addModuleInput( "signal", input );
    addModuleOutput( "signal", output );
}

void StereoDelay::patch() {

    channels.resize(2);
    
    addModuleInput("L", channels[0] );
    addModuleInput("R", channels[1] );
    addModuleOutput("L", channels[0] );
    addModuleOutput("R", channels[1] );

    channels[0].input >> inputFader.ch(0) >> delay0 >> outputFader.ch(0) >> channels[0].output;
    channels[1].input >> inputFader.ch(1) >> delay1 >> outputFader.ch(1) >> channels[1].output;

    time * (1.f/16.f) >> lDelayTimeControl >> delay0.in_time();
    time * (1.f/16.f) >> rDelayTimeControl >> delay1.in_time();

    lFeedbackControl >> delay0.in_feedback();
    rFeedbackControl >> delay1.in_feedback();
    dampingControl >> delay0.in_damping();
    dampingControl >> delay1.in_damping();

    parameters.setName("stereo delay");

    parameters.add( inputFader.set("input gain", -15, -48, 12) );
    
    parameters.add( lDelayTimeControl.set("L 16th time", 5, 1, 16) );
    parameters.add( lFeedbackControl.set("L feedback", 0.4f, 0.0f, 1.0f));
    parameters.add( rDelayTimeControl.set("R 16th time", 3, 1, 16) );
    parameters.add( rFeedbackControl.set("R feedback", 0.4f, 0.0f, 1.0f));   
    parameters.add( dampingControl.set("damping", 0.5f, 0.0f, 1.0f));   
   
    parameters.add( outputFader.set("output gain", 0, -48, 12) );

    inputFader.enableSmoothing(50.f);
    outputFader.enableSmoothing(50.f);
    lDelayTimeControl.enableSmoothing(450.f);
    lDelayTimeControl.enableSmoothing(450.f);
    lFeedbackControl.enableSmoothing(50.f);
    rFeedbackControl.enableSmoothing(50.f);

}

pdsp::Patchable & StereoDelay::ch( int index ){
    index = index%2; // wrap to stereo 
    return channels[index];
}
