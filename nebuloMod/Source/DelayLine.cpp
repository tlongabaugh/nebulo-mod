//
//  DelayLine.cpp
//  NebuloMod
//
//  Created by Tom Longabaugh on 12/13/14.
//
//

#include "DelayLine.h"
#include "../JuceLibraryCode/JuceHeader.h"

DelayLine::DelayLine() {
    memset(memory, 0, 88200*sizeof (float));
    
}
float DelayLine::dl(float input, int size, double feedback)
{
    if ( phase >=size ) {
        phase = 0;
    }
    output=memory[phase];
    memory[phase]=(memory[phase]*feedback)+(input*feedback)*0.5;
    phase+=1;
    return(output);
    
}

float DelayLine::dl(float input, int size, double feedback, int position)
{
    if ( phase >=size ) phase = 0;
    if ( position >=size ) position = 0;
    output=memory[position];
    memory[phase]=(memory[phase]*feedback)+(input*feedback);
    phase+=1;
    return(output);
    
}