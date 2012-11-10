//
//  TextureDevState.h
//  motherfarmLED
//
//  Created by 堀 宏行 on 11/4/12.
//
//

#ifndef motherfarmLED_TextureDevState_h
#define motherfarmLED_TextureDevState_h
#include "BaseState.h"
#include "CircleStar.h"
#define MAX_CIRCLE 600

class TextureDevState : public BaseState
{
public:
    ofTexture *tex;
    ofFbo fbo;
    ofPixels * colorPixels;
    int longestLen;
    vector<CircleStar> circles;
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
            circles.push_back(CircleStar());
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
                    circles.push_back(CircleStar());
                    circles[ circles.size() - 1].init(SVG_WIDTH, SVG_HEIGHT);
                }
            }
        }
        ofDisableLighting();
        ofEnableAlphaBlending();
        glDisable(GL_DEPTH_TEST);
        
        fbo.begin();
        if( doClear ) ofClear(0);
        doClear = false;
        ofSetColor(240, 240, 240, 240);
        fbo.draw(0, 0);
        ofSetColor(255, 255, 255);
        //        glLineWidth(3);
        glPointSize(2);
        
        ofPushMatrix();
        ofTranslate(longestLen/2, longestLen/2);
        ofRotate(sharedData->angle, 0, 0, 1);
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        for( int j = 0; j < circles.size(); j++)
        {
            circles[j].draw(longestLen/2, longestLen/2);
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
        return "TextureDevState";
    }
};
#endif
