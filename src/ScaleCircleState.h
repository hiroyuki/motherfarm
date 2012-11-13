//
//  ScaleCircleState.h
//  motherfarmLED
//
//  Created by 堀 宏行 on 11/5/12.
//
//

#ifndef motherfarmLED_ScaleCircleState_h
#define motherfarmLED_ScaleCircleState_h
#include "BaseState.h"
#include "ScaleCircle.h"
#define MAX_CIRCLE 5
class ScaleCircleState : public BaseState
{
public:
    ofTexture *tex;
    ofFbo fbo;
    ofPixels * colorPixels;
    int longestLen;
    vector<ScaleCircle> circles;
    bool doClear;
    
    void setup()
    {
        BaseState::setup();
        tex = sharedData->tex;
        colorPixels = sharedData->colorPixels;
        longestLen = sqrt(pow(SVG_WIDTH, 2.f) + pow(SVG_HEIGHT, 2.f));
        
        fbo.allocate(SVG_WIDTH, SVG_HEIGHT, GL_RGBA32F_ARB);
        int no = 0;
        while( circles.size() < MAX_CIRCLE / 2 )
        {
            no++;
            circles.push_back(ScaleCircle(no));
            circles[circles.size() -1].init(SVG_WIDTH, SVG_HEIGHT);
        }
    }
    
    void stateEnter()
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
                circles[ i ].init(SVG_WIDTH, SVG_HEIGHT);
                if ( circles.size() < MAX_CIRCLE )
                {
                    circles.push_back(ScaleCircle(i));
                    circles[ circles.size() - 1].init(SVG_WIDTH, SVG_HEIGHT);
                }
            }
        }
        ofDisableLighting();
        ofEnableAlphaBlending();
        glDisable(GL_DEPTH_TEST);
        for( int j = 0; j < circles.size(); j++)
        {
            circles[j].updateImage(longestLen);
        }
        fbo.begin();
        if( doClear ) ofClear(0);
        doClear = false;
        
        ofSetColor(253, 253, 253, 240);
        fbo.draw(0, 0);
        ofSetColor(255, 255, 255);
        //        glLineWidth(3);
        glPointSize(2);
        
        ofPushMatrix();
            ofEnableBlendMode(OF_BLENDMODE_ADD);
            for( int j = 0; j < circles.size(); j++)
            {
                circles[j].draw(longestLen);
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
        return "ScaleCircleState";
    }
};


#endif
