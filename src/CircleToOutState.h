//
//  CircleToOutState.h
//  motherfarmLED
//
//  Created by 堀 宏行 on 11/5/12.
//
//

#ifndef motherfarmLED_CircleToOutState_h
#define motherfarmLED_CircleToOutState_h
#include "CircleToCenterState.h"
class CircleToOutState : public CircleToCenterState
{
public:
    void stateEnter()
    {
        toCenter = false;
    }
    
    string getName()
    {
        return "CircleToOutState";
    }
    
};


#endif
