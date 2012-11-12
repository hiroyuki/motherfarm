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
#define MAX_CIRCLE 1000

class CircleToCenterState : public BaseState
{
public:
    ofTexture *tex;
    ofFbo fbo;
    ofPixels * colorPixels;
    int longestLen;
    vector<CircleToCenter> circles;
    bool doClear;
    bool toCenter = true;
    void setup()
    {
        BaseState::setup();
        tex = sharedData->tex;
        colorPixels = sharedData->colorPixels;
        longestLen = sqrt(pow(SVG_WIDTH, 2.f) + pow(SVG_HEIGHT, 2.f));
        fbo.allocate(SVG_WIDTH, SVG_HEIGHT, GL_RGBA);
        while( circles.size() < MAX_CIRCLE / 2 )
        {
            circles.push_back(CircleToCenter(toCenter));
            circles[circles.size() -1].init(SVG_WIDTH, SVG_HEIGHT, longestLen);
        }
    }
    
    virtual void stateEnter()
    {
        doClear = true;
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
                    circles.push_back(CircleToCenter(toCenter));
                    circles[ circles.size() - 1].init(SVG_WIDTH, SVG_HEIGHT, longestLen);
                }
            }
        }
        ofDisableLighting();
        ofEnableAlphaBlending();
        glDisable(GL_DEPTH_TEST);
        
        fbo.begin();
        if( doClear ) ofClear(0);
        doClear = false;
        ofSetColor(240);
        fbo.draw(0, 0);
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
        return "CircleToCenterState";
    }
};


#endif