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
    int longestLen;
    vector<CircleStar> circles;
    TextureDevState(SharedData *sharedData):BaseState(sharedData){}
    
    void setup()
    {
        BaseState::setup();
        longestLen = sqrt(pow(SVG_WIDTH, 2.f) + pow(SVG_HEIGHT, 2.f));
        while( circles.size() < MAX_CIRCLE / 2 )
        {
            circles.push_back(CircleStar());
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
        ofDisableLighting();
        ofEnableAlphaBlending();
        glDisable(GL_DEPTH_TEST);
        
        fbo->begin();
        ofClear(0);
        int red = (sin( ofGetElapsedTimeMillis() / 1000.f) * 0.5 + 0.5) * 255;
        int green = (cos( ofGetElapsedTimeMillis() / 1000.f) * 0.5 + 0.5) * 255;
        int blue = (sin( ofGetElapsedTimeMillis() / 1000.f) * 0.5 + 0.5) * 255;
//        cout << "_ " << red << " " << green << " " << blue << endl;
        ofSetColor(0, 0, 0);
        ofRect(0, 0, SVG_WIDTH, SVG_HEIGHT);
        fbo->end();
        if ( sharedData->mode == TEST_ENTTEC)
        {
            Enttec *g = sharedData->getEnttec(sharedData->testEnttec);
            g->setAllData(0xffffff);
        }
    }
    
    void draw()
    {
        BaseState::draw();
    }
    
    string getName()
    {
        return "TextureDevState";
    }
};
#endif
