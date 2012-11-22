//
//  SRandomNoiseState.h
//  motherfarmLED
//
//  Created by motherfarm on 11/19/12.
//
//

#ifndef motherfarmLED_SRandomNoiseState_h
#define motherfarmLED_SRandomNoiseState_h
#include "BaseState.h"
#include "RandomRect.h"

class SRandomNoiseState : public BaseState
{
public:
    vector< RandomRect> rects;
    SRandomNoiseState(SharedData *sharedData):BaseState(sharedData)
    {
        gui.addToggle("SRandomNoiseState", isActive);
    }
    
    void setup()
    {
        BaseState::setup();
        for( int i = 0; i < SVG_WIDTH; i+=5)
        {
            for( int j = 0; j < SVG_HEIGHT; j += 5 )
            {
                rects.push_back( RandomRect() );
                rects.back().setup( i, j, 5);
            }
        }
    }
    
    void stateEnter()
    {
        BaseState::stateEnter();
        sharedData->doNoise = 0;
        for( int i= 0; i < rects.size();i++)
        {
            rects[i].start();
        }
    }
    
    void update()
    {
        BaseState::update();

        fbo->begin();
        ofClear(0);
        {
//            sharedData->noiseAlpha = sharedData->doNoise == 1 ? ofLerp(sharedData->noiseAlpha, 1.f, 0.1)
//            :ofLerp(sharedData->noiseAlpha, 0.f, 0.01);
            for( int i = 0; i < rects.size(); i++ )
            {
                rects[i].update();
                rects[i].draw(sharedData->curveA);
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
        return "SRandomNoiseState";
    }
};
#endif
