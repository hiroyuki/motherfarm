//
//  ParseState.h
//  motherfarmLED
//
//  Created by 堀 宏行 on 11/4/12.
//
//

#ifndef motherfarmLED_ParseState_h
#define motherfarmLED_ParseState_h
#include "BaseState.h"

class ParseState : public BaseState
{
public:
    void update()
    {
        sharedData->location.update();
    }
  
    void draw()
    {
        sharedData->location.debugDraw();
    }
    
    string getName()
    {
        return "ParseState";
    }
};
#endif
