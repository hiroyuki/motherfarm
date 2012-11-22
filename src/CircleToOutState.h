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
    CircleToOutState() : CircleToCenterState()
    {
    }
    
    virtual void setSharedData( SharedData* sharedData)
    {
        this->sharedData = sharedData;
        gui.addToggle("CircleToOutState", isActive);
    }
    
    virtual void setup()
    {

        CircleToCenterState::setup();
        toCenter = false;
        longestLen = sqrt(pow(SVG_WIDTH, 2.f) + pow(SVG_HEIGHT, 2.f));
    }
    
    virtual void stateEnter()
    {
        CircleToCenterState::stateEnter();
        toCenter = false;
    }
    
    virtual string getName()
    {
        return "CircleToOutState";
    }
    
};


#endif
