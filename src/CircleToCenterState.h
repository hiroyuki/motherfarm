//
//  Circle2Center.h
//  motherfarmLED
//
//  Created by 堀 宏行 on 11/5/12.
//
//

#ifndef motherfarmLED_Circle2Center_h
#define motherfarmLED_Circle2Center_h

#include "BaseState.h"
#include "CircleToCenter.h"
#define MAX_CIRCLE 200

class CircleToCenterState : public BaseState
{
public:
    int longestLen;
    vector<CircleToCenter> circles;
    bool toCenter;
    CircleToCenterState(SharedData *sharedData):BaseState(sharedData)
    {
        gui.addToggle("CircleToCenterState", isActive);
    }
    
    CircleToCenterState():BaseState()
    {}
    
    void setup()
    {
        toCenter = true;
        BaseState::setup();
        longestLen = sqrt(pow(SVG_WIDTH, 2.f) + pow(SVG_HEIGHT, 2.f));
    }
    
    virtual void stateEnter()
    {
        while( circles.size() < MAX_CIRCLE / 2 )
        {
            circles.push_back(CircleToCenter(circles.size(), toCenter));
            circles.back().init(SVG_WIDTH, SVG_HEIGHT, longestLen);
        }
        BaseState::stateEnter();
    }
    
    void update()
    {
        BaseState::update();
        for( int i = 0; i < circles.size(); i++)
        {
            circles[ i ].update();
            if ( circles[ i ].status == STATUS_HIDE)
            {
                circles[ i ].init(SVG_WIDTH, SVG_HEIGHT, longestLen);
                if ( circles.size() < MAX_CIRCLE )
                {
                    circles.push_back(CircleToCenter(circles.size(), toCenter));
                    circles.back().init(SVG_WIDTH, SVG_HEIGHT, longestLen);
                }
            }
        }
        ofDisableLighting();
        ofEnableAlphaBlending();
        glDisable(GL_DEPTH_TEST);
        
        fbo->begin();
        ofSetColor(240.f*alpha, 240.f*alpha, 240.f*alpha,240.f*alpha);
        fbo->draw(0, 0);
        ofSetColor(255, 255, 255);
        //        glLineWidth(3);
        glPointSize(2);
        
        ofPushMatrix();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        for( int j = 0; j < circles.size(); j++)
        {
            circles[j].draw();
        }
        ofPopMatrix();
        ofDisableBlendMode();
        
        fbo->end();
    }
    
    void draw()
    {
        BaseState::draw();
    }
    
    string getName()
    {
        return "CircleToCenterState";
    }
};


#endif
