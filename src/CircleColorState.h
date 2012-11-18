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
#include "CircleSeed.h"
#define MAX_CIRCLE 100

class CircleColorState : public BaseState
{
public:
    ofTexture *tex;
    ofPixels * colorPixels;
    int longestLen;
    vector<CircleSeed> circles;
    bool doClear;
    
    void setup()
    {
        BaseState::setup();
        tex = sharedData->tex;
        colorPixels = sharedData->colorPixels;
        longestLen = sqrt(pow(SVG_WIDTH, 2.f) + pow(SVG_HEIGHT, 2.f));
        fbo.allocate(SVG_WIDTH, SVG_HEIGHT, GL_RGBA32F_ARB);
        while( circles.size() < MAX_CIRCLE / 2 )
        {
            circles.push_back(CircleSeed());
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
            circles[ i ].update();
            if ( circles[ i ].alpha == 0 && circles[ i ].status == STATUS_HIDE)
            {
                if ( circles.size() < MAX_CIRCLE )
                {
                    circles.push_back(CircleSeed());
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
        
        fbo.begin();
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
        
        fbo.end();
        fbo.readToPixels(*colorPixels);
        tex->loadData(colorPixels->getPixels(), SVG_WIDTH, SVG_HEIGHT, GL_RGBA);
    }
    
    void draw()
    {
        BaseState::draw();
        sharedData->location.drawLed();
        
    }
    
    string getName()
    {
        return "CircleColorState";
    }
};
#endif
