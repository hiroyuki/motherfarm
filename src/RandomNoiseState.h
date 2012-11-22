//
//  RandomNoiseState.h
//  motherfarmLED
//
//  Created by motherfarm on 11/19/12.
//
//

#ifndef motherfarmLED_RandomNoiseState_h
#define motherfarmLED_RandomNoiseState_h
#include "BaseState.h"

class RandomNoiseState : public BaseState
{
public:
    RandomNoiseState(SharedData *sharedData):BaseState(sharedData)
    {
        gui.addToggle("RandomNoiseState", isActive);
    }
    
    void setup()
    {
        BaseState::setup();
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
        {
            sharedData->noiseAlpha = sharedData->doNoise == 1 ? ofLerp(sharedData->noiseAlpha, 1.f, 0.1)
            :ofLerp(sharedData->noiseAlpha, 0.f, 0.01);
            for( int i = 0; i < SVG_WIDTH; i+=5)
            {
                for( int j = 0; j < SVG_HEIGHT; j += 5 )
                {
                    if ( ofRandom(1) > 0.5)
                    {
                        ofSetColor(255*sharedData->curveB, 255*sharedData->curveB, 255*sharedData->curveB, 255*sharedData->curveB);
                        ofRect(i, j, 5,5);
                    }
                }
            }
        }
        fbo->end();
    }
    
    void draw()
    {
        BaseState::draw();
    }
    
    string getName()
    {
        return "RandomNoiseState";
    }
};


#endif
