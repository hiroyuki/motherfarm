//
//  ClickState.h
//  motherfarmLED
//
//  Created by motherfarm on 11/19/12.
//
//

#ifndef motherfarmLED_ClickState_h
#define motherfarmLED_ClickState_h
#include "BaseState.h"
#include "StaticCircle.h"

class ClickState : public BaseState
{
public:
    vector< StaticCircle > circles;
    ClickState(SharedData *sharedData):BaseState(sharedData)
    {
        gui.addToggle("ClickState", isActive);
    }
    
    void setup()
    {
        BaseState::setup();
        circles.push_back(StaticCircle());
        circles.back().setup();
        circles.push_back(StaticCircle());
        circles.back().setup();
    }
    
    void stateEnter()
    {
        BaseState::stateEnter();
        sharedData->doNoise = 0;
    }
    
    void update()
    {
        BaseState::update();
        fbo->begin();
        ofClear(0);
        for( int i = 0; i < circles.size(); i++ )
        {
            circles[i].update();
            circles[i].draw();
        }
        fbo->end();
    }
    
    void draw()
    {
        BaseState::draw();
    }
    
    string getName()
    {
        return "ClickState";
    }
   
};


#endif
