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
        ofDisableLighting();
        ofEnableAlphaBlending();
        glDisable(GL_DEPTH_TEST);
        
        fbo.begin();
        ofClear(0);
        int red = (sin( ofGetElapsedTimeMillis() / 1000.f) * 0.5 + 0.5) * 255;
        int green = (cos( ofGetElapsedTimeMillis() / 1000.f) * 0.5 + 0.5) * 255;
        int blue = (sin( ofGetElapsedTimeMillis() / 1000.f) * 0.5 + 0.5) * 255;
//        cout << "_ " << red << " " << green << " " << blue << endl;
        ofSetColor(0, 0, 255);
        ofRect(0, 0, SVG_WIDTH, SVG_HEIGHT);
        fbo.end();
        fbo.readToPixels(*colorPixels);
        tex->loadData(colorPixels->getPixels(), SVG_WIDTH, SVG_HEIGHT, GL_RGBA);
        if ( sharedData->mode == TEST_ENTTEC)
        {
            Enttec *g = sharedData->getEnttec(sharedData->testEnttec);
            g->setAllData(0xffffff);
        }
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
