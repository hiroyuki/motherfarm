//
//  FillCircleState.h
//  motherfarmLED
//
//  Created by motherfarm on 11/19/12.
//
//

#ifndef motherfarmLED_FillCircleState_h
#define motherfarmLED_FillCircleState_h
#include "BaseState.h"

class FillCircleState : public BaseState
{
public:
    FillCircleState(SharedData *sharedData):BaseState(sharedData)
    {
        gui.addToggle("FillCircleState", isActive);
    }
    
    void setup()
    {
        BaseState::setup();
    }
    
    void stateEnter()
    {
        BaseState::stateEnter();
    }
    
    void update()
    {
        BaseState::update();
        fbo->begin();
        ofClear(0);
        sharedData->drawStars(alpha*(sharedData->curveB));
        fbo->end();
    }
    
    void draw()
    {
        BaseState::draw();
    }
    
    string getName()
    {
        return "FillCircleState";
    }
};


#endif
