//
//  DelayLine.h
//  NebuloMod
//
//  Created by Tom Longabaugh on 12/13/14.
//
//

#ifndef __NebuloMod__DelayLine__
#define __NebuloMod__DelayLine__

#include <stdio.h>

class DelayLine {
    float frequency;
    int phase;
    float startphase;
    float endphase;
    float output;
    float memory[88200];
    
public:
    DelayLine();
    float dl(float input, int size, double feedback);
    float dl(float input, int size, double feedback, int position);
    
    
};


#endif /* defined(__NebuloMod__DelayLine__) */
