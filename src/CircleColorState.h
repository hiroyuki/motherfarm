//
//  CircleColorState.h
//  motherfarmLED
//
//  Created by 堀 宏行 on 11/5/12.
//
//

#ifndef motherfarmLED_CircleColorState_h
#define motherfarmLED_CircleColorState_h
#include "BaseState.h"
#include "CircleSeed2.h"
#define MAX_CIRCLE 80

class CircleColorState : public BaseState
{
public:
    int longestLen;
    vector<CircleSeed2> circles;
    bool doClear;
    
    CircleColorState(SharedData *sharedData):BaseState(sharedData)
    {
        gui.addToggle("CircleColorState", isActive);
    }
    
    void setup()
    {
        BaseState::setup();
        longestLen = sqrt(pow(SVG_WIDTH, 2.f) + pow(SVG_HEIGHT, 2.f));
        while( circles.size() < MAX_CIRCLE / 2 )
        {
            circles.push_back(CircleSeed2());
            circles[circles.size() -1].init(SVG_WIDTH, SVG_HEIGHT);
        }
    }
    
    void stateEnter()
    {
        BaseState::stateEnter();
    }
    
    void update()
    { 
        BaseState::update();
        for( int i = 0; i < circles.size(); i++)
        {
            circles[ i ].update(sharedData->gotBang250, sharedData->curveA);
            if ( circles[ i ].alpha == 0 && circles[ i ].status == STATUS_HIDE)
            {
                if ( circles.size() < MAX_CIRCLE )
                {
                    circles.push_back(CircleSeed2());
                    circles[ circles.size() - 1].color.r = ofRandom(0xff);
                    circles[ circles.size() - 1].color.g = ofRandom(0xff);
                    circles[ circles.size() - 1].color.b = ofRandom(0xff);
                    circles[ circles.size() - 1].init(SVG_WIDTH, SVG_HEIGHT);
                }
                circles[ i ].init(SVG_WIDTH, SVG_HEIGHT);
            }
        }
        ofDisableLighting();
        ofEnableAlphaBlending();
        glDisable(GL_DEPTH_TEST);
        
        fbo->begin();
        ofClear(0);
        ofSetColor(255, 255, 255);
        //        glLineWidth(3);
        glPointSize(3);
        
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
        return "CircleColorState";
    }
};
#endif
