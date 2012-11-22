//
//  WindowState.h
//  motherfarmLED
//
//  Created by motherfarm on 11/18/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef motherfarmLED_WindowState_h
#define motherfarmLED_WindowState_h
#include "BaseState.h"
class WindowState : public BaseState
{
public:
    WindowState(SharedData *sharedData):BaseState(sharedData)
    {
        gui.addToggle("WindowState", isActive);
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
    }
    
    void draw()
    {
        BaseState::draw();
    }
    
    string getName()
    {
        return "WindowState";
    }
};
#endif
